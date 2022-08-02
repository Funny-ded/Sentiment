#pragma once

#include <string>

#include <nlohmann/json.hpp>

#include "ftr_message.hpp"
#include "package.hpp"
#include "token.hpp"

namespace sentiment::data::item
{
	struct Wordset
	{
		Wordset() = default;
		Wordset(const std::vector < FTR_Message >& ftr_sample, const Package& package) :
			timestamp(package.m_timestamp), binary(package.m_binary), ternary(package.m_ternary),
			source(package.m_source), tokens(), num_words(0)
		{
			initialize(ftr_sample, package);
		}

		std::uintmax_t timestamp;

		std::string source;

		std::unordered_map < std::string, Token > tokens;

		int binary;
		int ternary;
		int num_words;

		int year() const noexcept
		{
			return timeinfo("%Y");
		}

		int month() const noexcept
		{
			return timeinfo("%m");
		}

		int day() const noexcept
		{
			return timeinfo("%d");
		}

		bool empty() const noexcept
		{
			return num_words == 0;
		}

	private:
		int timeinfo(const char* const) const noexcept;

		void initialize(const std::vector < FTR_Message >&, const Package&);
	};

	inline void to_json(nlohmann::json& obj, const Wordset& wordset)
	{
		obj = {
			{"tokens",		wordset.tokens},
			{"timestamp",	wordset.timestamp},
			{"source",		wordset.source},
			{"binary",		wordset.binary},
			{"ternary",		wordset.ternary},
			{"num_words",	wordset.num_words},
		};
	}

	void from_json(const nlohmann::json&, Wordset&);
}