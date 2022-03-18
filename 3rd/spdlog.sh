#!/usr/bin/mysql sh
if [ "$CROSS_PLATFORM_LIB_ROOTDIR" == "" ] 
then
	export CROSS_PLATFORM_LIB_OUTDIR="/opt/code/c++/work/Mylib/cross_platform/out"
else
    export CROSS_PLATFORM_LIB_OUTDIR="$CROSS_PLATFORM_LIB_ROOTDIR"/out
fi

if [ "$1" == "clean" ]
then
    rm -rf $CROSS_PLATFORM_LIB_OUTDIR/include/3rd/spdlog

else
    mkdir -p $CROSS_PLATFORM_LIB_OUTDIR/include
    mkdir -p $CROSS_PLATFORM_LIB_OUTDIR/include/3rd
    mkdir -p $CROSS_PLATFORM_LIB_OUTDIR/include/3rd/spdlog

    cd spdlog
    
#    cp -p -rf ./include/*.* $NAUT_OUTDIR/include/3rd/spdlog
    cp -p -rf ./include/spdlog $CROSS_PLATFORM_LIB_OUTDIR/include/3rd/spdlog
    
fi


