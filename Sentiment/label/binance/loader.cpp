#include "loader.hpp"

namespace sentiment::data
{
	void Binance_Loader::run(const std::vector < item::Asset >& assets, const char* const timeframe)
	{
		try
		{
			for (const auto& asset : assets)
			{
				m_python->global()[Interface::variable_name](asset.m_name.c_str(), timeframe);
			}
		}
		catch (const boost::python::error_already_set&)
		{
			throw binance_loader_exception(m_python->exception());
		}
		
	}
}