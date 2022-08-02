#pragma once

#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#include "word.hpp"

namespace sentiment
{
	namespace data
	{
		namespace item
		{
			struct FTR_Message
			{
				FTR_Message() = default;

				std::string source;
				std::string source_ID;

				std::uintmax_t message_ID;

				std::vector < std::vector < Word > > words;

				std::string from_tokens() const;

			};

			inline void to_json(nlohmann::json& obj, const FTR_Message& message)
			{
				obj = {
					{"source_ID",	message.source_ID},
					{"source",		message.source},
					{"message_ID",	message.message_ID},
					{"words",		message.words},
				};
			}

			void from_json(const nlohmann::json&, FTR_Message&);
		}
	}
}