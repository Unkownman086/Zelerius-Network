// Copyright (c) 2012-2018, The CryptoNote developers, The Bytecoin developers.
// Licensed under the GNU Lesser General Public License. See LICENSE for details.

#include "http/JsonRpc.hpp"

namespace bytecoin {

namespace json_rpc {

// throw it directly, without error_code or other complications
Error::Error() : code(0) {}

Error::Error(int c) : code(c), message(get_message(c)) {}
std::string Error::get_message(int code) {
	switch (code) {
	case PARSE_ERROR:
		return "Parse error";
	case INVALID_REQUEST:
		return "Invalid request";
	case METHOD_NOT_FOUND:
		return "Method not found";
	case INVALID_PARAMS:
		return "Invalid params";
	case INTERNAL_ERROR:
		return "Internal error";
	}
	return "Unknown error";
}

Error::Error(int c, const std::string &msg) : code(c), message(msg) {}

void Error::seria_data(seria::ISeria &s) {
    s.begin_object();
    seria_data_members(s);
    s.end_object();
}
void Error::seria_data_members(seria::ISeria &s) {}

void make_generic_error_reponse(common::JsonValue &resp, const std::string &what, int error_code) {
	common::JsonValue error(common::JsonValue::OBJECT);

	std::string msg = !what.empty() ? what : Error::get_message(error_code);

	error.insert("code", common::JsonValue::Integer(error_code));
	error.insert("message", msg);

	resp.insert("error", error);
}
}
}
