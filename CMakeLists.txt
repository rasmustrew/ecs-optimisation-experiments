cmake_minimum_required(VERSION 3.20)
project(ECSExperiments LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
option(ENABLE_CLANG_TIDY_ON_BUILD "Run clang-tidy during normal builds" ON)
option(ENABLE_AMD_UPROF "Enable AMD uProf API integration" OFF)


file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "src/*.h")

add_executable(ECSExperiments ${SOURCES} ${HEADERS})

source_group(TREE "${CMAKE_SOURCE_DIR}/src"     PREFIX "Source Files" FILES ${SOURCES} ${HEADERS})

target_include_directories(ECSExperiments PRIVATE
    src
    ${Stb_INCLUDE_DIR}
)

if(ENABLE_AMD_UPROF)
    target_include_directories(ECSExperiments PRIVATE
        "C:/Program Files/AMD/AMDuProf/include"
    )
    target_compile_definitions(ECSExperiments PRIVATE USE_AMD_UPROF=1)
    target_link_libraries(ECSExperiments PRIVATE
        "C:/Program Files/AMD/AMDuProf/lib/x64/AMDProfileController.lib"
    )
endif()

if(MSVC)
    target_compile_options(ECSExperiments PRIVATE
        /W4
        /utf-8
        /permissive-
        /external:W0
        $<$<CONFIG:Debug>:/Od /Ob0 /Oy->
        $<$<CONFIG:RelWithDebInfo>:/Zi /Zo /Ob0 /Oy->
        $<$<CONFIG:Release>:/Gy /Oi>
        /WX
        /w44365
        /sdl
    )

    target_link_options(ECSExperiments PRIVATE
        $<$<CONFIG:RelWithDebInfo>:/DEBUG:FULL>
    )
endif()

include(CheckIPOSupported)
check_ipo_supported(RESULT _ipo_supported)
if(_ipo_supported)
    set_property(TARGET ECSExperiments PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)
    set_property(TARGET ECSExperiments PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELWITHDEBINFO FALSE)
endif()

get_filename_component(_compiler_dir "${CMAKE_CXX_COMPILER}" DIRECTORY)
find_program(CLANG_TIDY_EXE
    NAMES clang-tidy
    HINTS "${_compiler_dir}"
)
if(CLANG_TIDY_EXE)
    if(ENABLE_CLANG_TIDY_ON_BUILD)
        set_target_properties(ECSExperiments PROPERTIES
            CXX_CLANG_TIDY "${CLANG_TIDY_EXE};-quiet;--extra-arg=/EHsc;--extra-arg=-Wno-everything;--extra-arg=-Wno-error=unused-function;--extra-arg=-Wno-error=unused-command-line-argument"
        )
    endif()

    add_custom_target(tidy
        COMMAND ${CLANG_TIDY_EXE}
            -quiet
            -p "${CMAKE_BINARY_DIR}"
            --extra-arg=-w
            --extra-arg=-Wno-everything
            --extra-arg=-Wno-error=unused-function
            --extra-arg=-Wno-error=unused-command-line-argument
            ${SOURCES}
        COMMENT "Running clang-tidy"
        WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    )
endif()

