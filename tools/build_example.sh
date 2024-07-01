#!/usr/bin/env bash
##
# @file build_example.sh
# @brief 在windows环境编译时，gcc生成的.d文件中路径可能存在冒号“:”
#        导致第二次编译报错(makefile中冒号背当作目标)。
#        此脚本在编译前提前生成compiler_depend.internal文件，
#        可以阻止compiler_depend.make的生成
# @author Tuya
# @version 1.0.0
# @date 2024-01-30
#/

BUILD_DIR=$1
FLAG=$2
BUILD_CMD="example_all"

if [ x"$BUILD_DIR" = x"" ]; then
    echo "Error: Need build path."
    exit 1
fi

cd $BUILD_DIR

if [ x"$FLAG" = x"1" ]; then
    DEPEND_PATHS=`find ${BUILD_DIR} -name 'compiler_depend.make'`
    for p in $DEPEND_PATHS;
    do
    echo "" > ${p:0:-5}.internal;
    done
fi

make $BUILD_CMD
