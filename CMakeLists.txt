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
    target_include_directories(${LIBNAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/src/coremods
        ${CMAKE_SOURCE_DIR}/plugins
        ${CMAKE_SOURCE_DIR}/plugins/milk-extra-src
        $<TARGET_PROPERTY:CLIcore,INTERFACE_INCLUDE_DIRECTORIES>
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
            ${MILK_SOURCE_FOUND}/src/engine/libprocessinfo
            ${MILK_SOURCE_FOUND}/src/engine/libmilkdata
            ${MILK_SOURCE_FOUND}/src/engine/libcomputeunit
            ${MILK_SOURCE_FOUND}/src/engine/ImageStreamIO
        )
        if(EXISTS "${MILK_SOURCE_FOUND}/_build/config")
            list(APPEND MILK_STANDALONE_INCLUDES "${MILK_SOURCE_FOUND}/_build/config")
        endif()
    endif()
    if(MILK_INCLUDE_DIR)
        list(APPEND MILK_STANDALONE_INCLUDES
            ${MILK_INCLUDE_DIR}
            ${MILK_INCLUDE_DIR}/CommandLineInterface
            ${MILK_INCLUDE_DIR}/CommandLineInterface/CLIcore
            ${MILK_INCLUDE_DIR}/libmilkcommon
            ${MILK_INCLUDE_DIR}/libfps
            ${MILK_INCLUDE_DIR}/libprocessinfo
            ${MILK_INCLUDE_DIR}/libmilkdata
            ${MILK_INCLUDE_DIR}/libcomputeunit
            ${MILK_INCLUDE_DIR}/ImageStreamIO
        )
    endif()

    target_compile_definitions(${LIBNAME} PRIVATE MILK_NO_CLI)

    target_include_directories(${LIBNAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
        $<INSTALL_INTERFACE:include>
        $<INSTALL_INTERFACE:include/${SRCNAME}>
        ${MILK_STANDALONE_INCLUDES}
    )

    set(MILK_LIB_NAMES
        milkcomputeunit
        milkdata
        milkcommon
    )
    set(MILK_SEARCH_HINTS
        ${MILK_ROOT}/lib
        $ENV{MILK_ROOT}/lib
        ${CMAKE_INSTALL_PREFIX}/lib
        /usr/local/milk/lib
        /usr/local/lib
    )
    if(MILK_SOURCE_FOUND)
        list(APPEND MILK_SEARCH_HINTS ${MILK_SOURCE_FOUND}/_build/lib)
    endif()

    set(MILK_LIBRARIES "")
    foreach(lib ${MILK_LIB_NAMES})
        find_library(MILK_${lib}_LIB NAMES ${lib} HINTS ${MILK_SEARCH_HINTS})
        if(MILK_${lib}_LIB)
            list(APPEND MILK_LIBRARIES ${MILK_${lib}_LIB})
        else()
            list(APPEND MILK_LIBRARIES ${lib})
        endif()
    endforeach()

    target_link_libraries(${LIBNAME} PUBLIC
        ${MILK_LIBRARIES}
        m
    )
endif()

install(TARGETS ${LIBNAME} EXPORT milkTargets DESTINATION lib)
install(FILES ${INCLUDEFILES} DESTINATION include/${SRCNAME})
install(FILES ${INCLUDEFILES} DESTINATION include)

install(CODE "
    set(_milk_cli \"\${CMAKE_INSTALL_PREFIX}/bin/milk-cli\")
    if(EXISTS \"\${_milk_cli}\")
        execute_process(COMMAND \"\${_milk_cli}\" --catalog RESULT_VARIABLE _cat_rc)
    endif()
")
