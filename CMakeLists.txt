# =====================================================================
# milkOpticsMaterials - Optical Material Dispersion and Refractive Index
# =====================================================================

cmake_minimum_required(VERSION 3.16)

set(LIBNAME "milkOpticsMaterials")
set(SRCNAME "OpticsMaterials")

project(lib_${LIBNAME}_project C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(SOURCEFILES
    OpticsMaterials.c
    optmat_solids.c
    optmat_gases.c
    optmat_pmgi.c
    optmat_pmma.c
)

set(INCLUDEFILES
    OpticsMaterials.h
)

add_library(${LIBNAME} SHARED ${SOURCEFILES})

set_target_properties(${LIBNAME} PROPERTIES
    LINKER_LANGUAGE C
    INTERPROCEDURAL_OPTIMIZATION OFF
)

if(TARGET CLIcore)
    # In-tree build under milk framework
    target_include_directories(${LIBNAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
        $<INSTALL_INTERFACE:include>
        $<INSTALL_INTERFACE:include/${SRCNAME}>
    )
    target_link_libraries(${LIBNAME} PUBLIC
        CLIcore
        m
    )
else()
    # Out-of-tree / standalone build
    set(MILK_SOURCE_CANDIDATES
        ${MILK_SOURCE_DIR}
        $ENV{MILK_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/../milk-framework-dev
        $ENV{MILK_ROOT}
        ${CMAKE_CURRENT_SOURCE_DIR}/../milk
        /usr/local/milk
    )
    set(MILK_SOURCE_FOUND "")
    foreach(cand ${MILK_SOURCE_CANDIDATES})
        if(cand AND EXISTS "${cand}/src/coremods")
            get_filename_component(MILK_SOURCE_FOUND "${cand}" ABSOLUTE)
            break()
        endif()
    endforeach()

    find_path(MILK_INCLUDE_DIR
        NAMES CommandLineInterface/CLIcore.h CLIcore.h
        HINTS
            ${MILK_ROOT}/include
            $ENV{MILK_ROOT}/include
            /usr/local/milk/include
            /usr/local/include
    )

    set(MILK_STANDALONE_INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/..
    )
    if(MILK_SOURCE_FOUND)
        list(APPEND MILK_STANDALONE_INCLUDES
            ${MILK_SOURCE_FOUND}/src
            ${MILK_SOURCE_FOUND}/src/cli
            ${MILK_SOURCE_FOUND}/src/cli/CLIcore
            ${MILK_SOURCE_FOUND}/src/engine
            ${MILK_SOURCE_FOUND}/src/engine/libfps
            ${MILK_SOURCE_FOUND}/src/engine/libmilkcommon
            ${MILK_SOURCE_FOUND}/src/engine/libmilkdata
            ${MILK_SOURCE_FOUND}/src/engine/ImageStreamIO
        )
    endif()
    if(MILK_INCLUDE_DIR)
        list(APPEND MILK_STANDALONE_INCLUDES
            ${MILK_INCLUDE_DIR}
            ${MILK_INCLUDE_DIR}/CommandLineInterface
            ${MILK_INCLUDE_DIR}/CommandLineInterface/CLIcore
            ${MILK_INCLUDE_DIR}/libmilkcommon
            ${MILK_INCLUDE_DIR}/libfps
            ${MILK_INCLUDE_DIR}/libmilkdata
            ${MILK_INCLUDE_DIR}/ImageStreamIO
        )
    endif()

    target_include_directories(${LIBNAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
        $<INSTALL_INTERFACE:include>
        $<INSTALL_INTERFACE:include/${SRCNAME}>
        ${MILK_STANDALONE_INCLUDES}
    )

    find_library(MILK_CLICORE_LIB NAMES CLIcore HINTS
        ${MILK_ROOT}/lib
        $ENV{MILK_ROOT}/lib
        /usr/local/milk/lib
        ${MILK_SOURCE_FOUND}/_build/src/cli/CLIcore
    )
    if(NOT MILK_CLICORE_LIB)
        set(MILK_CLICORE_LIB CLIcore)
    endif()

    target_link_libraries(${LIBNAME} PUBLIC
        ${MILK_CLICORE_LIB}
        m
    )
endif()

install(TARGETS ${LIBNAME} EXPORT milkTargets DESTINATION lib)
install(FILES ${INCLUDEFILES} DESTINATION include/${SRCNAME})
install(FILES ${INCLUDEFILES} DESTINATION include)
