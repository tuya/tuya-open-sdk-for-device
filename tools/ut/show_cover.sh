#!/usr/bin/env bash
##
# @file ut/show_cover.sh
# @brief 
# @author Tuya
# @version 1.0.0
# @date 2023-04-13
#/

# $1: HTML_PATH


if [ ! -f $1 ]; then
    echo "[UT] Don't found [$1]"
    exit 0
fi

python3 -m webbrowser $1

