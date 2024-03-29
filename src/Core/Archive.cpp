// Copyright (c) 2012-2018, The CryptoNote developers, The Bytecoin developers.
// Licensed under the GNU Lesser General Public License. See LICENSE for details.

#include "Archive.hpp"
#include <boost/lexical_cast.hpp>
#include <iostream>
#include "CryptoNoteTools.hpp"
#include "Currency.hpp"
#include "common/StringTools.hpp"
#include "common/Varint.hpp"
#include "crypto/crypto.hpp"
#include "platform/Time.hpp"
#include "seria/BinaryInputStream.hpp"
#include "seria/BinaryOutputStream.hpp"

using namespace bytecoin;
using namespace platform;

static const std::string RECORDS_PREFIX = "r";
static const std::string HASHES_PREFIX  = "h";

const std::string Archive::BLOCK("b");
const std::string Archive::TRANSACTION("t");
const std::string Archive::CHECKPOINT("c");

//static const float DB_COMMIT_PERIOD = 60;  // 1 minute sounds good for archive

Archive::Archive(bool read_only, const std::string &path)
 : read_only(read_only)
// , commit_timer(std::bind(&Archive::db_commit, this))
 {
    #if !platform_USE_SQLITE
    try {
		m_db = std::make_unique<DB>(read_only, path);
		if (!m_db->get("$unique_id", unique_id)) {
			DB::Cursor cur = m_db->begin(std::string());
            if (!cur.end())
				throw std::runtime_error("Archive database format unknown version, please delete " + m_db->get_path());
			unique_id = common::pod_to_hex(crypto::random_keypair().public_key);
			m_db->put("$unique_id", unique_id, true);
			std::cout << "Created archive with unique id: " << unique_id << std::endl;
		}
		DB::Cursor cur2 = m_db->rbegin(RECORDS_PREFIX);
		next_record_id  = cur2.end() ? 0 : 1 + common::read_varint_sqlite4(cur2.get_suffix());
	} catch (const std::exception &) {
		if (read_only)
			m_db = nullptr;
		else
			throw;
	}
    #endif
//    commit_timer.once(DB_COMMIT_PERIOD);
}

// struct Record {
//	std::string type;
//	BinaryArray data;
//	Timestamp timestamp;
//};

void Archive::add(const std::string &type,
    const BinaryArray &data,
    const Hash &hash,
    const std::string &source_address) {
	if (!m_db || read_only || source_address.empty())
		return;
	std::cout << "Adding to archive: " << type << " hash=" << hash << " size=" << data.size()
	          << " source_address=" << source_address << std::endl;
	auto hash_key = HASHES_PREFIX + DB::to_binary_key(hash.data, sizeof(hash.data));
	DB::Value value;
	if (!m_db->get(hash_key, value))
		m_db->put(hash_key, data, true);
	api::bytecoind::GetArchive::ArchiveRecord rec;
	rec.timestamp      = now_unix_timestamp(&rec.timestamp_usec);
	rec.type           = type;
	rec.hash           = hash;
	rec.source_address = source_address;
	m_db->put(RECORDS_PREFIX + common::write_varint_sqlite4(next_record_id), seria::to_binary(rec), true);
	next_record_id += 1;
}

void Archive::db_commit() {
	if (!m_db || read_only)
		return;
	m_db->commit_db_txn();
//	commit_timer.once(DB_COMMIT_PERIOD);
}

void Archive::read_archive(api::bytecoind::GetArchive::Request &&req, api::bytecoind::GetArchive::Response &resp) {
	if (req.archive_id != unique_id) {
		api::bytecoind::GetArchive::Error err;
		err.code       = api::bytecoind::GetArchive::WRONG_ARCHIVE_ID;
		err.message    = "Archive id changed";
		err.archive_id = unique_id;
		throw err;
	}
	resp.from_record = req.from_record;
	if (resp.from_record > next_record_id)
		resp.from_record = next_record_id;
	if (req.max_count > api::bytecoind::GetArchive::Request::MAX_COUNT)
		req.max_count = api::bytecoind::GetArchive::Request::MAX_COUNT;
	if (!m_db)
		return;
	resp.records.reserve(static_cast<size_t>(req.max_count));
	for (DB::Cursor cur = m_db->begin(RECORDS_PREFIX, common::write_varint_sqlite4(resp.from_record)); !cur.end();
	     cur.next()) {
		if (resp.records.size() >= req.max_count)
			break;
		api::bytecoind::GetArchive::ArchiveRecord rec;
		seria::from_binary(rec, cur.get_value_array());
		resp.records.push_back(rec);
        if(req.records_only)
                continue;
		std::string str_hash = common::pod_to_hex(rec.hash);
		const auto hash_key  = HASHES_PREFIX + DB::to_binary_key(rec.hash.data, sizeof(rec.hash.data));
		if (rec.type == BLOCK) {
			if (resp.blocks.count(str_hash) == 0) {
				BinaryArray data;
				invariant(m_db->get(hash_key, data), "");
				api::bytecoind::GetArchive::ArchiveBlock &bl = resp.blocks[str_hash];
				RawBlock raw_block;
				seria::from_binary(raw_block, data);
				Block block;
				invariant(block.from_raw_block(raw_block), "");
				bl.raw_header = block.header;
				bl.raw_transactions.reserve(block.transactions.size());
				for (size_t i = 0; i != block.transactions.size(); ++i) {
					bl.raw_transactions.push_back(static_cast<TransactionPrefix &>(block.transactions.at(i)));
					bl.transaction_binary_sizes.push_back(static_cast<uint32_t>(raw_block.transactions.at(i).size()));
				}
				bl.base_transaction_hash = get_transaction_hash(block.header.base_transaction);
			}
		}
		if (rec.type == TRANSACTION) {
			if (resp.transactions.count(str_hash) == 0) {
				BinaryArray data;
				invariant(m_db->get(hash_key, data), "");
				TransactionPrefix &tr = resp.transactions[str_hash];
				Transaction transaction;
				seria::from_binary(transaction, data);
				tr = static_cast<TransactionPrefix &>(transaction);
			}
		}
		if (rec.type == CHECKPOINT) {
			if (resp.checkpoints.count(str_hash) == 0) {
				BinaryArray data;
				invariant(m_db->get(hash_key, data), "");
				SignedCheckPoint &ch = resp.checkpoints[str_hash];
				seria::from_binary(ch, data);
			}
		}
	}
}
