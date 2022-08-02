#pragma once

#include <type_traits>

namespace csv
{
	template < typename T >
	struct has_from_csv : std::false_type {};
}