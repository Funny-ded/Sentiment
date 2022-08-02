#pragma once

#include <algorithm>
#include <memory>
#include <regex>
#include <set>
#include <string>

#include "../stanza/stanza.hpp"

#include "../../data/item/asset.hpp"
#include "../../data/item/ftr_message.hpp"
#include "../../data/item/message.hpp"

#include "../../python/python.hpp"

namespace sentiment::processing
{
	class filter_exception : public std::exception
	{
	public:
		explicit filter_exception(const char* const message) :
			std::exception(message)
		{}

		explicit filter_exception(const std::string& message) :
			std::exception(message.c_str())
		{}

		~filter_exception() noexcept = default;
	};

	class Filter
	{
	private:
		using sample_t		= std::vector < data::item::Message >;
		using ftr_sample_t	= std::vector < data::item::FTR_Message >;

		using message_tokens_t	= std::vector < std::vector < data::item::Word > >;
		using python_t			= solution::shared::Python;


	public:
		Filter(std::shared_ptr < python_t >& python, std::vector < data::item::Asset > assets) :
			m_stanza(std::make_shared < Stanza >(python)), m_assets(assets)
		{}

		~Filter() noexcept = default;

	private:
		void exclude_refs(std::string& text) const
		{
			text = std::regex_replace(text, Pattern::refs, "");
		}

		void exclude_emoji(std::string& text) const
		{
			text.erase(std::remove_if(std::begin(text), std::end(text),
				[](const auto& c) { return c != '\n' && !std::isprint(c); }), std::end(text));
		}

		void replace_enter(std::string&) const;

		void exclude_hashtag_symbols(std::string& text) const
		{
			text = std::regex_replace(text, Pattern::hashtag_symbol, "");
		}

		void exclude_hashtag(message_tokens_t&) const;

		void exclude_other_symbols(message_tokens_t&) const;

		// special and one symbol length
		void exclude_words(message_tokens_t&) const;

		void to_lower(message_tokens_t&) const;

		void exclude_empty(ftr_sample_t&) const;

		void split(const std::string& s, char delim, std::vector<std::string>& elems) const;

	public:
		void preprocess(const std::vector < uintmax_t >&, sample_t&) const;

		void stanza_process() const
		{
			m_stanza->parallel_run();
		}

		void postprocess(ftr_sample_t&) const;

	private:
		struct Pattern
		{
			static inline const auto refs = std::regex(R"(https?:\/\/(www\.)?[-a-zA-Z0-9@:%._\/+~#=]{1,256}\.[a-zA-Z0-9()]{1,6}([-a-zA-Z0-9()@:%_\+.~#?&\/=]*)*)");
			static inline const auto hashtag_symbol = std::regex(R"([#$]+)");
			static inline const auto enter_after_any = std::regex(R"(([^.,!?\n\r ] ?)([\n\r]+))");
			static inline const auto enter_after_punct = std::regex(R"(([.,!?] *)([\n\r]+))");
			static inline const auto other_symbols = std::regex(R"(([,.!?\"#$%&()*+\-/:;<=>@\[\]^_`{|}'~\d\n\r]+))");
		};

	private:
		static inline const std::vector < std::string > excluded_words = {
			"i", "you", "he", "she", "it", "we", "they", "this", "that" // pronouns
			"a", "an", "the",											// articles
			"be", "have", "do",											// verbs
			"to", "of", "in", "for", "on", "with", "at", "about",		// prepositions
			"and", "if", "or"											// conjuctions
		};

		static inline const std::vector < std::string > bad_relations = {
			"flat",
			"compound",
			"list"
		};

	private:
		std::shared_ptr < Stanza >			m_stanza;
		std::vector < data::item::Asset >	m_assets;
	};
}