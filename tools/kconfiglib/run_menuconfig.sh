#!/usr/bin/env bash
##
# @file kconfiglib/run_menuconfig.sh
# @brief 
# @author Tuya
# @version 1.0.0
# @date 2023-04-08
#/

# $1: KCONFIG_CATALOG
# $2: DOT_KCONFIG

KCONFIG_TOOLS=`dirname $0`

export KCONFIG_CONFIG="$2"
python3 -u $KCONFIG_TOOLS/menuconfig.py $1

