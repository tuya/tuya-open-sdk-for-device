#!/usr/bin/env bash
##
# @file ut/git_clone.sh
# @brief 
# @author Tuya
# @version 1.0.0
# @date 2023-03-28
#/

# $1: git repo
# $2: target dir
# *$3: commit/tag/branch

cd `dirname $0`


if [ -d $2 ]; then
    echo "[CLONE] Repo already exists [$2]"
    exit 0
fi

mkdir -p `dirname $2`

git clone "$1" "$2"

if [ "$?" != "0" ]; then
    echo "
    [CLONE] git clone fail [${1}].
    Please check or run the command manually:
    [git clone ${1} ${2}]
    "
fi

if [ x"$3" != x"" ]; then
    cd $2
    git checkout "$3"
fi
