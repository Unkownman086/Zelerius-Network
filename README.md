# Zelerius

## About

Welcome to the repository of zelerius. Here you will find source code, instructions, wiki resources, and integration tutorials.

Contents
* Building on Linux 64-bit
* Building on Mac OSX
* Building on Windows
* Building on other platforms

## IMPORTANT

**Development branch** is used for active development and can be either unstable or incompatible with release software so, **DO NOT USE IT!**

## Building on Linux 64-bit

All commands below are adapted for Ubuntu, other distributions may need an other command set.

### Building with standard options

Create directory `zelerius` somewhere and go there:
```
$> mkdir zelerius
$> cd zelerius
```

To go futher you have to have a number of packages and utilities. You need at least gcc 5.4.

* `build-essential` package:
    ```
    $zelerius> sudo apt-get install build-essential
    ```

* CMake (3.5 or newer):
    ```
    $zelerius> sudo apt-get install cmake
    $zelerius> cmake --version
    ```
    If version is too old, follow instructions on [the official site](https://cmake.org/download/).

* Boost (1.62 or newer):
    You need boost in `zelerius` folder. We do not configure to use boost installed by `apt-get`, because it is sometimes updated without your control by installing some unrelated packages. Also some users reported crashes after `find_package` finds headers from one version of boost and libraries from different version, or if installed boost uses dynamic linking.
    ```
    $zelerius> wget -c 'http://sourceforge.net/projects/boost/files/boost/1.67.0/boost_1_67_0.tar.bz2/download'
    $zelerius> tar xf download
    $zelerius> rm download
    $zelerius> mv boost_1_67_0 boost
    $zelerius> cd boost
    $zelerius/boost> ./bootstrap.sh
    $zelerius/boost> ./b2 link=static -j 8 --build-dir=build64 --stagedir=stage
    cd ..
    ```

* OpenSSL (1.1.1 or newer):
    Install OpenSSL to `zelerius/openssl` folder. (In below commands use switch `linux-x86_64-clang` instead of `linux-x86_64` if using clang.)
    ```
    $zelerius> git clone https://github.com/openssl/openssl.git
    $zelerius> cd openssl
    $zelerius/openssl> ./Configure linux-x86_64 no-shared
    $zelerius/openssl> time make -j4
    $zelerius/openssl> cd ..
    ```

Git-clone (or git-pull) zelerius source code in that folder:
```
$zelerius> git clone https://github.com/zelerius/Zelerius-Network
```

Put LMDB source code in `zelerius` folder (source files are referenced via relative paths, so you do not need to separately build it):
```
$zelerius> git clone https://github.com/LMDB/lmdb.git
```

Create build directory inside `Zelerius-Network`, go there and run CMake and Make:
```
$zelerius> cd Zelerius-Network
$zelerius/Zelerius-Network> mkdir build
$zelerius/Zelerius-Network> cd build
$zelerius/zelerius/Zelerius-Network/build> cmake ..
$zelerius/zelerius/Zelerius-Network/build> time make -j4 (4 is an example, it is the number of CPU threads)
```

Check built binaries by running them from `../bin` folder
```
$zelerius/zelerius/Zelerius-Network/build> ../bin/zeleriusd -v
```

### Building with specific options

Download amalgamated [SQLite 3](https://www.sqlite.org/download.html) and unpack it into `zelerius/sqlite` folder (source files are referenced via relative paths, so you do not need to separately build it).

Below are the commands which remove OpenSSL support and switch from LMDB to SQLite by providing options to CMake:

```
$zelerius/Zelerius-Network> mkdir Zelerius-Network/build
$zelerius/Zelerius-Network> cd Zelerius-Network/build
$zelerius/Zelerius-Network/build> cmake -DUSE_SSL=0 -DUSE_SQLITE=1 ..
$zelerius/Zelerius-Network/build> time make -j4 (4 is an example, it is the number of CPU threads)
```

## Building on Mac OSX

### Building with standard options (10.13 or newer)

You need command-line tools. Either get XCode from an App Store or run 'xcode-select --install' in terminal and follow instructions. First of all, you need [Homebrew](https://brew.sh).

Then open terminal and install CMake and Boost:

* `brew install cmake`
* `brew install boost`

Create directory `zelerius` somewhere and go there:
```
$~/Downloads> mkdir zelerius
$~/Downloads> cd zelerius
```

Git-clone (or git-pull) zelerius source code in that folder:
```
$zelerius> git clone https://github.com/zelerius/Zelerius-Network
```

Put LMDB source code in `zelerius` folder (source files are referenced via relative paths, so you do not need to separately build it):
```
$zelerius> git clone https://github.com/LMDB/lmdb.git
```

Create build directory inside zelerius, go there and run CMake and Make:
```
$zelerius> cd Zelerius-Network
$zelerius/Zelerius-Network> mkdir Zelerius-Network/build
$zelerius/Zelerius-Network> cd Zelerius-Network/build
$zelerius/Zelerius-Network/build> cmake -DUSE_SSL=0 ..
$zelerius/Zelerius-Network/build> time make -j4 (4 is an example, it is the number of CPU threads)
```

Check built binaries by running them from `../bin` folder:
```
$zelerius/Zelerius-Network/build> ../bin/zeleriusd -v
```

### Building with specific options

Binaries linked with Boost installed by Homebrew will work only on your computer's OS X version or newer, but not on older versions like 10.13.

If you need binaries to run on all versions of OS X starting from 10.13, you need to build boost yourself targeting 10.13.

Download [Mac OSX 10.13 SDK](https://github.com/phracker/MacOSX-SDKs/releases) and unpack to it into `Downloads` folder

Download and unpack [Boost](https://boost.org) to `Downloads` folder.

Then build and install Boost:
```
$~> cd ~/Downloads/boost_1_67_0/
$~/Downloads/boost_1_67_0> ./bootstrap.sh
$~/Downloads/boost_1_67_0> ./b2 -a -j 4 cxxflags="-stdlib=libc++ -std=c++14 -mmacosx-version-min=10.11 -isysroot/Users/user/Downloads/MacOSX10.11.sdk" install`
```

Install OpenSSL to `zelerius/openssl` folder:
```
$~/Downloads/zelerius> git clone https://github.com/openssl/openssl.git
$~/Downloads/zelerius> cd openssl
```

If you need binaries to run on all versions of OS X starting from 10.13, you need to build OpenSSL targeting 10.13.
```
$zelerius/openssl> ./Configure darwin64-x86_64-cc no-shared -mmacosx-version-min=10.13 -isysroot/Users/user/Downloads/MacOSX10.13.sdk
```
Otherwise just use
```
$zelerius/openssl> ./Configure darwin64-x86_64-cc no-shared
```

```
$zelerius/openssl> time make -j4 (4 is an example, it is the number of CPU threads)
$zelerius/openssl> cd ..
```

Download amalgamated [SQLite 3](https://www.sqlite.org/download.html) and unpack it into `zelerius/sqlite` folder (source files are referenced via relative paths, so you do not need to separately build it).

You add OpenSSL support or switch from LMDB to SQLite by providing options to CMake:

```
$zelerius> cd Zelerius-Network
$zelerius/Zelerius-Network> mkdir Zelerius-Network/build
$zelerius/Zelerius-Network> cd Zelerius-Network/build
$zelerius/Zelerius-Network/build> cmake -DUSE_SSL=1 -DUSE_SQLITE=1 ..
$zelerius/Zelerius-Network/build> time make -j4 (4 is an example, it is the number of CPU threads)
```

## Building on Windows

You need Microsoft Visual Studio Community 2017. [Download](https://www.visualstudio.com/vs/) and install it selecting `C++`, `git`, `cmake integration` packages.
Run `Visual Studio x64 command prompt` from start menu.

Create directory `zelerius` somewhere:
```
$C:\> mkdir zelerius
$C:\> cd zelerius
```

Get [Boost](https://boost.org) and unpack it into a folder inside `zelerius` and rename it from `boost_1_66_0` or similar to just `boost`.

Build boost (build 32-bit boost version only if you need 32-bit zelerius binaries).
```
$> cd boost
$C:\zelerius\boost> bootstrap.bat
$C:\zelerius\boost> b2.exe address-model=64 link=static -j 8 --build-dir=build64 --stagedir=stage
$C:\zelerius\boost> b2.exe address-model=64 link=static -j 8 --build-dir=build64 --stagedir=stage runtime-link=static (if you want build static version)
$C:\zelerius\boost> b2.exe address-model=32 link=static -j 8 --build-dir=build32 --stagedir=stage32
cd ..
```

Git-clone (or git-pull) zelerius source code in that folder:
```
$C:\zelerius> git clone https://github.com/zelerius/Zelerius-Network
```

Put LMDB in the same folder (source files are referenced via relative paths, so you do not need to separately build it):
```
$C:\zelerius> git clone https://github.com/LMDB/lmdb.git
```

You need to build openssl, first install ActivePerl (select "add to PATH" option, then restart console):
```
$C:\zelerius> git clone https://github.com/openssl/openssl.git
$C:\zelerius> cd openssl
$C:\zelerius\openssl> perl Configure VC-WIN64A no-shared no-asm
$C:\zelerius\openssl> nmake
$C:\zelerius\openssl> cd ..
```
If you want to build 32-bit binaries, you will also need 32-bit build of openssl in separate folder (configuring openssl changes header files, so there is no way to have both 32-bit and 64-bit versions in the same folder):
```
$C:\zelerius> git clone https://github.com/openssl/openssl.git openssl32
$C:\zelerius> cd openssl32
$C:\zelerius\openssl> perl Configure VC-WIN32 no-shared no-asm
$C:\zelerius\openssl> nmake
$C:\zelerius\openssl> cd ..
```

Now launch Visual Studio, in File menu select `Open Folder`, select `C:\zelerius\Zelerius-Network` folder.
Wait until CMake finishes running and `Build` appears in main menu.
Select `x64-Debug` or `x64-Release` from standard toolbar, and then `Build/Build Solution` from the main menu.

You cannot add options to CMake running inside Visual Studio so just edit `CMakeLists.txt` and set `USE_SSL` or `USE_SQLITE` to `ON` if you wish to build with them.

## Building on 32-bit x86 platforms, iOS, Android and other ARM platforms

zelerius works on 32-bit systems if SQLite is used instead of LMDB (we've experienced lots of problems building and running with lmdb in 32-bit compatibility mode, especially on iOS).

Therefore SQLite option is automatically selected by CMake on 32-bit platforms and you must have SQLite downloaded as explained in appropriate sections above.

We build official x86 32-bit version for Windows only, because there is zero demand for 32-bit version for Linux or Mac.

Building source code for iOS, Android, Raspberry PI, etc is possible (we have experimental `zeleriusd` and `walletd` running on ARM64 iPhone) but requires major skills on your part. __TBD__

## Building on Big-Endian platforms

Currently it is impossible to run zelerius on any Big-Endian platform, due to lots of endianess-dependent code. This may be fixed in the future. If you wish to run on Big-Endian platform, please contact us.

## Building with parameters

If you want to use tools like `clang-tidy`, run `cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..` instead of `cmake ..`
