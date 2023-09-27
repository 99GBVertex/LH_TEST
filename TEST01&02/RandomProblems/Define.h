#pragma once

#include <string>

enum class EClassType : uint8_t
{
	NONE,
	WARRIOR,
	SORCERESS,
	ROGUE
};

constexpr static const char* ClassTypeToString(EClassType classType) {
	switch (classType)
	{
	default:
	case EClassType::NONE:		return "";
	case EClassType::WARRIOR:	return "warrior";
	case EClassType::SORCERESS:	return "sorceress";
	case EClassType::ROGUE:		return "rogue";
	}
};
