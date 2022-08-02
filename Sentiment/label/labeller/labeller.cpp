// header
#include "labeller.hpp"

namespace sentiment::processing
{
	void Labeller::parse_binance()
	{
		parser_t csv_parse;
		std::ifstream file;
		std::string line;

		for (const auto& asset_entry : std::filesystem::directory_iterator(binance))
		{
			const auto asset = asset_entry.path().relative_path().filename();

			m_klines[asset.string()].reserve(std::distance(
				std::filesystem::directory_iterator(asset_entry.path()),
				std::filesystem::directory_iterator())
				* 31 * 6); // num_files * approximateley one month

			for (const auto& entry : std::filesystem::directory_iterator(asset_entry.path()))
			{
				file.open(entry.path());

				while (std::getline(file, line))
					boost::spirit::qi::phrase_parse(std::begin(line), std::end(line), csv_parse, boost::spirit::qi::blank, m_klines[asset.string()]);

				if (m_date_iters[asset.string()].empty())
					m_date_iters[asset.string()].push_back(std::begin(m_klines[asset.string()]));

				m_date_iters[asset.string()].push_back(std::end(m_klines[asset.string()]));
				file.close();
			}

			m_date_iters[asset.string()].push_back(std::end(m_klines[asset.string()]));

			m_date_iters[asset.string()].shrink_to_fit();
		}
	}

	auto Labeller::get_iter(std::time_t timestamp, const std::string& asset) const
	{
		/*auto label_time			= std::chrono::system_clock::from_time_t(timestamp).time_since_epoch();
		auto first_kline_time	= std::chrono::system_clock::from_time_t(m_klines.at(asset)[0].time_open / 1000).time_since_epoch();

		auto start_year = std::chrono::duration_cast <std::chrono::years>(first_kline_time);
		auto start_month = std::chrono::duration_cast <std::chrono::months>(first_kline_time);
		auto year		= std::chrono::duration_cast <std::chrono::years>(label_time);
		auto month		= std::chrono::duration_cast <std::chrono::months>(label_time);

		if ((year.count() - start_year.count()) * 12 + (month.count() - start_month.count()) % 12 + 1 >= std::size(m_date_iters.at(asset)))
		{
			return m_date_iters.at(asset).back();
		}

		std::cout << (year.count() - start_year.count()) * 12 + (month.count() - start_month.count()) % 12  << ", " << start_month.count() << std::endl;
		auto begin	= m_date_iters.at(asset)[(year.count() - start_year.count()) * 12 + (month.count() - start_month.count()) % 12];
		auto end	= m_date_iters.at(asset)[(year.count() - start_year.count()) * 12 + (month.count() - start_month.count()) % 12 + 1];*/

		auto iter = std::find_if(std::begin(m_klines.at(asset)), std::end(m_klines.at(asset)), [timestamp](const auto& kline)
			{ return (timestamp >= kline.time_open / 1000) && (timestamp <= kline.time_close / 1000); });

		return iter;
	}

	int Labeller::mark(const kline_t& kline_start, const kline_t& kline_end, const double eps) const
	{
		auto difference = kline_end.price_close - kline_start.price_close;
		
		if (std::abs(difference) / kline_start.price_close < eps)
		{
			return 0;
		}
		
		return mark(kline_start, kline_end);
	}

	int Labeller::mark(const kline_t& kline_start, const kline_t& kline_end) const
	{
		auto difference = kline_end.price_close - kline_start.price_close;

		if (difference < 0)
			return -1;

		return 1;
	}

	void Labeller::operator()(pkg_sample_t& packages, const std::vector < data::item::Asset >& assets, const int n, const double eps) const
	{
		for (auto& package : packages)
		{
			for (auto& asset : assets)
			{
				auto kline_iter = get_iter(package.m_timestamp, asset.m_name);

				if (kline_iter != m_date_iters.at(asset.m_name).back() && std::next(kline_iter, n) < std::cend(m_klines.at(asset.m_name)))
				{
					package.m_binary += mark(*kline_iter, *std::next(kline_iter, n));
					package.m_ternary += mark(*kline_iter, *std::next(kline_iter, n), eps);
				}
				else
				{
					package.m_timestamp = 0;
				}
			}

			package.m_binary = package.m_binary >= 0 ? 1 : -1;
			package.m_ternary = std::abs(package.m_ternary) < std::size(assets) / 2 ? 0 :
				package.m_ternary > 0 ? 1 : -1;
		}

		packages.erase(std::remove_if(std::begin(packages), std::end(packages), [](const auto& package)
			{ return package.m_timestamp == 0; }), std::end(packages));
	}
}