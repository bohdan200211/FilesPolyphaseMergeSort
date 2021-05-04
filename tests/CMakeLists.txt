add_executable(MyTests main_tests.cpp)

target_link_libraries(MyTests PRIVATE doctest_with_main)