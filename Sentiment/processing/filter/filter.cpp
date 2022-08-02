#include "filter.hpp"

#include<iostream>

namespace sentiment::processing
{
	void Filter::replace_enter(std::string& text) const
	{
		std::vector < std::string > words;
		split(text, '\n', words);

		text.clear();
		for (const auto& word : words)
		{
			text += word + std::string(". ");
		}
	}

	void Filter::exclude_hashtag(message_tokens_t& words) const
	{
		words.erase(
			std::remove_if(std::begin(words), std::end(words), 
				[](const auto& sentence)
				{
					auto num_bad_relations = 0;

					for (auto& word : sentence)
					{
						if (std::find_if(std::begin(bad_relations), std::end(bad_relations),
							[&word](const auto& bad) { return word.deprel == bad && word.deprel != "root" && word.deprel != "punct"; })
							!= std::end(bad_relations))
						{
							++num_bad_relations;
						}
					}

					return num_bad_relations > std::size(sentence) / 2;
				}
		), std::end(words));
	}

	void Filter::exclude_other_symbols(message_tokens_t& words) const
	{
		for(auto& sentence: words)
			for (auto& word : sentence)
			{
				word.lemma = std::regex_replace(word.lemma, Pattern::other_symbols, "");
			}
	}

	void Filter::exclude_words(message_tokens_t& words) const
	{
		for (auto& sentence : words)
		{
			sentence.erase(std::remove_if(std::begin(sentence), std::end(sentence),
				[this](const auto& word)
				{ 
					return std::size(word.lemma) <= 1 
						|| std::find(std::begin(excluded_words), std::end(excluded_words), word.lemma) != std::end(excluded_words); 
				}), std::end(sentence));
		}
	}

	void Filter::to_lower(message_tokens_t& words) const
	{
		for (auto& sentence: words)
			for (auto& word : sentence)
			{
				std::transform(std::begin(word.lemma), std::end(word.lemma), std::begin(word.lemma),
					[](auto c) { return std::tolower(c); });
			}
	}

	void Filter::exclude_empty(ftr_sample_t& ftr_sample) const
	{
		for (auto& ftr_message : ftr_sample)
		{
			ftr_message.words.erase(std::remove_if(std::begin(ftr_message.words), std::end(ftr_message.words), [](const auto& sentence)
				{ return sentence.empty(); }), std::end(ftr_message.words));
		}

		ftr_sample.erase(std::remove_if(std::begin(ftr_sample), std::end(ftr_sample), [](const auto& item)
			{ return item.words.empty(); }), std::end(ftr_sample));
	}

	void processing::Filter::split(const std::string& s, char delim, std::vector<std::string>& elems) const
	{
		std::stringstream ss(s);
		std::string item;
		for (; std::getline(ss, item, delim);)
		{
			elems.push_back(item);
		}
	}

	void Filter::preprocess(const std::vector < uintmax_t >& filter_ids, sample_t& sample) const
	{
		for (const auto& id : filter_ids)
		{
			try
			{
				exclude_refs(sample[id].text);
				exclude_emoji(sample[id].text);
				replace_enter(sample[id].text);
			}
			catch (const std::regex_error& e)
			{
				std::cerr << "regex error while processing text:\n" << sample[id].text << std::endl;
				std::cerr << "Error: " << e.what() << std::endl;
			}
		}
		
	}

	void Filter::postprocess(ftr_sample_t& data) const
	{
		for (auto& ftr_message : data)
		{
			exclude_other_symbols(ftr_message.words);
			to_lower(ftr_message.words);
			exclude_words(ftr_message.words);
		}

		exclude_empty(data);
	}
}