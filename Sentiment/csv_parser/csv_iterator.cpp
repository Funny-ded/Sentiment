#include "csv_iterator.hpp"

namespace csv
{
	CSV_Iterator& CSV_Iterator::operator++()
	{
		if (m_stream != nullptr)
		{
			if (!(*m_stream >> m_row))
				m_stream = nullptr;
		}

		return *this;
	}

	CSV_Iterator CSV_Iterator::operator++(int)
	{
		CSV_Iterator tmp(*this);
		++(*this);

		return tmp;
	}
}