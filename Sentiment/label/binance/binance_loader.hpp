#pragma once

#include <exception>
#include <memory>
#include <string>

#include "../../data/item/asset.hpp"

#include "../../python/python.hpp"

namespace sentiment::data
{
	class binance_loader_exception : public std::exception
	{
	public:
		explicit binance_loader_exception(const std::string& message) noexcept :
			std::exception(message.c_str()) {}

		explicit binance_loader_exception(const char * const message) noexcept :
			std::exception(message) {}

		~binance_loader_exception() noexcept = default;
	};

	class Binance_Loader
	{
	private:

		using python_t = solution::shared::Python;

	private:

		struct Interface
		{
			static inline constexpr auto import_cmd = "from python.label.script import loader";
			static inline constexpr auto variable_name = "loader";
		};
		
	public:

		Binance_Loader(const std::shared_ptr < python_t >& python) :
			m_python(python) 
		{
			initialize();
		}

		~Binance_Loader() noexcept = default;

	private:

		void initialize()
		{
			boost::python::exec(Interface::import_cmd, m_python->global(), m_python->global());	
		}

	public:

		void run(const std::vector < item::Asset > & assets, const std::string& timeframe)
		{
			run(assets, timeframe.c_str());
		}

		void run(const std::vector < item::Asset >&, const char* const);

	private:
		std::shared_ptr < python_t > m_python;
	};
}