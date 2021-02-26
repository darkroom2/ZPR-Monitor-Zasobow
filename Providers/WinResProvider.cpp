#ifdef _WIN32

#include "WinResProvider.h"

void WinResProvider::init() {

    // krok 1
    // inicjalizacja COM (Component Object Model)

    this->hres = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    if (FAILED(this->hres)) {
        WinResProvider::print_error("Failed to initialize COM library. Error code = 0x", this->hres);
        throw std::exception();
    }

    // krok 2
    // ustawienie poziomow bezpieczenstwa

    this->hres = CoInitializeSecurity(
            nullptr,                        // Security descriptor
            -1,                          // COM negotiates authentication service
            nullptr,                        // Authentication services
            nullptr,                        // Reserved
            RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication level for proxies
            RPC_C_IMP_LEVEL_IMPERSONATE, // Default Impersonation level for proxies
            nullptr,                        // Authentication info
            EOAC_NONE,                   // Additional capabilities of the client or server
            nullptr                         // Reserved
    );

    if (FAILED(this->hres)) {
        WinResProvider::print_error("Failed to initialize security. Error code = 0x", this->hres);
        CoUninitialize();
        throw std::exception();
    }

    // krok 3
    // pozyskanie uchwytu do WMI

    this->hres = CoCreateInstance(
            CLSID_WbemLocator,
            nullptr,
            CLSCTX_INPROC_SERVER,
            IID_IWbemLocator,
            (LPVOID *) &pLoc
    );

    if (FAILED(this->hres)) {
        WinResProvider::print_error("Failed to create IWbemLocator object. Error code = 0x", this->hres);
        CoUninitialize();
        throw std::exception();
    }


    // krok 4
    // podlaczenie do WMI (Windows Management Instrumentation) poprzez IWbemLocator::ConnectServer()

    // podlaczamy sie do przestrzeni nazw CIMV2 gdzie znajdziemy informacje o pamieci fizycznej
    this->hres = this->pLoc->ConnectServer(
            BSTR(L"ROOT\\CIMV2"),      // Object path of WMI namespace
            nullptr,                      // User name. NULL = current user
            nullptr,                      // User password. NULL = current
            nullptr,                      // Locale. NULL indicates current
            NULL,                      // Security flags.
            nullptr,                      // Authority (for example, Kerberos)
            nullptr,                      // Context object
            &this->pSvc                      // pointer to IWbemServices proxy
    );

    if (FAILED(this->hres)) {
        WinResProvider::print_error("Failed to connect to ROOT\\CIMV2 namespace. Error code = 0x", this->hres);
        this->pLoc->Release();
        CoUninitialize();
        throw std::exception();
    }

//    std::cout << "Successfuly connected to ROOT\\CIMV2 WMI namespace" << std::endl;

    // krok 5
    // ustawiamy poziomy bezpieczenstwa u posrednika WMI

    this->hres = CoSetProxyBlanket(
            this->pSvc,                        // Indicates the proxy to set
            RPC_C_AUTHN_WINNT,           // RPC_C_AUTHN_xxx
            RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx
            nullptr,                        // Server principal name
            RPC_C_AUTHN_LEVEL_CALL,      // RPC_C_AUTHN_LEVEL_xxx
            RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx
            nullptr,                        // client identity
            EOAC_NONE                    // proxy capabilities
    );

    if (FAILED(this->hres)) {
        WinResProvider::print_error("Failed to set proxy blanket. Error code = 0x", this->hres);
        this->pLoc->Release();
        this->pSvc->Release();
        CoUninitialize();
        throw std::exception();
    }

}

std::wstring WinResProvider::query_WMI(const std::wstring &property_name, const std::wstring &class_name,
                                       const std::wstring &where_condition) {
    std::wstring result;

    std::wstring query_string;
    query_string += L"SELECT ";
    query_string += property_name;
    query_string += L" FROM ";
    query_string += class_name;
    if (!where_condition.empty()) {
        query_string += L" WHERE ";
        query_string += where_condition;
    }

    // krok 6
    // Uzycie uchwytu IWbemService do wykonania zapytania do WMI

    IEnumWbemClassObject *pEnumerator = nullptr;    // *NEW* czyszczenie metoda ->Release();

    this->hres = this->pSvc->ExecQuery(
            BSTR(L"WQL"),
            BSTR(query_string.c_str()),
            NULL,
            nullptr,
            &pEnumerator
    );

    if (FAILED(this->hres)) {
        WinResProvider::print_error("Failed to query WMI class. Error code = 0x",
                                    this->hres);
        this->pLoc->Release();
        this->pSvc->Release();
        CoUninitialize();
        throw std::exception();
    }

    // krok 7
    // Wyjecie danych z struktury wyliczajacej (enumerator)

    ULONG uReturn = 0;

    while (pEnumerator) {
        this->hres = pEnumerator->Next(WBEM_INFINITE, 1, &this->pclsObj, &uReturn);
        if (this->hres != 0) {
            break;
        }
        result = this->get_property(property_name);
        this->pclsObj->Release();
    }

    if (pEnumerator)
        pEnumerator->Release();

    return result;
}

std::wstring WinResProvider::get_property(const std::wstring &property_name) {
    VARIANT vtProperty;
    VariantInit(&vtProperty);

    std::wstring result;

    try {
        this->hres = this->pclsObj->Get(property_name.c_str(), NULL, &vtProperty, nullptr, nullptr);

        if (vtProperty.vt == VT_BSTR) {
            result = vtProperty.bstrVal;
        } else if (vtProperty.vt == VT_I4) {
            result = std::to_wstring(vtProperty.iVal);
        }
    }
    catch (...) {
        std::cerr << "Value from query invalid." << std::endl;
    }
    VariantClear(&vtProperty);
    return result;
}

void WinResProvider::print_error(const std::string &msg, HRESULT hr) {
    std::cerr << msg << std::hex << hr << std::endl;
}

WinResProvider::WinResProvider() {
    try {
        this->init();
    }
    catch (...) {
        std::cout << "WinResProvider()::Catched exception" << std::endl;
    }
}

int WinResProvider::getCpuUsage() {
    return 100 - std::stoi(
            this->query_WMI(L"PercentIdleTime", L"Win32_PerfFormattedData_PerfOS_Processor", L"Name = '_Total'"));
}

int WinResProvider::getRamUsage() {
    return std::stoi(this->query_WMI(L"PercentCommittedBytesInUse", L"Win32_PerfFormattedData_PerfOS_Memory"));
}

int WinResProvider::getHddUsage() {
    return 100 -
           std::stoi(this->query_WMI(L"PercentIdleTime", L"Win32_PerfFormattedData_PerfDisk_PhysicalDisk",
                                     L"Name = '_Total'"));
}

WinResProvider::~WinResProvider() {
    if (this->pSvc)
        this->pSvc->Release();
    if (this->pLoc)
        this->pLoc->Release();
    CoUninitialize();
}

#endif // _WIN32
