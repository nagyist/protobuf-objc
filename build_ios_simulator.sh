#!/bin/bash
unset ARCH ARCH_PREFIX PLATFORM SDKVER DEVROOT SDKROOT PKG_CONFIG_PATH AS ASCPP AR RANLIB CPP CXXCPP CC CXX LD STRIP CPPFLAGS CFLAGS CXXFLAGS LDFLAGS
export ARCH=i686-apple-darwin11
export ARCH_PREFIX=${ARCH}-
export PLATFORM=iPhoneSimulator

export SDKVER=5.1
export DEVROOT=/Applications/Xcode.app/Contents/Developer/Platforms/${PLATFORM}.platform/Developer
export SDKROOT="$DEVROOT/SDKs/${PLATFORM}$SDKVER.sdk"
export PKG_CONFIG_PATH="$SDKROOT/usr/lib/pkgconfig:$DEVROOT/usr/lib/pkgconfig"
export AS="$DEVROOT/usr/bin/as"
export ASCPP="$DEVROOT/usr/bin/as"
export AR="$DEVROOT/usr/bin/ar"
export RANLIB="$DEVROOT/usr/bin/ranlib"
export CPP="$DEVROOT/usr/bin/llvm-cpp-4.2"
export CXXCPP="$DEVROOT/usr/bin/llvm-cpp-4.2"
export CC="$DEVROOT/usr/bin/llvm-gcc"
export CXX="$DEVROOT/usr/bin/llvm-g++"
export LD="$DEVROOT/usr/bin/ld"
export STRIP="$DEVROOT/usr/bin/strip"

export CPPFLAGS="-pipe -no-cpp-precomp -I$SDKROOT/usr/lib/gcc/${ARCH}/4.2.1/include -I$SDKROOT/usr/include -I$DEVROOT/usr/include"
export CFLAGS="-arch i386 -fmessage-length=0 -pipe -fpascal-strings -no-cpp-precomp -miphoneos-version-min=5.0 --sysroot='$SDKROOT' -isystem $SDKROOT/usr/lib/gcc/${ARCH}/4.2.1/include -isystem $SDKROOT/usr/include -isystem $DEVROOT/usr/include"
export CXXFLAGS="$CFLAGS -I$SDKROOT/usr/include/c++/4.2.1/${ARCH}/v6"
export LDFLAGS="--sysroot='$SDKROOT' -L$SDKROOT/usr/lib -L$SDKROOT/usr/lib/system -L$DEVROOT/usr/lib"

./configure --host=${ARCH} --with-protoc=protoc --enable-static --disable-shared --prefix=/tmp/protobuf/simulator
make clean
make
make install