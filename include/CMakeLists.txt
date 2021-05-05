file(GLOB_RECURSE HEADER_FILES
        *.hpp
        )

file(GLOB_RECURSE SOURCE_FILES
        *.cpp
        )

add_library(PolyphaseSortLib STATIC ${HEADER_FILES} ${SOURCE_FILES})

target_include_directories(PolyphaseSortLib PUBLIC Sort)

