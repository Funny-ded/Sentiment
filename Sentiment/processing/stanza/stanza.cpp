#include "stanza.hpp"
#include <iostream>

//void sentiment::processing::Stanza::execute(const std::string& doc)
//{
//	m_doc.clear();
//
//	try
//	{
//		boost::python::list stanza_processed = 
//			boost::python::extract < boost::python::list >(m_python->global()[Interface::variable_name].attr(Interface::run)(doc.c_str()));
//		
//		m_doc.reserve(boost::python::len(stanza_processed));
//
//		for (auto i = 0U; i < boost::python::len(stanza_processed); ++i)
//		{
//			m_doc.push_back(std::vector < data::item::Token > ());
//
//			m_doc.back().reserve(boost::python::len(stanza_processed[i]));
//
//			for (auto j = 0U; j < boost::python::len(stanza_processed[i]); ++j)
//			{
//				m_doc.back().push_back(data::item::Token(stanza_processed[i][j]));
//			}
//		}
//	}
//	catch (boost::python::error_already_set&)
//	{
//		throw stanza_exception("Stanza: " + m_python->exception());
//	}
//}