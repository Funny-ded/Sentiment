#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "message.hpp"
#include "ftr_message.hpp"
#include "asset.hpp"

#include <nlohmann/json.hpp>

namespace sentiment::data::item
{
	struct Package
	{
		Package() = default;

		Package(const std::uintmax_t start, const std::uintmax_t end,
			const std::vector < Message >& sample,
			const std::vector < FTR_Message >& ftr_sample, 
			const std::vector < Asset >& assets, 
			const std::string& source) :
			m_binary(0), m_ternary(0), m_timestamp(end), m_messages(), m_source(source)
		{
			initialize(start, sample, ftr_sample, assets);
		}

		int m_binary;
		int m_ternary;

		std::uintmax_t m_timestamp;
		std::vector < std::uintmax_t > m_messages;

		std::string m_source;

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
			return m_messages.empty();
		}

	private:
		int timeinfo(const char* const) const noexcept;

		void initialize(const std::uintmax_t, 
			const std::vector < Message >&,
			const std::vector < FTR_Message >&, 
			const std::vector < Asset >&);
		
	};

	inline void to_json(nlohmann::json& obj, const Package& pkg_message)
	{
		obj = {
			{"source",			pkg_message.m_source},
			{"messages",		pkg_message.m_messages},
			{"timestamp",		pkg_message.m_timestamp},
			{"binary",			pkg_message.m_binary},
			{"ternary",			pkg_message.m_ternary},
		};
	}

	void from_json(const nlohmann::json&, Package&);
}
