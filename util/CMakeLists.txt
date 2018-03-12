
add_executable (
    passgen 
    passgen.cpp
    ${INCLUDE_DIRECTORIES}
    )
target_link_libraries(passgen ${ROLLRAT_LIBRARY})

add_executable (
    renamer
    renamer.cpp
    ${INCLUDE_DIRECTORIES}
    )
target_link_libraries(renamer ${ROLLRAT_LIBRARY} )

add_executable (
    index
    index.cpp
    ${INCLUDE_DIRECTORIES}
    )
target_link_libraries(index ${ROLLRAT_LIBRARY} )