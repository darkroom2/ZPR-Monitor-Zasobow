project(Config)

set(SOURCE_FILES
        ConfigLoader.cpp)

set(HEADER_FILES
        ConfigLoader.h)

add_library(Config_lib STATIC ${SOURCE_FILES} ${HEADER_FILES})