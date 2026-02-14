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
# Options
# -----------------------------
option(ENABLE_HTTPS "Enable HTTPS support (cpp-httplib)" ON)

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
# HTTPS / TLS support
# -----------------------------
if (ENABLE_HTTPS)
    find_package(OpenSSL)

    if (OpenSSL_FOUND)
        message(STATUS "HTTPS enabled: using OpenSSL")

        target_compile_definitions(main PRIVATE CPPHTTPLIB_OPENSSL_SUPPORT)

        target_link_libraries(main PRIVATE
            OpenSSL::SSL
            OpenSSL::Crypto
        )

        if (WIN32)
            target_link_libraries(main PRIVATE
                crypt32
                ws2_32
            )
        endif()

    else()
        message(STATUS "OpenSSL not found, trying mbedTLS")

        find_package(mbedTLS)

        if (mbedTLS_FOUND)
            message(STATUS "HTTPS enabled: using mbedTLS")

            target_compile_definitions(main PRIVATE CPPHTTPLIB_MBEDTLS_SUPPORT)

            target_link_libraries(main PRIVATE
                mbedtls
                mbedcrypto
                mbedx509
            )

        else()
            message(FATAL_ERROR
                "ENABLE_HTTPS is ON but no TLS backend was found (OpenSSL or mbedTLS)"
            )
        endif()
    endif()
endif()
