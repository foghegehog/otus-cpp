cmake_minimum_required(VERSION 3.5)

if ($ENV{TRAVIS_BUILD_NUMBER})
    project(realty VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else()
    project(realty VERSION 0.0.1)
endif()

set(CMAKE_CXX_CLANG_TIDY clang-tidy)

add_subdirectory(dlib dlib_build)

add_executable(rclst rclst.cpp io_func.cpp)

add_executable(rclss rclss.cpp io_func.cpp)

set_target_properties(rclst PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    )

set_target_properties(rclss PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    )

target_link_libraries(rclst dlib)

target_link_libraries(rclss dlib)

if (MSVC)
    target_compile_options(rclst PRIVATE
        /W4
    )
    target_compile_options(rclss PRIVATE
        /W4
    )
else ()
    target_compile_options(rclst PRIVATE
        -Wall -Wextra -pedantic -Werror 
    )
    target_compile_options(rclss PRIVATE
        -Wall -Wextra -pedantic -Werror 
    )  
endif()


install(TARGETS rclst RUNTIME DESTINATION bin)
install(TARGETS rclss LIBRARY DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)

