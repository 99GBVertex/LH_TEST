#pragma once

#include <cstdint>

struct Skill
{
public:
	explicit Skill(int32_t InSkillId, int64_t InSkillRate)
		: skillId(InSkillId), skillRate(InSkillRate) {}
	virtual ~Skill() {}

	int64_t GetSkillRate() const { return skillRate; }
	int32_t GetSkillId() const { return skillId; }

private:
	int64_t skillRate = 0;
	int32_t skillId = 0;
};

std::ostream& operator<<(std::ostream& os, std::weak_ptr<const Skill> data) {
	if (!data.expired())
	{
		os << "Skill Id : " << data.lock()->GetSkillId() << std::endl;
		os << "Skill Rate : " << data.lock()->GetSkillRate() << std::endl;
	}
	return os;
}