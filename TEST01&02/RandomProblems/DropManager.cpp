#include "DropManager.h"


void DropTable::Release()
{
	std::unordered_map <EClassType, RandomTable<DropInfo>> ::iterator it;
	for (it = dropDataTable.begin(); it != dropDataTable.end(); it++) {
		it->second.Release();
	}
	dropDataTable.clear();
}

void DropTable::AddDropInfo(EClassType InClassType, const DropInfo& InInfo) const
{
	if (dropDataTable.find(InClassType) != dropDataTable.end()) {
		RandomTable<DropInfo> newRandomTable(0);
		dropDataTable.emplace(InClassType, std::move(newRandomTable));
	}
	dropDataTable[InClassType].AddRandom(InInfo.GetDropRate(), InInfo);
}

DropInfo DropTable::GetRandomItem(EClassType InClassType) const
{
	DropInfo outPickDropInfo;
	if (dropDataTable.find(InClassType) == dropDataTable.end()) {
		return outPickDropInfo;
	}
	dropDataTable[InClassType].GetRandom(outPickDropInfo);
	return outPickDropInfo;
}

void DropManager::Release()
{
	itemInfoList.clear();
	for (DropTable& dropTable : dropTableList) {
		dropTable.Release();
	}
	dropTableList.clear();
	itemHash.clear();
	dropTableHash.clear();
}

void DropManager::LoadItemTable()
{
	itemInfoList.clear();
	itemInfoList = { ItemInfo(1000001, EClassType::WARRIOR,	 1),
					ItemInfo(1000002, EClassType::WARRIOR,	 2),
					ItemInfo(1000003, EClassType::WARRIOR,	 3),
					ItemInfo(1001001, EClassType::SORCERESS, 1),
					ItemInfo(1001002, EClassType::SORCERESS, 2),
					ItemInfo(1001003, EClassType::SORCERESS, 3),
					ItemInfo(1002001, EClassType::ROGUE,	 1),
					ItemInfo(1002002, EClassType::ROGUE,	 2),
					ItemInfo(1002003, EClassType::ROGUE,	 3) };

	itemHash.clear();
	for (const ItemInfo& curItemInfo : itemInfoList)
	{
		if (itemHash.find(curItemInfo.GetItemId()) == itemHash.end()) {
			itemHash.emplace(curItemInfo.GetItemId(), std::make_shared<ItemInfo>(curItemInfo));
		}
	}
}

void DropManager::LoadDropTable()
{
	std::vector<DropInfo> loadedDropInfos = {
		DropInfo(1001, EClassType::WARRIOR,		1000001, 5),
		DropInfo(1001, EClassType::SORCERESS,	1001001, 5),
		DropInfo(1001, EClassType::ROGUE,		1002001, 5),

		DropInfo(1002, EClassType::WARRIOR,		1000001, 50),
		DropInfo(1002, EClassType::WARRIOR,		1000002, 5),
		DropInfo(1002, EClassType::SORCERESS,	1001001, 50),
		DropInfo(1002, EClassType::SORCERESS,	1001002, 5),
		DropInfo(1002, EClassType::ROGUE,		1002001, 50),
		DropInfo(1002, EClassType::ROGUE,		1002002, 5),

		DropInfo(1003, EClassType::WARRIOR,		1000001, 50),
		DropInfo(1003, EClassType::WARRIOR,		1000002, 5),
		DropInfo(1003, EClassType::WARRIOR,		1000003, 1),
		DropInfo(1003, EClassType::SORCERESS,	1001001, 50),
		DropInfo(1003, EClassType::SORCERESS,	1001002, 5),
		DropInfo(1003, EClassType::SORCERESS,	1001003, 1),
		DropInfo(1003, EClassType::ROGUE,		1002001, 50),
		DropInfo(1003, EClassType::ROGUE,		1002002, 5),
		DropInfo(1003, EClassType::ROGUE,		1002003, 1),

		DropInfo(1004, EClassType::WARRIOR,		1000001, 30),
		DropInfo(1004, EClassType::WARRIOR,		1000002, 15),
		DropInfo(1004, EClassType::WARRIOR,		1000003, 2),
		DropInfo(1004, EClassType::SORCERESS,	1001001, 30),
		DropInfo(1004, EClassType::SORCERESS,	1001002, 15),
		DropInfo(1002, EClassType::SORCERESS,	1001003, 2),
		DropInfo(1004, EClassType::ROGUE,		1002001, 30),
		DropInfo(1004, EClassType::ROGUE,		1002002, 15),
		DropInfo(1004, EClassType::ROGUE,		1002003, 2)
	};
	
	std::unordered_map<int32_t, int32_t> infoCounter;
	for (const DropInfo& dropInfo : loadedDropInfos) {
		const int32_t itemId = dropInfo.GetDropItemId();
		infoCounter[itemId]++;
	}

	for (const DropInfo& dropInfo : loadedDropInfos) {
		const int32_t itemTableId = dropInfo.GetDropTableId();
		const int32_t itemId = dropInfo.GetDropItemId();
		if (itemId == 0 || itemHash.find(dropInfo.GetDropItemId()) == itemHash.end()) {
			continue;
		}

		if (dropTableHash.find(itemTableId) == dropTableHash.end()) {
			const int32_t curInfoCnt = infoCounter[itemTableId];
			DropTable curDropTable(itemTableId, curInfoCnt);
			dropTableList.emplace_back(std::move(curDropTable));
			dropTableHash.emplace(itemTableId, std::make_shared<DropTable>(dropTableList[dropTableList.size() - 1]));
		}
				
		if (itemHash.find(itemId) != itemHash.end()
			&& dropTableHash.find(itemTableId) != dropTableHash.end()) 
		{
			const EClassType itemClassType = itemHash[itemId]->GetItemClassType();
			dropTableHash[itemTableId]->AddDropInfo(itemClassType, dropInfo);
		}		
	}
}

DropInfo DropManager::GetRandomItem(int32_t InDropTableId, EClassType InDropClassType)
{
	DropInfo outFind;
	if (dropTableHash.find(InDropTableId) != dropTableHash.end())
	{
		if (dropTableHash[InDropTableId]) {
			outFind = dropTableHash[InDropTableId]->GetRandomItem(InDropClassType);
		}
	}
	return outFind;
}

std::ostream& operator<<(std::ostream& os, const DropInfo& data)
{
	os << "Drop TableId : " << data.GetDropTableId() << std::endl;
	os << "Drop ItemId : " << data.GetDropItemId() << std::endl;
	os << "Drop Rate : " << data.GetDropRate() << std::endl;
	os << "Drop Class : " << ClassTypeToString(data.GetDropClass()) << std::endl;
	return os;
}
