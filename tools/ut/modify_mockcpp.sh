#!/usr/bin/env bash
##
# @file ut/modify_mockcpp.sh
# @brief 
# @author Tuya
# @version 1.0.0
# @date 2023-03-29
#/

# $1: mockcpp src CMakeLists.txt

cd `dirname $0`

sed -i '10,20s/^SET(MOCKCPP_SRC_ROOT ${CMAKE_SOURCE_DIR})/get_filename_component(MOCKCPP_SRC_ROOT ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)/g' $1

