cmake_minimum_required(VERSION 3.2)

if ($ENV{TRAVIS_BUILD_NUMBER})
    project(bulk_server VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else()
    project(bulk_server VERSION 0.0.1)
endif()

find_package(Boost COMPONENTS system REQUIRED)
find_package(Threads)
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIR})
include_directories(${Boost_INCLUDE_DIR})

add_executable(bulk_server 
src/bulk_server.cpp
src/async.cpp
src/commands_parsing.cpp
src/context.cpp
src/handlers_factory.cpp
src/handlers/handler.cpp
src/handlers/accumulator.cpp
src/handlers/control_unit.cpp
src/handlers/accumulate_handler.cpp
src/handlers/control_unit_handler.cpp
src/handlers/processing_handler.cpp
src/handlers/handlers_chain.cpp
src/handlers/threadsafe_accumulator.cpp
src/postprocessing/postprocessing.cpp
src/postprocessing/bulk_writer.cpp
src/postprocessing/logging_handler.cpp
src/postprocessing/output_handler.cpp)

add_executable(unit_tests 
    src/handlers/control_unit.cpp
    tests/control_unit_tests.cpp)

set_target_properties(bulk_server PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        )

set_target_properties(unit_tests PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        )

target_link_libraries(bulk_server
        ${Boost_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT})

target_link_libraries(unit_tests
        gtest
        pthread)

if (MSVC)
    target_compile_options(bulk_server PRIVATE
        /W4
    )
    target_compile_options(unit_tests PRIVATE
        /W4
    )
else ()
    target_compile_options(bulk_server PRIVATE
        -Wall -Wextra -pedantic -Werror 
    )
    target_compile_options(unit_tests PRIVATE
        -Wall -Wextra -pedantic -Werror 
    )
endif()

enable_testing()

add_test(unit_tests unit_tests)


