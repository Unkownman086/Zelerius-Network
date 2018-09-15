// Copyright (c) 2012-2018, The CryptoNote developers, The Bytecoin developers.
// Licensed under the GNU Lesser General Public License. See LICENSE for details.

#pragma once

#include <cstddef>
#include <cstdint>
#include <limits>

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
const uint32_t CRYPTONOTE_BLOCK_FUTURE_TIME_LIMIT = 6 * DIFFICULTY_TARGET;
const uint64_t DIFFICULTY_LIMIT = 1000;// Min diff

// MONEY_SUPPLY & EMISSION
const uint64_t MONEY_SUPPLY          =  ((uint64_t)(-1));
const unsigned EMISSION_SPEED_FACTOR = 21;
const uint64_t FIXED_REWARD = static_cast<uint64_t>(350000000000); //0.35 ZLS
static_assert(EMISSION_SPEED_FACTOR <= 8 * sizeof(uint64_t), "Bad EMISSION_SPEED_FACTOR");

//BLOCK SIZE
const size_t CRYPTONOTE_REWARD_BLOCKS_WINDOW = 100;
const size_t CRYPTONOTE_BLOCK_GRANTED_FULL_REWARD_ZONE = 500000;  // size of block (bytes) after which reward for block calculated using block size
const size_t CRYPTONOTE_BLOCK_GRANTED_FULL_REWARD_ZONE_V2 = 20000;
const size_t CRYPTONOTE_BLOCK_GRANTED_FULL_REWARD_ZONE_V1 = 10000;

const size_t CRYPTONOTE_COINBASE_BLOB_RESERVED_SIZE = 600;
const size_t CRYPTONOTE_DISPLAY_DECIMAL_POINT       = 12;

//FEE & DUST
const uint64_t MINIMUM_FEE                          = 1000000;  // pow(10, 6)
const uint64_t DEFAULT_DUST_THRESHOLD               = 1000000;  // pow(10, 6)


constexpr uint32_t EXPECTED_NUMBER_OF_BLOCKS_PER_DAY(uint32_t difficulty_target) {
	return 24 * 60 * 60 / difficulty_target;
}

const size_t MAX_BLOCK_SIZE_INITIAL                  = 100000;//20 * 1024;
const uint64_t MAX_BLOCK_SIZE_GROWTH_SPEED_NUMERATOR = 300000;//100 * 1024;
constexpr uint64_t MAX_BLOCK_SIZE_GROWTH_SPEED_DENOMINATOR(uint32_t difficulty_target) {
	return 365 * 24 * 60 * 60 / difficulty_target;
}

// After next hardfork remove settings below
const uint32_t CRYPTONOTE_LOCKED_TX_ALLOWED_DELTA_BLOCKS = 1;
constexpr uint32_t CRYPTONOTE_LOCKED_TX_ALLOWED_DELTA_SECONDS(uint32_t difficulty_target) {
	return difficulty_target * CRYPTONOTE_LOCKED_TX_ALLOWED_DELTA_BLOCKS;
}

const uint32_t CRYPTONOTE_MEMPOOL_TX_LIVETIME = 60 * 60 * 24;  // seconds, one day

const uint32_t UPGRADE_HEIGHT_V2 = 1;
const uint32_t UPGRADE_HEIGHT_V3 = 2;
const uint32_t UPGRADE_HEIGHT_V4 = 3;//CN Variant 1

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

const char *const CHECKPOINT_PUBLIC_KEYS[] = {};
const char *const CHECKPOINT_PUBLIC_KEYS_TESTNET[] = {};

const char *const SEED_NODES[] = {
    "82.223.50.28:48080", "82.223.22.92:48080", "82.223.50.28:58080"};

struct CheckpointData {
	uint32_t height;
	const char *hash;
};

constexpr const CheckpointData CHECKPOINTS[] = {
    {0, ""}
};

}  // CryptoNote