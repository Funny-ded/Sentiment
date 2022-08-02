#include "wordset.hpp"

namespace sentiment::data::item
{
	int Wordset::timeinfo(const char* const format) const noexcept
	{
		auto point = std::chrono::system_clock::time_point(std::chrono::seconds(timestamp));

		time_t curr_time = std::chrono::system_clock::to_time_t(point);
		tm timeinfo;
		gmtime_s(&timeinfo, &curr_time);

		std::stringstream time_stream;

		time_stream << std::put_time(&timeinfo, format);

		int formatted;

		time_stream >> formatted;

		return formatted;
	}

	void Wordset::initialize(const std::vector<FTR_Message>& ftr_sample, const Package& package)
	{
		for (const auto& i : package.m_messages)
		{
			std::vector < std::string > words;

			for (const auto& sentence : ftr_sample[i].words)
			{
				for (const auto& word : sentence)
				{
					++num_words;

					if (!tokens.contains(word.lemma))
					{
						tokens.emplace(word.lemma, word.lemma);
					}

					++tokens[word.lemma].m_count;
					++tokens[word.lemma].m_part;
				}
			}
		}

		for (auto& [name, token] : tokens)
		{
			token.m_part /= num_words;
		}
	}

	void from_json(const nlohmann::json& obj, Wordset& wordset)
	{
		wordset.tokens = obj["tokens"].get < std::unordered_map < std::string, Token > >();
		wordset.timestamp = obj["timestamp"].get < std::uintmax_t >();
		wordset.source = obj["source"].get < std::string >();
		wordset.binary = obj["binary"].get < int >();
		wordset.ternary = obj["ternary"].get < int >();
		wordset.num_words = obj["num_words"].get < int >();
	}
}

