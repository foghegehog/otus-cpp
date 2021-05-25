cmake_minimum_required(VERSION 3.2)

if ($ENV{TRAVIS_BUILD_NUMBER})
    project(join_server VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else()
    project(join_server VERSION 0.0.1)
endif()

find_package(Boost COMPONENTS system REQUIRED)
find_package(Threads)
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIR})
include_directories(${Boost_INCLUDE_DIR})

add_executable(join_server
    src/main.cpp
    src/command_parser.cpp
    src/table.cpp
    src/database.cpp)

add_executable(unit_tests 
    tests/tests.cpp
    src/table.cpp
)

set_target_properties(join_server PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        )

set_target_properties(unit_tests PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        )

target_link_libraries(join_server
        ${Boost_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT})

target_link_libraries(unit_tests
        gtest
        pthread)

if (MSVC)
    target_compile_options(join_server PRIVATE
        /W4
    )
    target_compile_options(unit_tests PRIVATE
        /W4
    )
else ()
    target_compile_options(join_server PRIVATE
        -Wall -Wextra -pedantic -Werror 
    )
    target_compile_options(unit_tests PRIVATE
        -Wall -Wextra -pedantic -Werror 
    )
endif()

install(TARGETS join_server RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)

enable_testing()

add_test(unit_tests unit_tests)


