#pragma once

#include <iostream>
#include <cstdint>
#include <string>

#include "Define.h"

struct ItemInfo
{
public:
	explicit ItemInfo(int32_t InItemId, EClassType InItemClassType, int32_t InItemValue)
		: itemId(InItemId), itemClassType(InItemClassType), itemValue(InItemValue) {}
	virtual ~ItemInfo() {}

	int32_t GetItemId() const { return itemId; }
	EClassType GetItemClassType() const { return itemClassType; }
	int32_t GetItemValue() const { return itemValue; }

private:
	int32_t itemId = 0;
	int32_t itemValue = 0;
	EClassType itemClassType = EClassType::NONE;
};