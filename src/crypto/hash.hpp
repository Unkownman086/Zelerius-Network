// Copyright (c) 2012-2018, The CryptoNote developers, The Bytecoin developers.
// Licensed under the GNU Lesser General Public License. See LICENSE for details.

#pragma once

#include <stddef.h>

#include "hash-ops.h"
#include "types.hpp"

namespace crypto {
// Cryptonight hash functions

inline Hash cn_fast_hash(const void *data, size_t length) {
	Hash h;
	cn_fast_hash(data, length, h.data);
	return h;
}
//	inline Hash cn_fast_hash(const std::vector<uint8_t> & data) {
//		return cn_fast_hash(data.data(), data.size());
//	}

class CryptoNightContext {
public:
	CryptoNightContext();
	~CryptoNightContext();

	CryptoNightContext(const CryptoNightContext &) = delete;
	void operator=(const CryptoNightContext &) = delete;

    /*inline void cn_slow_hash(const void *src_data, size_t length, unsigned char *hash) {
        crypto::cn_slow_hash(data, src_data, length, hash, 0);
    }*/

    inline void cn_slow_hash(const void *data, std::size_t length, Hash &hash, int variant = 0) {
      crypto::cn_slow_hash(data, length, reinterpret_cast<unsigned char *>(&hash), variant, 0/*prehashed*/);
    }

    inline Hash cn_slow_hash(const void *src_data, size_t length, int variant = 0) {
		Hash hash;
        crypto::cn_slow_hash(src_data, length, reinterpret_cast<unsigned char *>(&hash), variant, 0/*prehashed*/);
		return hash;
    }

    /*inline void cn_slow_hash_v1(const void *src_data, size_t length, unsigned char *hash) {
        crypto::cn_slow_hash(data, src_data, length, hash, 1);
    }
    inline Hash cn_slow_hash_v1(const void *src_data, size_t length) {
        Hash hash;
        crypto::cn_slow_hash(data, src_data, length, hash.data, 1);
        return hash;
    }*/

private:
    void *data;
};

inline Hash tree_hash(const Hash *hashes, size_t count) {
	Hash root_hash;
	tree_hash(reinterpret_cast<const unsigned char(*)[HASH_SIZE]>(hashes), count, root_hash.data);
	return root_hash;
}

inline void tree_branch(const Hash *hashes, size_t count, Hash *branch) {
	tree_branch(reinterpret_cast<const unsigned char(*)[HASH_SIZE]>(hashes), count,
	    reinterpret_cast<unsigned char(*)[HASH_SIZE]>(branch));
}

inline Hash tree_hash_from_branch(const Hash *branch, size_t depth, const Hash &leaf, const void *path) {
	Hash root_hash;
	tree_hash_from_branch(
	    reinterpret_cast<const unsigned char(*)[HASH_SIZE]>(branch), depth, leaf.data, path, root_hash.data);
	return root_hash;
}
}
