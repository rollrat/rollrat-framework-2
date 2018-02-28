set(SOURCES
    test.cpp
    test-StringTools.cpp
    Test.h
    ${INCLUDE_DIRECTORIES}
    )

add_executable(rftest ${SOURCES})
target_link_libraries(rftest ${ROLLRAT_LIBRARY})
add_test(rftest_test rftest)