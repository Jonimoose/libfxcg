# Installer

HOMEBREW=/opt/homebrew/opt
PATH=$PATH:/Applications/CASIO/SDK/bin

if [ ! -d "/opt/homebrew" ]; then
    /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
fi

if [ ! -d "$HOMEBREW/mpfr" ]; then
        brew install mpfr
fi
if [ ! -d "$HOMEBREW/mpc" ]; then
        brew install mpc
fi
if [ ! -d "$HOMEBREW/gmp" ]; then
        brew install gmp
fi
if [ ! -d "$HOMEBREW/isl" ]; then
        brew install isl
fi
if [ ! -d "$HOMEBREW/libmpc" ]; then
        brew install libmpc
fi
if [ ! -d "$HOMEBREW/libpng" ]; then
        brew install libpng
fi
if [ ! -d "$HOMEBREW/ppl" ]; then
        brew install ppl
fi
if [ ! -d "$HOMEBREW/cmake" ]; then
        brew install cmake
fi
if [ ! -d "$HOMEBREW/imagemagick" ]; then
        brew install imagemagick
fi
if [ ! -d "$HOMEBREW/texinfo" ]; then
        brew install texinfo
fi

# For multicore CPU with RAM to spare, to speed up builds use `make -j$(nproc)` instead of `make` for this guide.
# NOTE! for macOS use make -j$(sysctl -n hw.ncpu)

# Compiling binutils
if [ ! -d "build-binutils" ]; then
    tar -xzvf binutils*.tar.gz
    mkdir build-binutils
fi
if [ "$(ls -A build-binutils)" ]; then
    echo "binutils: Allready Compiled!"
else
    cd build-binutils
    echo "Compiling binutils"
    
    ../binutils-*/./configure --target=sh3eb-elf --prefix=/Applications/CASIO/SDK --disable-nls
    make -j$(sysctl -n hw.ncpu)
    make install
    
    cd ..
fi

# Compiling GCC
if [ ! -d "build-gcc" ]; then
    tar -xzvf gcc*.tar
    mkdir build-gcc
fi

if [ "$(ls -A build-gcc)" ]; then
    echo "gcc: Allready Compiled!"
else
    cd build-gcc
    echo "Compiling gcc"
        
    GMP=$HOMEBREW/gmp
    MPFR=$HOMEBREW/mpfr
    MPC=$HOMEBREW/mcp

    ../gcc-*/./configure --target=sh3eb-elf --prefix=/Applications/CASIO/SDK \
    --with-gmp=$GMP \
    --with-mpfr=$MPFR \
    --with-mpc=/opt/homebrew/Cellar/libmpc/1.3.1 \
    --disable-nls --enable-languages=c,c++ --without-headers

    make -j$(sysctl -n hw.ncpu) all-gcc
    make install-gcc
    make -j$(sysctl -n hw.ncpu) all-target-libgcc
    make install-target-libgcc
    
    cd ..
fi

# Compiling mkg3a
if ! ls -d mkg3a*/ >/dev/null 2>&1; then
    tar -xzvf mkg3a*.tar.gz
fi
if [ -f "/Applications/CASIO/SDK/bin/mkg3a" ]; then
    echo "mkg3a: Allready exists!"
else
    cd mkg3a*
    
    cmake .
    make
    su -c 'make install'
    
    PREFIX := /Applications/CASIO/SDK/bin/sh3eb-elf-
    PREFIX := /usr/bin/sh3eb-elf-
    
    make
    
    cd ..
fi

# fx-CG Library
if ! ls -d libfxcg*/ >/dev/null 2>&1; then
    tar -xzvf libfxcg*.tar
fi
if [ "$(ls -A libfxcg*/lib)" ]; then
    echo "libfxcg: Allready Compiled!"
else
    cd libfxcg*
    
    make -j$(sysctl -n hw.ncpu)

    cp lib/*.a /Applications/CASIO/SDK/lib/
    cp -a toolchain /Applications/CASIO/SDK/
    cp -a include /Applications/CASIO/SDK/
    
    cd ..
fi
    
