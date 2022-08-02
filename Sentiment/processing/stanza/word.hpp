#pragma once
#include <string>

#include <nlohmann/json.hpp>
#include "../python/python.hpp"

namespace stanza
{
	struct Word
	{
		Word() = default;
		Word(const boost::python::api::object&);

		int id;
		int head;

		std::string lemma;
		std::string upos;
		//std::string feats;
		std::string deprel;
	};

	inline void to_json(nlohmann::json& obj, const Word& word)
	{
		obj = {
			{"id",		word.id},
			{"lemma",	word.lemma},
			{"upos",	word.upos},
			//{"feats",	word.feats},
			{"head",	word.head},
			{"deprel",	word.deprel},
		};
	}

	void from_json(const nlohmann::json&, Word&);
}
