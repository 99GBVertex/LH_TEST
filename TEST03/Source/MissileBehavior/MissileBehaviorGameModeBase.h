// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MissileBehaviorGameModeBase.generated.h"

class APlayerCharacter;
class AMonsterCharacter;

/**
 * 
 */
UCLASS()
class MISSILEBEHAVIOR_API AMissileBehaviorGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMissileBehaviorGameModeBase();

	// ~Begin AGameModeBase
	virtual void StartPlay() override;
	virtual void Logout(AController* Exiting);
	// ~Begin AGameModeBase

	UFUNCTION()
		void OnResetMonsters();

private:
	UPROPERTY() TSubclassOf<APlayerCharacter> m_PlayerClass;
	UPROPERTY() TSubclassOf<AMonsterCharacter> m_MonsterClass;

	UPROPERTY() TObjectPtr<APlayerCharacter> m_Player;
	UPROPERTY() TArray<TObjectPtr<AMonsterCharacter>> m_Monsters;

	UPROPERTY(EditAnywhere, Category = "GameMove Info")
		int32 m_MonsterCount = 200; // clamp 1 ~ 100
};
