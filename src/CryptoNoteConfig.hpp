// Copyright (c) 2012-2018, The CryptoNote developers, The Bytecoin developers.
// Licensed under the GNU Lesser General Public License. See LICENSE for details.

#pragma once

#include <cstddef>
#include <cstdint>
#include <limits>

#include "p2p/P2pProtocolTypes.hpp"

// This approach allows unlimited customization through config file/command line parameters
// Never include this header into other headers
namespace bytecoin {
namespace parameters {

const uint32_t CRYPTONOTE_MAX_BLOCK_NUMBER             = 500000000;
const uint32_t CRYPTONOTE_MAX_BLOCK_BLOB_SIZE          = 500000000;
const uint32_t CRYPTONOTE_MAX_TX_SIZE                  = 1000000000;
const uint64_t CRYPTONOTE_PUBLIC_ADDRESS_BASE58_PREFIX = 9153; //ZL
const uint32_t CRYPTONOTE_MINED_MONEY_UNLOCK_WINDOW    = 10;

// DIFICULTY
const uint32_t DIFFICULTY_TARGET = 30;// seconds
const uint32_t BLOCKCHAIN_TIMESTAMP_CHECK_WINDOW = 11;
const uint32_t DIFFICULTY_WINDOW_LWMA2 = 60;
const uint32_t DIFFICULTY_WINDOW_LWMA4 = DIFFICULTY_WINDOW_LWMA2;
const uint32_t DIFFICULTY_WINDOW_LWMA3 = DIFFICULTY_WINDOW_LWMA2;
const uint32_t CRYPTONOTE_BLOCK_FUTURE_TIME_LIMIT = 6 * DIFFICULTY_TARGET;
const uint64_t DIFFICULTY_LIMIT = 1000;// Min diff

// MONEY_SUPPLY & EMISSION
const uint64_t MONEY_SUPPLY          =  ((uint64_t)(-1));
const unsigned EMISSION_SPEED_FACTOR = 21;
const uint64_t FIXED_REWARD = static_cast<uint64_t>(350000000000); //0.35 ZLS
static_assert(EMISSION_SPEED_FACTOR <= 8 * sizeof(uint64_t), "Bad EMISSION_SPEED_FACTOR");

//BLOCK SIZE
const size_t CRYPTONOTE_REWARD_BLOCKS_WINDOW = 100;
const size_t CRYPTONOTE_BLOCK_GRANTED_FULL_REWARD_ZONE = 500000;
const size_t CRYPTONOTE_BLOCK_GRANTED_FULL_REWARD_ZONE_V2 = 20000;
const size_t CRYPTONOTE_BLOCK_GRANTED_FULL_REWARD_ZONE_V1 = 10000;
const size_t CRYPTONOTE_BLOCK_GRANTED_FULL_REWARD_ZONE_V5 = 100000;  // size of block (bytes) after which reward for block calculated using block size

constexpr uint32_t EXPECTED_NUMBER_OF_BLOCKS_PER_DAY(uint32_t difficulty_target) {
	return 24 * 60 * 60 / difficulty_target;
}

const size_t MAX_BLOCK_SIZE_INITIAL                  = 100000;
const size_t MAX_BLOCK_SIZE_INITIAL_V5               = MAX_BLOCK_SIZE_INITIAL;

const uint64_t MAX_BLOCK_SIZE_GROWTH_SPEED_NUMERATOR = 300000;
const uint64_t MAX_BLOCK_SIZE_GROWTH_SPEED_NUMERATOR_V5 = 100 * 1024;

constexpr uint64_t MAX_BLOCK_SIZE_GROWTH_SPEED_DENOMINATOR(uint32_t difficulty_target) {
	return 365 * 24 * 60 * 60 / difficulty_target;
}

const size_t CRYPTONOTE_COINBASE_BLOB_RESERVED_SIZE = 600;
const size_t CRYPTONOTE_DISPLAY_DECIMAL_POINT       = 12;

//FEE & DUST
const uint64_t MINIMUM_FEE                          = 1000000;  // pow(10, 6)
const uint64_t DEFAULT_DUST_THRESHOLD               = 1000000;  // pow(10, 6)
const uint64_t SELF_DUST_THRESHOLD                  = 1000;     // pow(10, 3)

// After next hardfork remove settings below
const uint32_t CRYPTONOTE_LOCKED_TX_ALLOWED_DELTA_BLOCKS = 1;
constexpr uint32_t CRYPTONOTE_LOCKED_TX_ALLOWED_DELTA_SECONDS(uint32_t difficulty_target) {
	return difficulty_target * CRYPTONOTE_LOCKED_TX_ALLOWED_DELTA_BLOCKS;
}

const uint32_t CRYPTONOTE_MEMPOOL_TX_LIVETIME = 60 * 60 * 24;  // seconds, one day

//==================================MAIN NET================================================
const uint32_t UPGRADE_HEIGHT_V2 = 1;
const uint32_t UPGRADE_HEIGHT_V3 = 2;
const uint32_t UPGRADE_HEIGHT_V4 = 3; //CN Variant 1
const uint32_t UPGRADE_HEIGHT_V5 = 64000; //Reduces max block size ( ~ 06/oct/2018 )
const uint32_t UPGRADE_HEIGHT_V6 = 265000; //CN Variant 2 ( ~ 15/dec/2018 )
const uint32_t UPGRADE_HEIGHT_V7 = 534800; // CN Variant zls + LWMA3 ( ~ 22/mar/2019 - 23/mar/2019 )
const uint32_t UPGRADE_HEIGHT_V8 = static_cast<uint32_t>(-1); // Future hard fork

//==================================TEST NET================================================
const uint32_t UPGRADE_HEIGHT_V2_TEST = 1;
const uint32_t UPGRADE_HEIGHT_V3_TEST = 2;
const uint32_t UPGRADE_HEIGHT_V4_TEST = 3; // CN Variant 1
const uint32_t UPGRADE_HEIGHT_V5_TEST = 4; // Reduces max block size
const uint32_t UPGRADE_HEIGHT_V6_TEST = 5; // CN Variant 2
const uint32_t UPGRADE_HEIGHT_V7_TEST = 6; // CN Variant zls + LWMA3
const uint32_t UPGRADE_HEIGHT_V8_TEST = 7; // Future hard fork

const uint8_t V1 = 1; //V1
const uint8_t V2 = 2; //UPGRADE_HEIGHT_V2
const uint8_t V3 = 3; //UPGRADE_HEIGHT_V3
const uint8_t V4 = 4; //UPGRADE_HEIGHT_V4
const uint8_t V5 = 5; //UPGRADE_HEIGHT_V5
const uint8_t V6 = 6; //UPGRADE_HEIGHT_V6
const uint8_t V7 = 7; //UPGRADE_HEIGHT_V7
const uint8_t V8 = 8; //UPGRADE_HEIGHT_V8
//==========================================================================================

const char CRYPTONOTE_BLOCKS_FILENAME[]       = "blocks.bin";
const char CRYPTONOTE_BLOCKINDEXES_FILENAME[] = "blockindexes.bin";
}  // parameters

const char CRYPTONOTE_NAME[] = "zelerius";

const uint8_t CURRENT_TRANSACTION_VERSION = 1;

const size_t BLOCKS_IDS_SYNCHRONIZING_DEFAULT_COUNT = 10000;  // by default, blocks ids count in synchronizing
const size_t BLOCKS_SYNCHRONIZING_DEFAULT_COUNT     = 100;    // by default, blocks count in blocks downloading
const size_t COMMAND_RPC_GET_BLOCKS_FAST_MAX_COUNT  = 1000;

const int P2P_DEFAULT_PORT        = 48080;
const int RPC_DEFAULT_PORT        = 48081;
const int WALLET_RPC_DEFAULT_PORT = 48082;

const size_t P2P_LOCAL_WHITE_PEERLIST_LIMIT = 1000;
const size_t P2P_LOCAL_GRAY_PEERLIST_LIMIT  = 5000;

const size_t P2P_CONNECTION_MAX_WRITE_BUFFER_SIZE        = 32 * 1024 * 1024;  // 32 Mb
const uint32_t P2P_DEFAULT_CONNECTIONS_COUNT             = 8;
const uint32_t P2P_DEFAULT_WHITELIST_CONNECTIONS_PERCENT = 70;
const uint32_t P2P_DEFAULT_HANDSHAKE_INTERVAL            = 60;        // seconds
const uint32_t P2P_DEFAULT_PACKET_MAX_SIZE               = 50000000;  // 50000000 bytes maximum packet size
const uint32_t P2P_DEFAULT_PEERS_IN_HANDSHAKE            = 250;
const uint32_t P2P_DEFAULT_CONNECTION_TIMEOUT            = 5000;           // 5 seconds
const uint32_t P2P_DEFAULT_PING_CONNECTION_TIMEOUT       = 2000;           // 2 seconds
const uint32_t P2P_DEFAULT_INVOKE_TIMEOUT                = 60 * 2 * 1000;  // 2 minutes
const uint32_t P2P_DEFAULT_HANDSHAKE_INVOKE_TIMEOUT      = 5000;           // 5 seconds
const char P2P_STAT_TRUSTED_PUBLIC_KEY[] = "b5a7e6891ced4744641db6b2b36d3e2d8dafd185b4d357acf05d7a2142dde4bc";

const char *const CHECKPOINT_PUBLIC_KEYS[] = {"0"};
const char *const CHECKPOINT_PUBLIC_KEYS_TESTNET[] = {"0"};

const char *const SEED_NODES[] = {
    "82.223.50.28:58080", "82.223.22.92:48080", "82.223.50.28:48000", "82.223.22.92:58080"};

struct CheckpointData {
	uint32_t height;
	const char *hash;
};

constexpr const CheckpointData CHECKPOINTS[] = {
    {1, "78dcb4b8c901206d5a1871684feadc36021500ee040c0106219a5c89e47c82db"},
    {2,"48c675eaf143898f6931180678bf96564150676b940b3add93d4c17756e9e086"},
    {3,"df487e48d713eb542219de9f22d883374a586e5f0990f962eb760944bcb49dc8"},
    {4,"d8e4b50bc10c9d9d7653a8f8be6d9eca8002d8afa1a6ea98c89e3ffc04d50a27"},
    {5,"aed9def8c13daa8db58409050f0301c4ba481727f02eb40b27044d6930db35f2"},
    {10000,"9bbfd234a099d821459a0803f0780968a1bedc8bf236a3bb0047bb4c87ad962e"},
    {19001,"534a1c4899c1f3154927beef315aea4a9c4de1a80def68f12f1ac07b1cf84188"},
    {45740,"6b1be5e528a5d2e5c5bec56ac0bbc46312d8ef4e6f1b036298310323c1f3ef5f"},
    {60000,"93845b0dc886776a66054d61c0a59369d8c5b3da521bcd3ecd4fd3aeec36e5f4"},
    {100000,"d179abfbb7f8969c31bbf7c195b3c7a0aa2b0c8b0377342023f476b7d7e2f610"},
    {150000,"70f0016d4514f8b29ca321e435c9656302722a8a75ebc2c44a1c453927c641ff"},
    {180000,"8903dfd69bb134367812fffa94830f5aceda968415c46fdcfd3194b509569aac"},
    {185678,"3253160316c2b58a53cd20437a24af0827b8577af63abcb5baa502f4a4be5622"},
    {200000,"3611cbbef1ae4df1481f0e25fa4585136296d2b7a70df03dd1210b143a648dee"},
    {300000,"ce4eb13655c60fa3408bb17fd8509f1d8c48ccb4c8b207025dbe8c0983f84366"},
    {350000,"e19ffc9e7ee462e549632e6fa59781515fb62a4c9001a69c5d030382459a55d8"},
    {400000,"c5b92d949e1d886c598dfe491f0b5e039ba69bf4d3e1ac843b3a9c9804159bd5"},
    {420000,"6877e215c47568eef1c1e509990f822296fe17324e9f70a718f2c0c8bfd1f22b"},
    {517400,"6bd87688c12af8248bfb6fca4d450adb9c341ea86bf7fcfd0aaeaa46b6d093f2"}
};

const char GENESIS_COINBASE_TX_HEX[] =
        "010a01ff0001ffffffffffff01029b2e4c0281c0b02e7c53291a94d1d0cbff8883f8024f5142ee494ffbbd08807121017b1b0f3aa9a4a821b32c34291678ea36b8d1d601661a494c3a6cde36d7cdedc2";

const char GENESIS_COINBASE_TX_HEX_TEST[] =
        "010a01ff0001ffffffffffff01029b2e4c0281c0b02e7c53291a94d1d0cbff8883f8024f5142ee494ffbbd08807121017b1b0f3aa9a4a821b32c34291678ea36b8d1d601661a494c3a6cde36d7cdedc2";

// ZELERIUSvxxxxxx1
const UUID BYTECOIN_NETWORK{{0x5a, 0x45, 0x4c, 0x45, 0x52, 0x49, 0x55, 0x53, 0x76, 0x78, 0x78, 0x78, 0x78, 0x78, 0x78, 0x31}};

// ZELERIUStestvxx1
const UUID BYTECOIN_NETWORK_TEST{{0x5a, 0x45, 0x4c, 0x45, 0x52, 0x49, 0x55, 0x53, 0x74, 0x65, 0x73, 0x74, 0x76, 0x78, 0x78, 0x31}};

}  // CryptoNote
