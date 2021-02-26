project(Managers)

set(SOURCE_FILES
        ConditionManager.cpp
        PollManager.cpp
        ServerManager.cpp)

set(HEADER_FILES
        ConditionManager.h
        PollManager.h
        ServerManager.h)

add_library(Managers_lib STATIC ${SOURCE_FILES} ${HEADER_FILES})