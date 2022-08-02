#include "word.hpp"

namespace stanza
{
	void from_json(const nlohmann::json& obj, Word& word)
	{
		word.id		= obj["id"]		.get < int >();
		word.head	= obj["head"]	.get < int >();

		word.lemma	= obj["lemma"]	.get < std::string >();
		word.upos	= obj["upos"]	.get < std::string >();
		//word.feats	= obj["feats"]	.get < std::string >();
		word.deprel = obj["deprel"]	.get < std::string >();
	}

	Word::Word(const boost::python::api::object& obj)
	{
		id		= boost::python::extract < int >(obj["id"])();
		head	= boost::python::extract < int >(obj["head"])();

		lemma	= boost::python::extract < std::string >(obj["lemma"])();
		upos	= boost::python::extract < std::string >(obj["upos"])();
		//feats	= boost::python::extract < std::string >(obj["feats"])();
		deprel	= boost::python::extract < std::string >(obj["deprel"])();
	}


}