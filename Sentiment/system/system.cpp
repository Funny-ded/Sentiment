#include "system.hpp"

void sentiment::System::Data::Config::load()
{
	json_t config_object;

	try
	{
		Data::load(File::config_json, config_object);

		m_start_date = config_object[Fields::start_date].get< std::uint64_t >();

		m_excluded_coins = config_object[Fields::excluded_coins].get< std::vector < std::string > >();
		m_sources = config_object[Fields::sources].get< std::vector < std::string > >();

		m_lens = config_object[Fields::package_lens].get < std::vector < int > > ();
	}
	catch (const json_t::exception&)
	{
		throw system_exception("Wrong config file");
	}
	catch (const system_exception& e)
	{
		throw e;
	}
}

void sentiment::System::Data::Config::save() const
{
	json_t config_object;

	config_object[Fields::start_date]		= m_start_date;
	config_object[Fields::sources]			= m_sources;
	config_object[Fields::excluded_coins]	= m_excluded_coins;
	config_object[Fields::package_lens]		= m_lens;

	Data::save(File::config_json, config_object);
}

void sentiment::System::Data::Config::set_start_date(const std::uint64_t new_start_date) noexcept
{
	m_start_date = new_start_date;
}

void sentiment::System::Data::load(sample_t& sample, const std::string& source)
{
	json_t data;

	load(Data::Directory::data_sources / source / Data::File::sample_json, data);

	sample = data.get < sample_t >();
}

void sentiment::System::Data::load(ftr_sample_t& ftr_sample, const std::string& source)
{
	json_t data;

	load(Data::Directory::data_sources / source / File::ftr_sample_json, data);

	ftr_sample = data.get < ftr_sample_t >();
}

void sentiment::System::Data::save(const ftr_sample_t& ftr_sample, const std::string& source)
{
	json_t obj = ftr_sample;

	save(Directory::data_sources / source / File::ftr_sample_json, obj);
}

void sentiment::System::Data::save(const pkg_sample_t& pkg_sample, const std::string& source, const int len)
{
	json_t data = pkg_sample;

	auto month = pkg_sample.back().month();
	std::string month_str;
	if (month < 10)
		month_str = std::format("0{}", month);
	else
		month_str = std::to_string(month);

	auto day = pkg_sample.back().day();
	std::string day_str;
	if (day < 10)
		day_str = std::format("0{}", day);
	else
		day_str = std::to_string(day);

	auto filename = std::format("packages-{}-{}-{}.json", pkg_sample.back().year(), month_str, day_str);

	save(Directory::data_sources / source / Directory::packages / std::format("L{}", len) / filename, data);
}

void sentiment::System::Data::save(const wst_sample_t& wst_sample, const std::string& source, const int len)
{
	json_t data = wst_sample;

	auto month = wst_sample.back().month();
	std::string month_str;
	if (month < 10)
		month_str = std::format("0{}", month);
	else
		month_str = std::to_string(month);

	auto day = wst_sample.back().day();
	std::string day_str;
	if (day < 10)
		day_str = std::format("0{}", day);
	else
		day_str = std::to_string(day);


	auto filename = std::format("wordsets-{}-{}-{}.json", wst_sample.back().year(), month_str, day_str);

	save(Directory::data_sources / source / Directory::wordsets / std::format("L{}", len) / filename, data);
}

void sentiment::System::Data::load_last(pkg_sample_t& pkg_sample, const std::string& source, const int len)
{
	auto path = Directory::data_sources / source / Directory::packages / std::format("L{}", len);

	auto num_files = Data::Directory::size(path);

	auto entity = std::next(std::filesystem::directory_iterator(path), num_files - 1);

	json_t data;

	load(entity->path(), data);

	pkg_sample = data.get < pkg_sample_t >();
}

void sentiment::System::Data::load_last(wst_sample_t& wst_sample, const std::string& source, const int len)
{
	auto path = Directory::data_sources / source / Directory::wordsets / std::format("L{}", len);

	auto num_files = Data::Directory::size(path);

	auto entity = std::next(std::filesystem::directory_iterator(path), num_files - 1);

	json_t data;

	load(entity->path(), data);

	wst_sample = data.get < wst_sample_t >();
}

void sentiment::System::Data::load_tmp(ftr_sample_t& ftr_sample)
{
	json_t data;
		
	load(File::tmp_sample_json, data);
	
	ftr_sample = data.get < ftr_sample_t >();
		
	std::filesystem::remove(File::tmp_sample_json);
}

void sentiment::System::Data::save_tmp(const std::vector < uintmax_t >& to_save, const sample_t& sample)
{
	json_t obj;

	for (const auto& id : to_save)
	{
		obj[std::to_string(id)] = sample[id];
	}
	
	if (!std::filesystem::exists(File::tmp_sample_json))
	{
		std::ofstream create_file(File::tmp_sample_json);
	}
	
	save(File::tmp_sample_json, obj);
}

void sentiment::System::Data::load_assets(std::vector< data::item::Asset >& assets)
{
	std::ifstream fin(File::assets_data);

	if (!fin)
	{
		throw system_exception("Cannot open file " + File::assets_data.string());
	}

	csv::CSV_Parser parser(fin);

	for (const auto& row : parser)
	{
		std::vector < std::string > mentions;

		for (auto i = 1U; i < std::size(row); ++i)
		{
			mentions.push_back(row[i]);
		}

		assets.push_back(data::item::Asset(row[0], mentions));
	}
}

