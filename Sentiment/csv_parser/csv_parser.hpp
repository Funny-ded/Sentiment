#pragma once

#include <exception>
#include <filesystem>
#include <istream>
#include <string>
#include <vector>

#include "csv_iterator.hpp"
#include "csv_row.hpp"
#include "csv_traits.hpp"

namespace csv
{
	class csv_parser_exception : std::exception
	{
	public:
		explicit csv_parser_exception(const std::string& message) noexcept :
			std::exception(message.c_str()) {}

		explicit csv_parser_exception(const char * const message) noexcept :
			std::exception(message) {}

		~csv_parser_exception() noexcept = default;
	};

	class CSV_Parser
	{
	private:

	public:
		
		CSV_Parser(std::istream& stream) : m_stream(stream), m_del(' ') {}

	public:

		CSV_Iterator begin() const
		{
			return CSV_Iterator(m_stream, m_del);
		}

		CSV_Iterator end() const
		{
			return CSV_Iterator(m_del);
		}

		//template < typename T, std::enable_if< has_from_csv< T >::v > >
		//T & parse() const;

	private:
		std::istream& m_stream;
		char m_del;

	};
}
