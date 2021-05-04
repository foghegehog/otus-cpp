cmake_minimum_required(VERSION 3.2)

if ($ENV{TRAVIS_BUILD_NUMBER})
    project(async VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else()
    project(async VERSION 0.0.1)
endif()

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIR})

add_executable(demo main.cpp)
add_library(async SHARED 
    async.cpp
    commands_parsing.cpp
    context.cpp
    handlers/handler.cpp
    handlers/accumulator.cpp
    handlers/control_unit.cpp
    handlers/accumulate_handler.cpp
    handlers/control_unit_handler.cpp
    handlers/processing_handler.cpp
    handlers/handlers_chain.cpp
    postprocessing/postprocessing.cpp
    postprocessing/bulk_writer.cpp
    postprocessing/logging_handler.cpp
    postprocessing/output_handler.cpp)
add_executable(unit_tests 
    tests/control_unit_tests.cpp
    handlers/control_unit.cpp)

set_target_properties(demo async unit_tests PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)
target_include_directories(async
    PRIVATE "${CMAKE_BINARY_DIR}"
)

target_link_libraries(demo PRIVATE
    async Threads::Threads
)

target_link_libraries(unit_tests gtest pthread)

if (MSVC)
    target_compile_options(async PRIVATE
        /W4
    )
    target_compile_options(demo PRIVATE
        /W4
    )
    target_compile_options(unit_tests PRIVATE
        /W4
    )
else ()
    target_compile_options(async PRIVATE
        -Wall -Wextra -pedantic -Werror 
    )
    target_compile_options(demo PRIVATE
        -Wall -Wextra -pedantic -Werror 
    ) 
    target_compile_options(unit_tests PRIVATE
        -Wall -Wextra -pedantic -Werror 
    )
endif()


install(TARGETS demo RUNTIME DESTINATION bin)
install(TARGETS async LIBRARY DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)

enable_testing()

add_test(unit_tests unit_tests)
