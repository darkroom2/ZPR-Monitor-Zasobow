set(Boost_USE_STATIC_LIBS OFF)
find_package(Boost REQUIRED COMPONENTS unit_test_framework system thread)
include_directories(${Boost_INCLUDE_DIRS})

# 'Boost_Tests_run' is the target name
# 'test1.cpp' are source files with tests
add_executable(Boost_Tests_run test1.cpp)
target_link_libraries(Boost_Tests_run ${Boost_LIBRARIES})
target_link_libraries(Boost_Tests_run Managers_lib Config_lib Providers_lib)