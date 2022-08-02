#pragma once

#include <istream>
#include <iterator>

#include "csv_row.hpp"

namespace csv
{
	class CSV_Iterator
	{
	public:
		using iterator_category	= std::input_iterator_tag;
		using difference_type	= std::size_t;
		using value_type		= CSV_Row;
		using pointer			= value_type*;
		using reference			= value_type&;
		using const_pointer		= const value_type*;
		using const_reference	= const value_type&;

	public:
		CSV_Iterator() : m_stream(nullptr), m_row() {};

		CSV_Iterator(const char del) : m_stream(nullptr), m_row(del) {};

		CSV_Iterator(std::istream& stream) :
			m_stream(stream.good() ? &stream : nullptr), m_row()
		{
			++(*this);
		}

		CSV_Iterator(std::istream& stream, const char del) :
			m_stream(stream.good() ? &stream : nullptr), m_row(del)
		{
			++(*this);
		}

	public:
		CSV_Iterator& operator++();

		CSV_Iterator operator++(int);

		const_reference operator*() const noexcept
		{
			return m_row;
		}

		reference operator*() noexcept
		{
			return m_row;
		}

		const_pointer operator->() const noexcept
		{
			return &m_row;
		}

		pointer operator->() noexcept
		{
			return &m_row;
		}

		bool operator==(const CSV_Iterator& rhs) const noexcept
		{
			return (this == &rhs || (this->m_stream == nullptr) && (rhs.m_stream == nullptr));
		}

		bool operator!=(const CSV_Iterator& rhs) const noexcept
		{
			return !(*this == rhs);
		}

	private:
		std::istream* m_stream;
		value_type		m_row;
	};
}