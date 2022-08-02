#pragma once

#include <exception>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>

#include <nlohmann/json.hpp>

#include "../../python/python.hpp"

namespace sentiment::processing
{
	class stanza_exception : public std::exception
	{
	public:
		explicit stanza_exception(const std::string& message) noexcept : 
			std::exception(message.c_str()) {}

		explicit stanza_exception(const char * const message) noexcept : 
			std::exception(message) {}

		~stanza_exception() noexcept = default;
	};

	class Stanza
	{
	private:
		using python_t = solution::shared::Python;

	public:
		Stanza(std::shared_ptr < python_t >& python) 
			: m_python(python)
		{
			initialize();
		}

		~Stanza() = default;

	private:
		struct Interface
		{
			static inline constexpr auto import_cmd		= "from python.script import stanza_module\n";
			static inline constexpr auto variable_name	= "stanza_module";
			static inline constexpr auto run			= "run";
			static inline constexpr auto parralel_run	= "parallel_run";
		};

	private:
		void initialize()
		{
			try
			{
				boost::python::exec(Interface::import_cmd, m_python->global(), m_python->global());
			}
			catch (const boost::python::error_already_set&)
			{
				std::cerr << m_python->exception();
			}
		}

	public:
		//void execute(const std::string&);

		void parallel_run() const
		{
			try
			{
				m_python->global()[Interface::variable_name].attr(Interface::parralel_run)(std::thread::hardware_concurrency());
			}
			catch (const boost::python::error_already_set&)
			{
				std::cerr << m_python->exception();
			}
			
		}

		//auto& doc() const noexcept
		//{
		//	return m_doc;
		//}

	private:
		std::shared_ptr < python_t > m_python;
		//std::vector < std::vector < data::item::Token > > m_doc;
	};

	
}