add_executable(MyMain main.cpp)
add_executable(MyTable tables.cpp)

target_link_libraries(MyMain PRIVATE PolyphaseSortLib)
target_link_libraries(MyTable PRIVATE PolyphaseSortLib libfort::fort)