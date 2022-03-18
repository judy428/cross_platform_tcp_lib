#!/usr/bin/mysql sh
if [ "$CROSS_PLATFORM_LIB_ROOTDIR" == "" ] 
then
	export CROSS_PLATFORM_LIB_OUTDIR="/opt/code/c++/work/Mylib/cross_platform/out"
else
    export CROSS_PLATFORM_LIB_OUTDIR="$CROSS_PLATFORM_LIB_ROOTDIR"/out
fi

if [ "$1" == "clean" ]
then
    rm -rf $CROSS_PLATFORM_LIB_OUTDIR/include/3rd/boost_1_77_0
    rm -rf $CROSS_PLATFORM_LIB_OUTDIR/lib/debug/libboost_*
    rm -rf $CROSS_PLATFORM_LIB_OUTDIR/lib/release/libboost_*
    

else
    mkdir -p $CROSS_PLATFORM_LIB_OUTDIR/include
    mkdir -p $CROSS_PLATFORM_LIB_OUTDIR/include/3rd
    mkdir -p $CROSS_PLATFORM_LIB_OUTDIR/include/3rd/boost_1_77_0
    mkdir -p $CROSS_PLATFORM_LIB_OUTDIR/lib/debug
    mkdir -p $CROSS_PLATFORM_LIB_OUTDIR/lib/release

    cd boost_1_77_0
    
#    cp -p -rf ./include/*.* $CROSS_PLATFORM_LIB_OUTDIR/include/3rd/hv
    cp -p -rf ./include/* $CROSS_PLATFORM_LIB_OUTDIR/include/3rd/boost_1_77_0
    cp -p ./lib/*.a  $CROSS_PLATFORM_LIB_OUTDIR/lib/debug/
    cp -p ./lib/*.a  $CROSS_PLATFORM_LIB_OUTDIR/lib/release/
    cp -p ./lib/*.so  $CROSS_PLATFORM_LIB_OUTDIR/lib/debug/
    cp -p ./lib/*.so  $CROSS_PLATFORM_LIB_OUTDIR/lib/release/
fi