void sentiment::System::Data::load(const path_t& path, json_t& object)
{
	std::fstream fin(path, std::ios::in);

	if (!fin)
	{
		throw system_exception("Cannot open file " + path.string());
	}

	fin >> object;
}

void sentiment::System::Data::save(const path_t& path, const json_t& object)
{
	std::fstream fout(path, std::ios::out);

	if (!fout)
	{
		throw system_exception("Cannot open file " + path.string());
	}

	fout << object;
}

void sentiment::System::initialize()
{
	m_python = std::make_shared < python_t >();
	m_loader = std::make_unique < data::Loader >(m_python);
	m_config.load();
	Data::load_assets(m_assets);

	m_assets.erase(std::remove_if(std::begin(m_assets), std::end(m_assets), [this](const auto& asset)
		{ return std::find(std::begin(m_config.excluded_coins()), std::end(m_config.excluded_coins()), asset.m_name)
			!= std::end(m_config.excluded_coins()); }), std::end(m_assets));

	m_filter = std::make_unique < processing::Filter >(m_python, m_assets);
}

auto sentiment::System::download() const
{
	auto end_date = std::chrono::duration_cast <std::chrono::seconds> (std::chrono::system_clock::now().time_since_epoch()).count();

	for (const auto& source : m_config.sources())
	{
		m_loader->set_source(source);

		m_loader->download(m_config.start_date(), end_date);
	}

	return end_date;
}

void sentiment::System::filter_sample(std::vector < std::uintmax_t >& to_filter, const sample_t& sample, const ftr_sample_t& ftr_sample) const
{
	for (auto id = 0U; id < std::size(sample); ++id)
	{
		if (!ftr_sample.empty() && std::find_if(std::begin(ftr_sample), std::end(ftr_sample),
			[id](const auto& ftr_item) { return ftr_item.message_ID == id; }) != std::end(ftr_sample))
		{
			// messages in sample are in ascending order. If something has been processed it means that all messages before has been processed too. 
			// So we can clear to_filter vector 
			if (!to_filter.empty())
			{
				to_filter.clear();
			}

			continue;
		}

		for (const auto& asset : m_assets)
		{
			if (asset.mentioned(sample[id].text))
			{
				to_filter.push_back(id);
				break;
			}
		}
	}
}

void sentiment::System::run()
{
	for (;;)
	{
		try
		{
			m_config.set_start_date(download());

			m_config.save();

			for (const auto& source : m_config.sources())
			{
				std::cout << "Start processing " << source << std::endl;

				sample_t sample;

				Data::load(sample, source);

				std::cout << "Sample loaded" << std::endl;

				ftr_sample_t ftr_sample;

				Data::load(ftr_sample, source);

				std::cout << "Filtered sample loaded" << std::endl;

				std::vector < std::uintmax_t > to_filter;

				filter_sample(to_filter, sample, ftr_sample);

				if (!to_filter.empty())
				{
					std::cout << "Fitration started" << std::endl;

					m_filter->preprocess(to_filter, sample);

					std::cout << "Start processing stanza" << std::endl;

					Data::save_tmp(to_filter, sample);
					m_filter->stanza_process();

					ftr_sample_t processed;
					Data::load_tmp(processed);

					m_filter->postprocess(processed);

					for (const auto& ftr_message : processed)
					{
						ftr_sample.push_back(ftr_message);
					}

					std::sort(std::begin(ftr_sample), std::end(ftr_sample), [](const auto& msg_1, const auto& msg_2)
						{ return msg_1.message_ID < msg_2.message_ID; });

					Data::save(ftr_sample, source);
				}

				for (const auto len_package : m_config.lens())
				{
					std::uintmax_t package_start = m_default_start;

					pkg_sample_t packages;
					wst_sample_t wordsets;

					if (!Data::no_packages(source, len_package))
					{
						Data::load_last(packages, source, len_package);

						package_start = packages.back().m_timestamp - m_timeout.count() * (len_package - 1);
					}

					if (!Data::no_wordsets(source, len_package))
					{
						Data::load_last(wordsets, source, len_package);
					}

					for (std::uintmax_t package_end = package_start + m_timeout.count() * len_package;
						package_end < m_config.start_date();
						package_end += m_timeout.count(), package_start += m_timeout.count())
					{
						auto package = data::item::Package(package_start, package_end, sample, ftr_sample, m_assets, source);
						auto wordset = data::item::Wordset(ftr_sample, package);

						if (!packages.empty() && package.day() != packages.back().day())
						{
							Data::save(packages, source, len_package);
							Data::save(wordsets, source, len_package);

							packages.clear();
							wordsets.clear();
						}

						if (!package.empty())
						{
							packages.push_back(package);
						}

						if (!wordset.empty())
						{
							wordsets.push_back(wordset);
						}
					}

					if (!packages.empty())
					{
						Data::save(packages, source, len_package);
					}

					if (!wordsets.empty())
					{
						Data::save(wordsets, source, len_package);
					}
				}
			}
			
			auto wake_time = std::chrono::time_point < std::chrono::system_clock, std::chrono::seconds >(
				std::chrono::seconds(m_timeout.count() + m_config.start_date())
				);

			std::cout << "Falling asleep" << std::endl;
			std::this_thread::sleep_until(wake_time);
		}
		catch (const system_exception& e)
		{
			throw e;
		}
	}
}
