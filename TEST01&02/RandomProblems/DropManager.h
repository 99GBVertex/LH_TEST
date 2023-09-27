#pragma once

#include <cstdint>
#include <vector>
#include <unordered_map>

#include "RandomTable.h"
#include "Item.h"

struct ItemInfo;
enum class EClassType : uint8_t;

struct DropInfo
{
	explicit DropInfo() {
		dropTableId = 0;
		dropItemId = 0;
		dropRate = 0;
		dropClass = EClassType::NONE;
	}
	explicit DropInfo(int32_t InDropTableId, EClassType InDropClass, int32_t InDropItemId, int64_t InDropRate) :
		dropTableId(InDropTableId), dropClass(InDropClass), dropItemId(InDropItemId), dropRate(InDropRate) {}
	~DropInfo() {}

	int32_t GetDropTableId() const { return dropTableId; }
	int32_t GetDropItemId() const { return dropItemId; }
	int64_t GetDropRate() const { return dropRate; }
	EClassType GetDropClass() const { return dropClass; }
private:
	int32_t dropTableId = 0;
	int32_t dropItemId = 0;
	int64_t dropRate = 0;
	EClassType dropClass = EClassType::NONE;
};

std::ostream& operator<<(std::ostream&, const DropInfo&);

struct DropTable
{
	explicit DropTable(int32_t InDropTableId, int32_t InDropTableQuantity = 0) : dropDataTable(InDropTableId)
	{
		dropDataTable.clear();
		dropDataTable.reserve(InDropTableQuantity);
	}
	~DropTable() {
		Release();
	}

	void Release();

	void AddDropInfo(EClassType InClassType, const DropInfo& InInfo) const;
	DropInfo GetRandomItem(EClassType InClassType) const;

private:
	int32_t dropTableId = 0;
	mutable std::unordered_map<EClassType, RandomTable<DropInfo>> dropDataTable;
};

class DropManager
{
public:
	DropManager() {}
	~ DropManager() {}

	void Release();

	void LoadItemTable();
	void LoadDropTable();

	DropInfo GetRandomItem(int32_t InDropTableId, EClassType InDropClassType);

private:
	std::vector<ItemInfo> itemInfoList;
	std::vector<DropTable> dropTableList;

	std::unordered_map<int32_t, const std::shared_ptr<ItemInfo>> itemHash;
	std::unordered_map<int32_t, const std::shared_ptr<DropTable>> dropTableHash;
};