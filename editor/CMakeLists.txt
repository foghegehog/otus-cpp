cmake_minimum_required(VERSION 3.5)

if ($ENV{TRAVIS_BUILD_NUMBER})
    project(editor VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else()
    project(editor VERSION 0.0.1)
endif()

add_executable(editor main.cpp model.cpp)
set_target_properties(editor PROPERTIES CXX_STANDARD 14 CXX_STANDARD_REQUIRED ON)

install(TARGETS editor RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)
