#include "loader.hpp"

void sentiment::data::Loader::download(const std::uint64_t start_date, const std::uint64_t end_date)
{
	try
	{
		m_python->global()[m_source].attr(source_download)(start_date, end_date);
	}
	catch (const boost::python::error_already_set&)
	{
		throw loader_exception(std::format("Loader exception on downloading: {}", m_python->exception()));
	}
}

void sentiment::data::Loader::set_source(const std::string& source)
{
	m_source = source;

	try
	{
		if (!imported(m_source))
		{
			m_imported.push_back(m_source);

			boost::python::exec(std::format(import_string, m_source, m_source).c_str(), m_python->global(), m_python->global());
		}
	}
	catch (const boost::python::error_already_set&)
	{
		throw loader_exception(std::format("Loader exception on setting source: {}", m_python->exception()));
	}
}
