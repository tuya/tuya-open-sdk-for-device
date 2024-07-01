#!/usr/bin/env bash
##
# @file kconfiglib/choice_pre_config.sh
# @brief 
# @author Tuya
# @version 1.0.0
# @date 2023-04-12
#/

# $1: PRE_CONFIG_DIR
# $2: DOT_CONFIG_PATH
# *$3: CONFIG_CMAKE

cd `dirname $0`

LISTMENU="../listmenu.sh"

PRE_CONFIG_NAME=$(ls -p $1 | grep -E "^[^/]*\.config$" | bash $LISTMENU "Pre-config" 20 60 13)
cp ${1}/${PRE_CONFIG_NAME} ${2}

echo "[KCONFIG] Choice pre-config [${PRE_CONFIG_NAME}]"

if [ x"$3" == x"" ];then
    exit 0
fi

NEW_LINE="set(PRE_CONFIG_NAME \"${PRE_CONFIG_NAME}\")"
sed -i "s/^set(PRE_CONFIG_NAME .*/${NEW_LINE}/g" $3
echo "[KCONFIG] Change config-cmake  [${3}]"
