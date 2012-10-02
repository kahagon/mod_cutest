#!/bin/bash

usage_exit() {
    echo "Usage: modcutest-config [--cflags | --libs]";
    exit 1;
}

if [ $# -ne 1 ]; then
    usage_exit
fi

case $1 in
    --cflags)
        echo "-I%MODCUTEST_INCLUDEDIR%"
        ;;
    --libs)
        echo "-L%MODCUTEST_LIBEXECDIR% -lmodcutest"
        ;;
    *)
        usage_exit
        ;;
esac