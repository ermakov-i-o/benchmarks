cmake_minimum_required(VERSION 3.19 FATAL_ERROR)

project(benchmarks
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(GenerateExportHeader)

set(GENERATED_HEADERS_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated_headers")
set(EXPORT_HEADER "${GENERATED_HEADERS_DIR}/benchmarks/export.hpp")

set(LINUX_SOURCES
    src/tools/linux/cpu_cache_info.cpp
)

add_library(benchmarks
    include/benchmarks/memory.hpp
    src/memory.cpp
    src/memory/cycle_unroll_benchmark.cpp
    src/memory/cycle_unroll_benchmark.hpp
    src/tools/cpu_cache_info.hpp
    src/tools/data.hpp
    src/tools/execution.hpp
    src/tools/stopwatch.hpp
    ${LINUX_SOURCES}
)
add_library(benchmarks::benchmarks ALIAS benchmarks)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
#elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
#    set_source_files_properties(${LINUX_SOURCES}
#        PROPERTIES
#            HEADER_FILE_ONLY ON
#    )
else()
    message(FATAL_ERROR "Unknown OS!")
endif()

set_target_properties(benchmarks
    PROPERTIES
        DEFINE_SYMBOL BENCHMARKS_EXPORT_SYMBOL #before generate_export_header()
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN ON
        DEBUG_POSTFIX "-d"
        RELWITHDEBINFO_POSTFIX "-rd"
        MINSIZEREL_POSTFIX "-min"
)
generate_export_header(benchmarks
    BASE_NAME benchmarks
    EXPORT_FILE_NAME "${EXPORT_HEADER}"
    INCLUDE_GUARD_NAME BENCHMARKS_EXPORT_HEADER
)
target_include_directories(benchmarks
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<BUILD_INTERFACE:${GENERATED_HEADERS_DIR}>
        $<INSTALL_INTERFACE:include>
)
target_compile_definitions(benchmarks
    PUBLIC
        $<$<NOT:$<BOOL:${BUILD_SHARED_LIBS}>>:BENCHMARKS_STATIC_DEFINE>
)


install(TARGETS benchmarks
    EXPORT benchmarks-targets
    COMPONENT runtime
)

set(CONFIG_INSTALL_DIR lib/cmake/benchmarks)

install(EXPORT benchmarks-targets
    NAMESPACE benchmarks::
    DESTINATION ${CONFIG_INSTALL_DIR}
    COMPONENT development-cmake
)

set(CONFIG_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/benchmarks-config.cmake")
set(VERSION_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/benchmarks-config-version.cmake")

include(CMakePackageConfigHelpers)

configure_package_config_file(cmake/benchmarks-config.cmake "${CONFIG_FILE_NAME}"
    INSTALL_DESTINATION "${CONFIG_INSTALL_DIR}"
)
write_basic_package_version_file("${VERSION_FILE_NAME}"
    VERSION "${benchmarks_VERSION}"
    COMPATIBILITY SameMajorVersion
)

install(DIRECTORY include/
    TYPE INCLUDE
    COMPONENT development
)
install(DIRECTORY "${GENERATED_HEADERS_DIR}/"
    TYPE INCLUDE
    COMPONENT development
)
install(
    FILES
    "${CONFIG_FILE_NAME}"
    "${VERSION_FILE_NAME}"
    DESTINATION "${CONFIG_INSTALL_DIR}"
    COMPONENT development
)
