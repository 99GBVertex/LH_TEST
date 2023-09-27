#pragma once

#include <vector>
#include <iostream>
#include <random>

enum class ERandomSection
{
	NONE,
	SKILL,
	DROP,
};

template<typename T>
class RandomTable
{
public:
	RandomTable() {}
	RandomTable(const std::size_t InPoolCapacity) {
		randPool.reserve(InPoolCapacity);
	};
	~RandomTable() {};

public:
	void Release();

	//확률과 데이터 입력
	void AddRandom(int64_t InRate, const T& InData);

	//확률에 따라 데이터 1개를 리턴하고 값이 없으면 false 리턴
	bool GetRandom(T& OutData) const;

private:
	std::vector<std::pair<int64_t, T>> randPool;
	int64_t maxRate = 0;

	static std::random_device rd;
	static std::mt19937 gen;
};

template <typename T>
std::random_device RandomTable<T>::rd;

template<typename T>
std::mt19937 RandomTable<T>::gen = std::mt19937(RandomTable<T>::rd());

template<typename T>
inline void RandomTable<T>::Release()
{
	randPool.clear();
	maxRate = 0;
}

template<typename T>
inline void RandomTable<T>::AddRandom(int64_t InRate, const T& InData)
{
	if (maxRate + InRate < 0)
	{
		std::cout << "over maxRate." << std::endl
			<< (InData) << __func__ << __LINE__ << std::endl;
		return;
	}
	std::cout << "Add Random Info" << std::endl
		<< (InData) << std::endl;

	maxRate += InRate;
	randPool.emplace_back(maxRate, InData);
}

template<typename T>
inline bool RandomTable<T>::GetRandom(T& OutData) const
{
	if (randPool.size() == 0) {
		return false;
	}

	std::uniform_int_distribution<int64_t> dis(0, maxRate-1);
	int64_t rand = dis(gen);
	for (std::pair<int64_t, T> kvp : randPool) {
		if (kvp.first > rand) {
			OutData = kvp.second;
			break;
		}
	}
	return true;
}