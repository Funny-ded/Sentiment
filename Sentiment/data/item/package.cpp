#include "package.hpp"

namespace sentiment::data::item
{
	int Package::timeinfo(const char* const format) const noexcept
	{
		auto point = std::chrono::system_clock::time_point(std::chrono::seconds(m_timestamp));

		time_t curr_time = std::chrono::system_clock::to_time_t(point);
		tm timeinfo;
		gmtime_s(&timeinfo, &curr_time);

		std::stringstream time_stream;

		time_stream << std::put_time(&timeinfo, format);

		int formatted;

		time_stream >> formatted;

		return formatted;
	}

	void Package::initialize(const std::uintmax_t start, const std::vector < Message >& sample, 
		const std::vector<FTR_Message>& ftr_sample, const std::vector<Asset>& assets)
	{
		for (auto i = 0U; i < std::size(ftr_sample); ++i)
		{
			if (sample[ftr_sample[i].message_ID].timestamp >= start && sample[ftr_sample[i].message_ID].timestamp < m_timestamp)
			{
				auto filtered_text = ftr_sample[i].from_tokens();

				for (const auto asset : assets)
				{
					if (asset.mentioned(filtered_text))
					{
						m_messages.push_back(i);
						break;
					}
				}
				
			}
		}
	}

	void from_json(const nlohmann::json& obj, Package& message)
	{
		message.m_timestamp = obj["timestamp"].get< std::uintmax_t >();

		message.m_messages = obj["messages"].get< std::vector < std::uintmax_t > >();
		message.m_source = obj["source"].get< std::string >();

		message.m_binary = obj["binary"].get< int >();
		message.m_ternary = obj["ternary"].get< int >();
	}
}

