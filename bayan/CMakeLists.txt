cmake_minimum_required(VERSION 3.2)

if ($ENV{TRAVIS_BUILD_NUMBER})
    project(bayan VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else()
    project(bayan VERSION 0.0.1)
endif()

find_package(Boost COMPONENTS program_options unit_test_framework filesystem REQUIRED)

add_executable(bayan main.cpp)
add_library(lib settings.cpp)
add_executable(test_bayan test_options.cpp)

set_target_properties(bayan lib test_bayan PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)
target_include_directories(bayan
    PRIVATE "${CMAKE_BINARY_DIR}"
)
set_target_properties(test_bayan PROPERTIES
    COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
    INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
)

target_link_libraries(lib PRIVATE
    ${Boost_LIBRARIES}
)

target_link_libraries(bayan PRIVATE
    ${Boost_LIBRARIES}
    lib
)
target_link_libraries(test_bayan
    ${Boost_LIBRARIES}
    lib
)

if (MSVC)
    target_compile_options(bayan PRIVATE
        /W4
    )
    target_compile_options(lib PRIVATE
        /W4
    )
    target_compile_options(test PRIVATE
        /W4
    )
else ()
    target_compile_options(bayan PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(lib PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(test_bayan PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()



install(TARGETS bayan RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)

enable_testing()

add_test(test_bayan test)
