project(Providers)

set(SOURCE_FILES
        LinuxResProvider.cpp
        ResProvider.cpp
        WinResProvider.cpp)

set(HEADER_FILES
        LinuxResProvider.h
        ResProvider.h
        WinResProvider.h)

add_library(Providers_lib STATIC ${SOURCE_FILES} ${HEADER_FILES})

