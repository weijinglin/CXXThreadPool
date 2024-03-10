find_package(GTest REQUIRED)

add_executable(test mythread_test.cpp)
target_link_libraries(test GTest::gtest_main mythread)

include(GoogleTest)
gtest_discover_tests(test)