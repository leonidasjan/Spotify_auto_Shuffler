# -----------------------------
# This CMAKE list is the only file in this repo that was made by AI.
# -----------------------------
cmake_minimum_required(VERSION 3.20)
project(SPOTIFY_AUTO_SHUFFLER VERSION 1.0 LANGUAGES CXX)

# -----------------------------
# C++ standard
# -----------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# -----------------------------
# Sources
# -----------------------------
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
)

add_executable(main ${SOURCES})

# -----------------------------
# Include directories
# -----------------------------
target_include_directories(main PRIVATE
    ${CMAKE_SOURCE_DIR}/src/include
)

# -----------------------------
# Compiler warnings (portable)
# -----------------------------
if (MSVC)
    target_compile_options(main PRIVATE /W4)
else()
    target_compile_options(main PRIVATE
        -Wall
        -Wextra
        -pedantic
    )
endif()

# -----------------------------
# Platform-specific system libs
# -----------------------------
find_package(OpenSSL REQUIRED)

target_link_libraries(main PRIVATE
    OpenSSL::SSL
    OpenSSL::Crypto
)

if (WIN32)
    target_link_libraries(main PRIVATE
        ws2_32
        crypt32
    )
endif()
