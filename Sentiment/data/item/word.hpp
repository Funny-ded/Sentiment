#pragma once
#include <string>

#include <nlohmann/json.hpp>

namespace sentiment::data::item
{
	struct Word
	{
		Word() = default;

		int id;
		int head;

		std::string lemma;
		std::string upos;
		std::string deprel;
	};

	inline void to_json(nlohmann::json& obj, const Word& word)
	{
		obj = {
			{"id",		word.id},
			{"lemma",	word.lemma},
			{"upos",	word.upos},
			{"head",	word.head},
			{"deprel",	word.deprel},
		};
	}

	void from_json(const nlohmann::json&, Word&);
}
