#pragma once

#include <exception>
#include <memory>
#include <string>
#include <format>

#include "../python/python.hpp"

namespace sentiment
{
	namespace data
	{
		class loader_exception : public std::exception
		{
		public:
			loader_exception(const char * const message) noexcept :
				std::exception(message) {}

			loader_exception(const std::string & message) noexcept :
				std::exception(message.c_str()) {}

			~loader_exception() noexcept = default;
		};

		class Loader
		{
		private:
			using python_t = solution::shared::Python;

		public:
			Loader(std::shared_ptr < python_t > & python) : m_python(python)
			{}

			~Loader() noexcept = default;

		private:

			bool imported(const std::string& source) const
			{
				return std::find(std::begin(m_imported), std::end(m_imported), m_source) != std::end(m_imported);
			}

		public:
			void download(const std::uint64_t start_date, const std::uint64_t end_date);

			void set_source(const std::string& source);

		private:
			static inline constexpr auto source_download	= "update_data";
			static inline constexpr auto import_string		= "import data.sources.{}.Loader as {}";

		private:
			std::shared_ptr < python_t >	m_python;
			std::string						m_source;

			std::vector < std::string >		m_imported;
		};
	}
}