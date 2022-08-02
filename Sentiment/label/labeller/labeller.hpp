#pragma once

#include <filesystem>
#include <fstream>
#include <unordered_map>
#include <vector>

// cls
#include "../binance/klines.hpp"
#include "../../data/item/asset.hpp"
#include "../../data/item/package.hpp"

namespace sentiment::processing
{
	class Labeller
	{
	private:
		using kline_t			= solution::system::detail::Kline;
		using parser_t			= solution::system::detail::Kline_Parser <std::string::const_iterator>;
		using pkg_sample_t 		= std::vector < data::item::Package >;
		using path_t			= std::filesystem::path;

	public:
		Labeller() : m_klines(), m_date_iters()
		{
			parse_binance();
		}

	private:

		void parse_binance();

		auto get_iter(std::time_t, const std::string&) const;

		int mark(const kline_t&, const kline_t&, const double) const;
		int mark(const kline_t&, const kline_t&) const;

	public:

		void operator()(pkg_sample_t&, const std::vector < data::item::Asset >&, const int, const double) const;

	private:
		inline static const path_t binance = "data/binance";

	private:
		std::unordered_map < std::string, std::vector < kline_t > >									m_klines;
		std::unordered_map < std::string, std::vector < std::vector < kline_t >::iterator > >	m_date_iters;
	};
}
