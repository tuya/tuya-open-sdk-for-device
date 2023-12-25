##
# @file CMakeLists.txt
# @brief 
# @author Tuya
# @version 1.0.0
# @date 2023-03-26
#/


cmake_minimum_required(VERSION 3.16 FATAL_ERROR)


# include cmake
set(TOP_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(TOP_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${TOP_BINARY_DIR}/lib)
SET(EXECUTABLE_OUTPUT_PATH ${TOP_BINARY_DIR}/bin)

if(${TOP_SOURCE_DIR} STREQUAL ${TOP_BINARY_DIR})
    message(FATAL_ERROR "
    [In-source builds] are not allowed.
    You should create separate directory for build files.
    Remove \"CMakeCache.txt\" and \"CMakeFiles/\".
    Use commands like:
    [mkdir build; cd build; cmake ..]
    or
    [cmake -S . -B build]
    ")
endif()

include(${TOP_SOURCE_DIR}/tools/cmake/colors.cmake)
include(${TOP_SOURCE_DIR}/tools/cmake/util.cmake)
set(CONFIG_CMAKE "${TOP_SOURCE_DIR}/cache/config.cmake")
include(${CONFIG_CMAKE})


# use Kconfig
set(DOT_CONFIG_DIR "${TOP_BINARY_DIR}/cache")
set(DOT_CONFIG "using.config")
set(CMAKE_CONFIG "using.cmake")
set(HEADER_DIR "${TOP_BINARY_DIR}/include")
set(HEADER_FILE "tuya_kconfig.h")
set(HEADER_IN_PATH "${TOP_SOURCE_DIR}/tools/kconfiglib/config.h.in")
add_subdirectory("${TOP_SOURCE_DIR}/tools/kconfiglib")


# toolchain cmake
set(CMAKE_TOOLCHAIN_FILE "${BOARD_PATH}/toolchain_file.cmake")
message(STATUS "[TOP] Using toolchain file [${CMAKE_TOOLCHAIN_FILE}].")


project(opensdk)

# add board
include("${BOARD_PATH}/board_config.cmake")


# add src
set(COMPONENT_LIBS "")
set(COMPONENT_PUBINC "")
list_components(COMPONENT_LIST "${TOP_SOURCE_DIR}/src")
foreach(comp ${COMPONENT_LIST})
    # message(STATUS "comp: ${comp}")
    add_subdirectory("${TOP_SOURCE_DIR}/src/${comp}")
endforeach(comp)
message(STATUS "[TOP] COMPONENT_LIBS: ${COMPONENT_LIBS}")
# copy_public_include("${COMPONENT_PUBINC}" "${HEADER_DIR}")
list(APPEND HEADER_DIR ${BOARD_PUBINC})
list(APPEND HEADER_DIR ${COMPONENT_PUBINC})
message(STATUS "[TOP] HEADER_DIR: ${HEADER_DIR}")
deal_with_components("${COMPONENT_LIBS}" "${HEADER_DIR}")

# build components_all lib
set(COMPONENTS_ALL_LIB "tuyaos")
set(all_need)
foreach(c ${COMPONENT_LIBS})
    list(APPEND all_need $<TARGET_OBJECTS:${c}>)
endforeach(c)
add_library(${COMPONENTS_ALL_LIB} STATIC ${all_need})


# add example
message(STATUS "[TOP] Using example [${EXAMPLE_NAME}].")
set(EXAMPLE_LIB "tuyaapp")
set(EXAMPLE_VER "1.0.0" CACHE STRING "Example Version")
string(REPLACE ";" " " BOARD_NEED_HDIR "${HEADER_DIR}")
set(BOARD_NEED_LIBS "${EXAMPLE_LIB} ${COMPONENTS_ALL_LIB}")
message(STATUS "[TOP] BOARD_NEED_LIBS: ${BOARD_NEED_LIBS}")
add_subdirectory("${EXAMPLE_DIR}/${EXAMPLE_NAME}")
target_include_directories(${EXAMPLE_LIB}
    PUBLIC
    ${HEADER_DIR}
    )
target_link_libraries(${EXAMPLE_LIB} ${COMPONENTS_ALL_LIB})
add_custom_target(example
    DEPENDS
    show_menuconfig
    ${EXAMPLE_LIB}
    build_example_exe

    COMMENT
    "[TOP] example=[${EXAMPLE_NAME}]"
    )
add_custom_command(
    OUTPUT
    show_menuconfig

    COMMAND
    ${CMAKE_COMMAND} -E echo "        ${Cyan}[make menuconfig]${ColourReset}"

    COMMAND
    ${CMAKE_COMMAND} -E echo "        ${Cyan}[ccmake ..]${ColourReset}"

    COMMAND
    ${CMAKE_COMMAND} -E echo ""

    COMMENT
    "[EXAMPLE] If you want to change the configuration or change example version:"
    )
add_custom_command(
    OUTPUT
    build_example_exe

    WORKING_DIRECTORY
    ${BOARD_PATH}

    COMMAND
    sh build_example.sh "${EXAMPLE_NAME}" "${EXAMPLE_VER}" "${BOARD_NEED_HDIR}"
    "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}" "${BOARD_NEED_LIBS}"
    "${EXECUTABLE_OUTPUT_PATH}" "build"

    COMMENT
    "[EXAMPLE] Generate execute file [${EXAMPLE_NAME}]"
    )

# clean
add_custom_target(board_clean
    WORKING_DIRECTORY
    ${BOARD_PATH}

    COMMAND
    sh build_example.sh "${EXAMPLE_NAME}" "${EXAMPLE_VER}" "${BOARD_NEED_HDIR}"
    "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}" "${BOARD_NEED_LIBS}"
    "${EXECUTABLE_OUTPUT_PATH}" "clean"

    COMMENT
    "[TOP] Board clean."
    )

add_custom_target(clean_all
    DEPENDS
    board_clean

    COMMAND ${CMAKE_BUILD_TOOL} clean
)

# add UT
enable_testing()
add_subdirectory("${TOP_SOURCE_DIR}/tools/ut")


# prompt
message(STATUS "[TOP] If you want to build example
        ${Cyan}[make example]${ColourReset}.
")
