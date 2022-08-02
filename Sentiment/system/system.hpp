#pragma once

// std
#include <cinttypes>
#include <cstdint>
#include <chrono>
#include <exception>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <string>

// json
#include <nlohmann/json.hpp>

// python
#include "../python/python.hpp"

// data loader
#include "../data/loader.hpp"

// data item
#include "../data/item/message.hpp"
#include "../data/item/ftr_message.hpp"
#include "../data/item/package.hpp"
#include "../data/item/wordset.hpp"
#include "../data/item/asset.hpp"

// filter
#include "../processing/filter/filter.hpp"

// parser
#include "../csv_parser/csv_parser.hpp"


namespace sentiment
{
	class system_exception : public std::exception
	{
	public:
		explicit system_exception(const std::string& message) noexcept :
			std::exception(message.c_str()) {}

		explicit system_exception(const char* const message) noexcept :
			std::exception(message) {}

		~system_exception() noexcept = default;
	};

	class System
	{
	private:
		using duration_t			= std::chrono::seconds;
		using python_t				= solution::shared::Python;

		using sample_t				= std::vector < data::item::Message >;
		using ftr_sample_t			= std::vector < data::item::FTR_Message >;
		using pkg_sample_t			= std::vector < data::item::Package >;
		using wst_sample_t			= std::vector < data::item::Wordset >;


	public:

		System()
		{
			initialize();
		}

		~System() noexcept
		{
			try
			{
				uninitialize();
			}
			catch (...)
			{
				// ...
			}
		}

	private:

		void initialize();

		void uninitialize()
		{
			m_config.save();
		}

	private:

		class Data
		{
		private:
			using path_t = std::filesystem::path;
			using json_t = nlohmann::json;

		private:
			struct File
			{
				static inline const path_t config_json = "system/config.json";
				static inline const path_t assets_data = "system/assets.data";

				static inline const path_t sample_json		= "sample.json";	
				static inline const path_t ftr_sample_json	= "ftr_sample.json";

				static inline const path_t pkg_sample_json	= "pkg_sample.json";
				static inline const path_t wst_sample_json	= "wst_sample.json";

				static inline const path_t tmp_sample_json = "python/tmp/tmp_sample.json";

			};

			struct Directory
			{
				static inline const path_t python_tmp		= "python/tmp";
				static inline const path_t data_sources		= "data/sources";

				static inline const path_t packages	= "packages";
				static inline const path_t wordsets = "wordsets";

				static int size(const path_t& path)
				{
					return std::distance(std::filesystem::directory_iterator(path), std::filesystem::directory_iterator());
				}

				static bool empty(const path_t& path)
				{
					return size(path) == 0;
				}
			};

		public:
			class Config
			{
			public:
				void load();

				void save() const;

			public:
				auto sources() const noexcept
				{
					return m_sources;
				}

				auto start_date() const noexcept
				{
					return m_start_date;
				}

				auto excluded_coins() const noexcept
				{
					return m_excluded_coins;
				}

				auto lens() const noexcept
				{
					return m_lens;
				}

				void set_start_date(const std::uint64_t) noexcept;

			private:
				struct Fields
				{
					static inline constexpr auto start_date		= "start_date";
					static inline constexpr auto excluded_coins = "excluded_coins";
					static inline constexpr auto sources		= "sources";
					static inline constexpr auto package_lens	= "package_lens";
				};

			private:
				std::uint64_t m_start_date;

				std::vector < std::string > m_excluded_coins;
				std::vector < std::string > m_sources;
				std::vector < int > m_lens;

			}; // class Config

		public:
			static void load(sample_t&, const std::string&);

			static void load(ftr_sample_t&, const std::string&);

			static void save(const ftr_sample_t&, const std::string&);

			static void save(const pkg_sample_t&, const std::string&, const int);

			static void save(const wst_sample_t&, const std::string&, const int);

			static void load_last(pkg_sample_t&, const std::string&, const int);

			static void load_last(wst_sample_t&, const std::string&, const int);

			static void load_tmp(ftr_sample_t&);

			static void save_tmp(const std::vector < uintmax_t >&, const sample_t&);

			static void load_assets(std::vector< data::item::Asset >&);

			static bool no_packages(const std::string& source, const int len)
			{
				return Directory::empty(Directory::data_sources / source / Directory::packages / std::format("L{}", len));
			}

			static bool no_wordsets(const std::string& source, const int len)
			{
				return Directory::empty(Directory::data_sources / source / Directory::wordsets / std::format("L{}", len));
			}

		private:
			static void load(const path_t&, json_t&);

			static void save(const path_t&, const json_t&);
		};

	private:

		auto download() const;

		void filter_sample(std::vector < std::uintmax_t >&, const sample_t&, const ftr_sample_t&) const;

	public:
		void run();

	private:
		static inline constexpr auto m_timeout			= duration_t(4 * 60 * 60); // 4 hours
		//static inline constexpr auto m_len_package		= 30; // m_lan_package * m_timeout - time len package
		static inline constexpr auto m_default_start	= 1613779200;

	private:
		std::shared_ptr < python_t >	m_python;

		std::unique_ptr < data::Loader >		m_loader;
		std::unique_ptr < processing::Filter >	m_filter;

		Data::Config							m_config;
		std::vector < data::item::Asset >		m_assets;

	}; // class System

}; // namespace sentiment