#pragma once

#include <istream>
#include <string>
#include <vector>

namespace csv
{
	class CSV_Row
	{
	public:
		CSV_Row() : m_delimeter(' '), m_line(), m_data() {};

		CSV_Row(const char del) : m_delimeter(del), m_line(), m_data() {}

		~CSV_Row() noexcept = default;

	public:
		auto operator[](const std::size_t index) const
		{
			return std::string(&m_line[m_data[index] + 1], (m_data[index + 1]) - (m_data[index] + 1));
		}

		constexpr auto size() const
		{
			return std::size(m_data) - 1;
		}

	public:
		std::string			m_line;
		std::vector < int > m_data;
		char				m_delimeter;

	private:
		friend std::istream& operator>> (std::istream&, CSV_Row&);

	};

	std::istream & operator>>(std::istream & stream, CSV_Row& row);
}