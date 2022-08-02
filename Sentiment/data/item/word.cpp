#include "word.hpp"

namespace sentiment::data::item
{
	void from_json(const nlohmann::json& obj, Word& word)
	{
		word.id		= obj["id"].get < int >();
		word.head	= obj["head"].get < int >();

		word.lemma	= obj["lemma"].get < std::string >();
		word.upos	= obj["upos"].get < std::string >();
		word.deprel = obj["deprel"].get < std::string >();
	}
}