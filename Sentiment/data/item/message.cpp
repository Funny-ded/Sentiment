#include "message.hpp"

namespace sentiment::data::item
{
	void from_json(const nlohmann::json& obj, Message& message)
	{
		message.timestamp	= obj["timestamp"].get< std::uintmax_t >();

		message.text		= obj["text"].get< std::string >();
		message.source		= obj["source"].get< std::string >();
		message.source_ID	= obj["source_ID"].get< std::string >();

		message.binary	= obj["binary"].get< int >();
		message.ternary = obj["ternary"].get< int >();
	}
}

