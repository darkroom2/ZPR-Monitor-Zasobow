#pragma once

#ifdef _WIN32

#include "ResProvider.h"

#define _WIN32_DCOM

#include <iostream>

#include <Windows.h>
#include <WbemIdl.h>

#pragma comment(lib, "wbemuuid.lib")

class WinResProvider : public ResProvider {

    // private members
    HRESULT hres{};
    IWbemClassObject *pclsObj = nullptr;    // *NEW* czyszczenie metoda
    IWbemServices *pSvc = nullptr;    // *NEW* czyszczenie metoda
    IWbemLocator *pLoc = nullptr; // *NEW* czyszczenie pamieci poprzez dedykowana metode pLoc->Release();

    // private functions
    void init();

    std::wstring
    query_WMI(const std::wstring &property_name, const std::wstring &class_name,
              const std::wstring &where_condition = L"");

    std::wstring get_property(const std::wstring &property_name);

    static void print_error(const std::string &msg, HRESULT hr);

public:
    WinResProvider();

    // Inherited via ResProvider
    virtual int getCpuUsage() override;

    virtual int getRamUsage() override;

    virtual int getHddUsage() override;

    virtual ~WinResProvider();

};

#endif // _WIN32
