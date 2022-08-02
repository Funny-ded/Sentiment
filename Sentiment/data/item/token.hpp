#pragma once

#include <string>

#include <nlohmann/json.hpp>

namespace sentiment::data::item
{
	struct Token
	{
		Token() = default;
		Token(const std::string& name) : m_name(name), m_part(0.0), m_count(0) {}

		std::string m_name;

		double m_part;
		int m_count;
	};

	inline void to_json(nlohmann::json& obj, const Token& token)
	{
		obj = {
			{"name",	token.m_name},
			{"part",	token.m_part},
			{"count",	token.m_count},
		};
	}

	void from_json(const nlohmann::json&, Token&);
}