#pragma once

#include <format>
#include <regex>
#include <string>
#include <vector>


namespace sentiment::data::item
{
	struct Asset
	{
		Asset(const std::string& name, const std::vector < std::string >& mentions)
			: m_name(name), m_mentions(mentions)
		{}

		std::string m_name;

		std::vector < std::string > m_mentions;

		bool mentioned(const std::string&) const;
	};
}