# Copyright (c) 2012-2018, The CryptoNote developers, The Bytecoin developers.
# Licensed under the GNU Lesser General Public License. See LICENSE for details.

cmake_minimum_required(VERSION 3.5)
project(zelerius)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_STANDARD 11)
#set(CMAKE_BUILD_TYPE Debug)
# message(STATUS "Bytecoind profile: According to cmake, sizeof(void *) == " ${CMAKE_SIZEOF_VOID_P})
option(USE_INSTRUMENTATION "For testing - builds with address sanitizer instrument" OFF)
option(WITH_THREAD_SANITIZER "For testing - builds with thread sanitizer instrument, USE_INSTRUMENTATION must be also set" OFF)
option(USE_SSL "Builds with support of https between walletd and zeleriusd" ON)
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    option(USE_SQLITE "Builds with SQLite instead of LMDB. 4x slower, but works on 32-bit and mobile platforms" OFF)
    set(OPENSSL_ROOT ../openssl)
else()
    option(USE_SQLITE "Builds with sqlite instead of LMDB. 4x slower, but works on 32-bit and mobile platforms" ON)
    set(OPENSSL_ROOT ../openssl32)
endif()

if (MSVC)
    set (USE_SQLITE ON)
    set (CMAKE_CXX_FLAGS_MINSIZEREL     "${CMAKE_CXX_FLAGS_MINSIZEREL} /MT")
    set (CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /MTd")
    set (CMAKE_CXX_FLAGS_RELEASE        "${CMAKE_CXX_FLAGS_RELEASE} /MT")
    set (CMAKE_CXX_FLAGS_DEBUG          "${CMAKE_CXX_FLAGS_DEBUG} /MTd")

    set (CMAKE_C_FLAGS_MINSIZEREL       "${CMAKE_C_FLAGS_MINSIZEREL} /MT")
    set (CMAKE_C_FLAGS_RELWITHDEBINFO   "${CMAKE_C_FLAGS_RELWITHDEBINFO} /MTd")
    set (CMAKE_C_FLAGS_RELEASE          "${CMAKE_C_FLAGS_RELEASE} /MT")
    set (CMAKE_C_FLAGS_DEBUG            "${CMAKE_C_FLAGS_DEBUG} /MTd")
    message(STATUS "MSVC usage: ON")
  #else()
  #  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
endif()

if(WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
    add_definitions(-D_SCL_SECURE_NO_WARNINGS=1 -D_CRT_SECURE_NO_WARNINGS=1 -D_WIN32_WINNT=0x0501)
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -maes -g -O2 -Wall -Wextra -Werror=return-type -Wno-unused-parameter")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -maes -g -O2 -Wall -Wextra -Werror=return-type -Wno-unused-parameter")
    message(STATUS "Instrumentation usage: " ${USE_INSTRUMENTATION})
    message(STATUS "Thread sanitizer usage: " ${WITH_THREAD_SANITIZER})
    if(USE_INSTRUMENTATION)
	if(WITH_THREAD_SANITIZER)
	    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=thread")
	    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=thread")
	else()
	    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer -fsanitize=address,undefined")
	    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fno-omit-frame-pointer -fsanitize=address,undefined")
	endif()
    endif()
endif()
if(USE_SQLITE)
    # Requires dl on Linux, we add it unconditionally for simplicity.
    message(STATUS "Database selected: SQLite 3. Make sure it is put into ../sqlite/")
    include_directories(../sqlite)
    set(SRC_WARNINGS_DB ../sqlite/sqlite3.c)
    set(SRC_DB src/platform/DBsqlite3.cpp src/platform/DBsqlite3.hpp)
    add_definitions(-Dplatform_USE_SQLITE=1)
        message(STATUS "SQLITE usage: ON")
else()
    message(STATUS "Database selected: LMDB. Make sure it is put into ../lmdb/")
    include_directories(../lmdb/libraries/liblmdb)
    set(SRC_WARNINGS_DB ../lmdb/libraries/liblmdb/mdb.c ../lmdb/libraries/liblmdb/midl.c)
    set(SRC_DB src/platform/DBlmdb.cpp src/platform/DBlmdb.hpp)
        message(STATUS "SQLITE usage: OFF")
endif()
if(USE_SSL)
    message(STATUS "SSL usage: ON. Make sure openssl headers are in " ${OPENSSL_ROOT} "/include and static libs are in " ${OPENSSL_ROOT} "/")
    include_directories(${OPENSSL_ROOT}/include)
    link_directories(${OPENSSL_ROOT}) # Must be placed before add_executable, add_library.
    set(LINK_OPENSSL ssl crypto)
    add_definitions(-Dplatform_USE_SSL=1)
else()
    message(STATUS "SSL usage: OFF")
endif()
file(GLOB SRC_CRYPTO
    src/crypto/*.cpp src/crypto/*.hpp src/crypto/*.h
    src/crypto/*.c src/crypto/*.h
    src/crypto/bernstein/*.h src/crypto/bernstein/*.c
    src/crypto/blake/*.h src/crypto/blake/*.c
    src/crypto/groestl/*.h src/crypto/groestl/*.c
    src/crypto/jh/*.h src/crypto/jh/*.c
    src/crypto/keccak/*.c
    src/crypto/oaes/*.h src/crypto/oaes/*.c
    src/crypto/skein/*.h src/crypto/skein/*.c
)
file(GLOB SRC_COMMON src/common/*.cpp src/common/*.hpp)
file(GLOB SRC_SERIALIZATION src/Serialization/*.cpp src/Serialization/*.hpp)
file(GLOB SRC_SERIA src/seria/*.cpp src/seria/*.hpp)
file(GLOB SRC_LOGGING src/logging/*.cpp src/logging/*.hpp)
file(GLOB SRC_P2P src/p2p/*.cpp src/p2p/*.hpp)
file(GLOB SRC_CORE src/Core/*.cpp src/Core/*.hpp src/CryptoNote.hpp src/rpc_api.hpp)
file(GLOB SRC_HTTP src/http/*.cpp src/http/*.hpp)
file(GLOB SRC_PLATFORM
    src/platform/ExclusiveLock.cpp src/platform/ExclusiveLock.hpp
    src/platform/Files.cpp src/platform/Files.hpp
    src/platform/Time.cpp src/platform/Time.hpp
    src/platform/Network.cpp src/platform/Network.hpp
    src/platform/PathTools.cpp src/platform/PathTools.hpp
    src/platform/PreventSleep.cpp src/platform/PreventSleep.hpp
    src/platform/Windows.hpp src/platform/DB.hpp
    #src/platform/DBsqlite3.cpp src/platform/DBsqlite3.hpp
)
if(WIN32)
    set_property(SOURCE ${SRC_CRYPTO} PROPERTY COMPILE_FLAGS -Ot)
    set_property(SOURCE ${SRC_DB} PROPERTY COMPILE_FLAGS -Ot)
    set_property(SOURCE ${SRC_WARNINGS_DB} PROPERTY COMPILE_FLAGS "-Ot -w")
    set_property(SOURCE ${SRC_COMMON} PROPERTY COMPILE_FLAGS -Ot)
    set_property(SOURCE ${SRC_SERIALIZATION} PROPERTY COMPILE_FLAGS -Ot)
    set_property(SOURCE ${SRC_SERIA} PROPERTY COMPILE_FLAGS -Ot)
else()
    set_property(SOURCE ${SRC_CRYPTO} PROPERTY COMPILE_FLAGS -O3)
    set_property(SOURCE ${SRC_DB} PROPERTY COMPILE_FLAGS -O3)
    set_property(SOURCE ${SRC_WARNINGS_DB} PROPERTY COMPILE_FLAGS "-O3 -w")
    set_property(SOURCE ${SRC_COMMON} PROPERTY COMPILE_FLAGS -O3)
    set_property(SOURCE ${SRC_SERIALIZATION} PROPERTY COMPILE_FLAGS -O3)
    set_property(SOURCE ${SRC_SERIA} PROPERTY COMPILE_FLAGS -O3)
endif()
include_directories(src)
set(SOURCE_FILES
    ${SRC_DB}
    ${SRC_WARNINGS_DB}
    ${SRC_COMMON}
    ${SRC_HTTP}
    ${SRC_CORE}
    ${SRC_SERIALIZATION}
    ${SRC_SERIA}
    ${SRC_LOGGING}
    ${SRC_PLATFORM}
    ${SRC_P2P}
    src/CryptoNote.hpp
    src/CryptoNoteConfig.hpp
    src/rpc_api.hpp
    src/version.hpp
)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/libs")
if(WIN32)
    include_directories(SYSTEM ../boost)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
	link_directories(SYSTEM ../boost/stage/lib) # must be before add_executable, add_library
    else()
	link_directories(SYSTEM ../boost/stage32/lib) # must be before add_executable, add_library
    endif()
endif()
add_library(zelerius-crypto ${SRC_CRYPTO})
add_library(zelerius-core ${SOURCE_FILES})
target_link_libraries(zelerius-core zelerius-crypto)
if(WIN32)
    add_executable(walletd src/main_walletd.cpp src/bytecoin.rc) # .rc works only if referenced directly in add_executable
    add_executable(zeleriusd src/main_bytecoind.cpp src/bytecoin.rc) # .rc works only if referenced directly in add_executable
else()
    add_executable(walletd src/main_walletd.cpp)
    add_executable(zeleriusd src/main_bytecoind.cpp)
endif()
add_executable(tests src/main_tests.cpp tests/io.hpp tests/Random.hpp
        tests/blockchain/test_blockchain.cpp tests/blockchain/test_blockchain.hpp
	tests/crypto/test_crypto.cpp tests/crypto/test_crypto.hpp
	tests/hash/test_hash.cpp tests/hash/test_hash.hpp
	tests/json/test_json.cpp tests/json/test_json.hpp
	tests/wallet_state/test_wallet_state.cpp tests/wallet_state/test_wallet_state.hpp
	tests/wallet_file/test_wallet_file.cpp tests/wallet_file/test_wallet_file.hpp)
set(Boost_USE_STATIC_LIBS ON)
add_definitions(-DBOOST_BIND_NO_PLACEHOLDERS=1 -DBOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE=1) # boost::_1 conflicts with std::_1
target_link_libraries(walletd zelerius-crypto zelerius-core)
target_link_libraries(zeleriusd zelerius-crypto zelerius-core)
target_link_libraries(tests zelerius-crypto zelerius-core)
if(WIN32)
else()
    set(Boost_USE_MULTITHREADED OFF) # all boost libraries are multithreaded since some version
    if(APPLE)
	set(BOOST_ROOT ../boost)
	find_package(Boost 1.62 REQUIRED COMPONENTS system)
	message(STATUS "Boost_INCLUDE_DIRS: " ${Boost_INCLUDE_DIRS})
	include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
	#set(Boost_LIBRARIES "${Boost_LIBRARIES}")
    else()
	message(STATUS "Boost_INCLUDE_DIRS: ../boost")
	include_directories(SYSTEM ../boost)
	set(Boost_LIBRARIES "${CMAKE_CURRENT_SOURCE_DIR}/../boost/stage/lib/libboost_system.a")
    endif()
    message(STATUS "Boost_LIBRARIES: " ${Boost_LIBRARIES})
    if(APPLE)
	set(CMAKE_OSX_DEPLOYMENT_TARGET "10.14")
	target_link_libraries(walletd "-framework Foundation" "-framework IOKit")
	target_link_libraries(zeleriusd "-framework Foundation" "-framework IOKit")
    endif()
    target_link_libraries(walletd ${Boost_LIBRARIES} ${LINK_OPENSSL} dl pthread)
    target_link_libraries(zeleriusd ${Boost_LIBRARIES} ${LINK_OPENSSL} dl pthread)
    target_link_libraries(tests ${Boost_LIBRARIES} ${LINK_OPENSSL} dl pthread)
endif()
