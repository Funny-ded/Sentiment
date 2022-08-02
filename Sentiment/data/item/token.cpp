#include "token.hpp"

void sentiment::data::item::from_json(const nlohmann::json& obj, Token& token)
{
	token.m_name = obj["name"].get < std::string >();
	token.m_part = obj["part"].get < double >();
	token.m_count = obj["count"].get < int >();
}
