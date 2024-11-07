#!/usr/bin/sh
# 参数说明：
# $1 - example name: echo_app_top
# $2 - example version: 1.0.0
# $3 - header files directory:
# $4 - libs directory:
# $5 - libs: tuyaos tuyaapp
# $6 - output directory:

print_not_null()
{
    if [ x"$1" = x"" ]; then
        return 1
    fi

    echo "$1"
}


set -e
cd `dirname $0`

EXAMPLE_NAME=$1         # the example name selected when ./configure
EXAMPLE_VER=$2          # the version, default is 1.0.0
HEADER_DIR=$3           # the include file directory, default is build/include
LIBS_DIR=$4             # the libraries file directory, default is build/lib
LIBS=$5                 # the libraries file, default is libtuyaapp & libtuyaos
OUTPUT_DIR=$6           # the output file directory, default is build/bin
USER_CMD=$7             # the user compile command

echo EXAMPLE_NAME=$EXAMPLE_NAME
echo EXAMPLE_VER=$EXAMPLE_VER
echo HEADER_DIR=$HEADER_DIR
echo LIBS_DIR=$LIBS_DIR
echo LIBS=$LIBS
echo OUTPUT_DIR=$OUTPUT_DIR
echo USER_CMD=$USER_CMD

if [ "$USER_CMD" = "build" ]; then
    USER_CMD=all
fi

# make EXAMPLE_NAME=$EXAMPLE_NAME EXAMPLE_VER=$EXAMPLE_VER LIBS_DIR=$LIBS_DIR LIBS="$LIBS" OUTPUT_DIR=$OUTPUT_DIR HEADER_DIR="$HEADER_DIR" $USER_CMD
echo add your compile cmds here!
echo add your compile cmds here!
echo add your compile cmds here!

