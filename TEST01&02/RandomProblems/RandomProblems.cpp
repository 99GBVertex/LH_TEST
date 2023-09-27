#include <iostream>
#include <vector>
#include <memory>
#include <unordered_map>

#include "Define.h"
#include "RandomTable.h"
#include "Skill.h"
#include "Item.h"
#include "DropManager.h"

constexpr int kTest1TryCount = 5;
constexpr int kTest2TryCount = 1;

int main()
{
    // Test 1
    {
		std::cout << "Test1 Start" << std::endl << std::endl;

		std::shared_ptr<const Skill> skill1 = std::make_shared<Skill>(1, 15);
		std::shared_ptr<const Skill> skill2 = std::make_shared<Skill>(2, 25);
		std::shared_ptr<const Skill> skill3 = std::make_shared<Skill>(3, 10);

		RandomTable<std::weak_ptr<const Skill>> skillRandomTable(3);
		if (skill1) {
			skillRandomTable.AddRandom(skill1->GetSkillRate(), skill1);
		}
		if (skill2) {
			skillRandomTable.AddRandom(skill2->GetSkillRate(), skill2);
		}
		if (skill3) {
			skillRandomTable.AddRandom(skill3->GetSkillRate(), skill3);
		}

		for (int i = 0; i < kTest1TryCount; i++) 
		{
			std::weak_ptr<const Skill> selectedSkill;
			if (skillRandomTable.GetRandom(selectedSkill))
			{
				if (!selectedSkill.expired())
				{
					std::cout << "Pick this skill" << std::endl
						<< selectedSkill << std::endl;
				}
			}
		}
		skill1.reset();
		skill2.reset();
		skill3.reset();

		std::cout << "Test1 End" << std::endl << std::endl;
    }

	// Test 2
	{
		std::cout << "Test2 Start" << std::endl << std::endl;

		DropManager dropMgr;
		dropMgr.LoadItemTable();
		dropMgr.LoadDropTable();

		typedef std::pair<int32_t, EClassType> dropCall;
		std::vector<dropCall> dropCalls = {
											{1001, EClassType::WARRIOR},
											{1001, EClassType::SORCERESS},
											{1001, EClassType::ROGUE},
											{1002, EClassType::ROGUE},
											{1003, EClassType::ROGUE},
											{1004, EClassType::ROGUE} };

		for (int i = 0; i < kTest2TryCount; i++) 
		{
			for (const dropCall& curCall : dropCalls)
			{
				std::cout << "Qry Drop!" << std::endl
					<< "Pick Table Id : " << curCall.first << std::endl
					<< "Pick Table Class : " << ClassTypeToString(curCall.second) << std::endl;

				const int32_t dropTableId = curCall.first;
				const EClassType dropClassType = curCall.second;
				const DropInfo pickedDropInfo = dropMgr.GetRandomItem(dropTableId, dropClassType);
				if (pickedDropInfo.GetDropItemId() == 0) {
					std::cout << "Rpl Drop!" << std::endl
						<< "Invalid Result!!!" << std::endl << std::endl;
				}
				else {
					std::cout << "Rpl Drop!" << std::endl
						<< (pickedDropInfo) << std::endl << std::endl;
				}
			}
		}
		std::cout << "Test2 End" << std::endl << std::endl;
	}

	system("pause");

	return 1;
}