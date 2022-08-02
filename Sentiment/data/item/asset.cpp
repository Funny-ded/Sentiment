#include "asset.hpp"

namespace sentiment::data::item
{
	bool Asset::mentioned(const std::string& text) const
	{
		for (const auto mention : m_mentions)
		{
			if (std::regex_search(text, std::regex(std::format("\\b{}\\b", mention), std::regex_constants::icase)))
				return true;
		}

		return false;
	}
}
