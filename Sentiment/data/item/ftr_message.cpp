#include "ftr_message.hpp"

namespace sentiment::data::item
{
	void from_json(const nlohmann::json& obj, FTR_Message& message)
	{
		message.source		= obj["source"].get < std::string >();
		message.source_ID	= obj["source_ID"].get < std::string >();

		message.message_ID = obj["message_ID"].get < std::uintmax_t >();

		message.words = obj["words"].get < std::vector < std::vector < Word > > >();
	}

	std::string FTR_Message::from_tokens() const
	{
		std::string filtered = "";

		for (const auto& sentence : words)
		{
			for (const auto& word : sentence)
			{
				filtered += word.lemma + std::string(" ");
			}
		}

		return filtered;
	}

}