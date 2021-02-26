#include "ResProvider.h"

#ifdef __linux__

#include "LinuxResProvider.h"

#endif // __linux__

#ifdef _WIN32
#include "WinResProvider.h"
#endif // _WIN32

ResProvider::ResProvider() {}

ResProvider *ResProvider::makeResProvider() {
#ifdef __linux__
    return new LinuxResProvider;
#endif // __linux__

#ifdef _WIN32
    return new WinResProvider;
#endif // _WIN32
}

ResProvider::~ResProvider() = default;
