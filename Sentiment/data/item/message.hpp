#pragma once

#include <string>

#include <nlohmann/json.hpp>

namespace sentiment
{
	namespace data
	{
		namespace item
		{
			struct Message
			{
				Message() = default;

				std::uintmax_t timestamp;

				std::string text;
				std::string source;
				std::string source_ID;

				int binary;
				int ternary;
			};

			inline void to_json(nlohmann::json& obj, const Message& message)
			{
				obj = {
					{"source_ID",		message.source_ID},
					{"source",			message.source},
					{"text",			message.text},
					{"timestamp",		message.timestamp},
					{"binary",			message.binary},
					{"ternary",			message.ternary},
				};
			}

			void from_json(const nlohmann::json&, Message&);
		}
	}
}