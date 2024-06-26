cmake_minimum_required(VERSION 3.5)

project(07.homework)

#download googletest sources at configure time to build directory
#build from sources at build time
include(FetchContent)
#describe how to get content
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
#it is advisable to use a hash for GIT_TAG rather than a branch or tag name.
#A commit hash is more secure and helps to confirm that the downloaded contents are what you expected.
  GIT_TAG        f8d7d77c06936315286eb55f8de22cd23c188571 # release-1.14
)
#make sure that all the dependencies are available
FetchContent_MakeAvailable(googletest)

#no add_subdirectory to googletest requires!

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

add_executable(
    test_containers
    test/main.cpp
    test/MyVector_tests.cpp
    test/MyList_tests.cpp
)

# Add src to the include directories for the test target
target_include_directories(
    test_containers PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
)

# Link test target against gtest libraries
target_link_libraries(
    test_containers PRIVATE
    gtest
)

set_target_properties(test_containers PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)

# if (MSVC)
#     target_compile_options(
#         test_containers PRIVATE
#         /W4
#     )    
# else ()
#     target_compile_options(
#         test_containers PRIVATE
#         -Wall -Wextra -pedantic -Werror
#     )    
# endif()
