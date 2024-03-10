find_package(GTest REQUIRED)

add_executable(test mythread_test.cc)
target_link_libraries(test GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(test)