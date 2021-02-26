cmake_minimum_required(VERSION 3.15)
project(ZPR_Monitor_Zasobow)

set(CMAKE_CXX_STANDARD 14)

add_executable(
        ZPR_Monitor_Zasobow
        "ZPR_Monitor.cpp"
        "Providers/LinuxResProvider.h"
        "Providers/LinuxResProvider.cpp"
        "Providers/ResProvider.h"
        "Providers/ResProvider.cpp"
        "Providers/WinResProvider.h"
        "Providers/WinResProvider.cpp"
        "Config/ConfigLoader.cpp"
        "Config/ConfigLoader.h"
        "Managers/ConditionManager.cpp"
        "Managers/ConditionManager.h"
        "Managers/PollManager.cpp"
        "Managers/PollManager.h"
        "Managers/ServerManager.cpp"
        "Managers/ServerManager.h"
        include/json.h
)

if (UNIX)
    find_package(Threads)
    target_link_libraries(ZPR_Monitor_Zasobow Threads::Threads)
endif ()

include_directories(Managers Providers Config)

#include(FindPkgConfig)
#pkg_check_modules(CURLPP REQUIRED curlpp)

add_subdirectory(Managers)
add_subdirectory(Providers)
add_subdirectory(Config)

#target_link_libraries(ZPR_Monitor_Zasobow curl)
#target_link_libraries(ZPR_Monitor_Zasobow curlpp)
target_link_libraries(ZPR_Monitor_Zasobow Managers_lib Providers_lib Config_lib)

#if jest boost
#add_subdirectory(Boost_tests)
#else