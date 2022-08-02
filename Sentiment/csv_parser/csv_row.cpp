#include "csv_row.hpp"

namespace csv
{
	std::istream & operator>> (std::istream & stream, CSV_Row& row)
	{
		std::getline(stream, row.m_line);

		row.m_data.clear();
		row.m_data.push_back(-1);

		for (auto pos = row.m_line.find(row.m_delimeter, 0); pos != std::string::npos; pos = row.m_line.find(row.m_delimeter, pos))
		{
			row.m_data.push_back(pos);
			++pos;
		}

		row.m_data.push_back(std::size(row.m_line));

		return stream;
	}
}