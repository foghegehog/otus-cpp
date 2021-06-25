cmake_minimum_required(VERSION 3.5)

if ($ENV{TRAVIS_BUILD_NUMBER})
    project(realty VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else()
    project(realty VERSION 0.0.1)
endif()

add_subdirectory(dlib dlib_build)

add_executable(rclst rclst.cpp)

set_target_properties(rclst PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        )

target_link_libraries(rclst
        dlib
        )

