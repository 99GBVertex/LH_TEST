// Copyright Epic Games, Inc. All Rights Reserved.


#include "MissileBehaviorGameModeBase.h"
#include "PlayerCharacter.h"
#include "MonsterCharacter.h"

AMissileBehaviorGameModeBase::AMissileBehaviorGameModeBase()
{
	if (!HasAnyFlags(RF_ClassDefaultObject))
	{
		static ConstructorHelpers::FClassFinder<APlayerCharacter> PlayerCharacterFinder(TEXT("/Game/APlayerCharacter"));
		if (PlayerCharacterFinder.Succeeded())
		{
			m_PlayerClass = PlayerCharacterFinder.Class;
		}

		static ConstructorHelpers::FClassFinder<AMonsterCharacter> MonsterCharacterFinder(TEXT("/Game/AMonsterCharacter"));
		if (MonsterCharacterFinder.Succeeded())
		{
			m_MonsterClass = MonsterCharacterFinder.Class;
		}
	}
}

void AMissileBehaviorGameModeBase::StartPlay()
{
	TObjectPtr<APawn> defaultPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (IsValid(defaultPawn)) {
		defaultPawn->SetActorHiddenInGame(true);
	}
	m_MonsterCount = FMath::Clamp<int>(m_MonsterCount, 1, 200);
	if(IsValid(m_MonsterClass))
	{
		for (int32 monsterIdx = 0; monsterIdx < m_MonsterCount; monsterIdx++)
		{
			TObjectPtr<AMonsterCharacter> spawnMonster = GetWorld()->SpawnActor<AMonsterCharacter>(m_MonsterClass, FTransform::Identity);
			if (IsValid(spawnMonster))
			{
				spawnMonster->SetLocation(spawnMonster->GetActorLocation());

				const float randX = FMath::RandRange(-100.f, 100.f);
				const float randY = FMath::RandRange(-100.f, 100.f);
				const float randZ = FMath::RandRange(-100.f, 100.f);
				FNewLocation shuffleLocation = FNewLocation(randX, randY, randZ);
				spawnMonster->AddLocation(shuffleLocation);
				spawnMonster->Reset();

				m_Monsters.Emplace(spawnMonster);
			}
		}
	}

	if (IsValid(m_PlayerClass))
	{
		m_Player = GetWorld()->SpawnActor<APlayerCharacter>(m_PlayerClass, FTransform::Identity);
		m_Player->SetLocation(m_Player->GetActorLocation());
		m_Player->ShootActivate(true);
	}
	Super::StartPlay();

	TObjectPtr<APlayerController> mainPC = GetWorld()->GetFirstPlayerController();
	if (mainPC) {
		mainPC->InputComponent->BindAction("Reset", IE_Released, this, &ThisClass::OnResetMonsters);
	}

	GEngine->AddOnScreenDebugMessage(0, 30000.f, FColor::Black, TEXT("RESET MONSTERS : [R]"), true, FVector2D(2.f, 2.f));
}

void AMissileBehaviorGameModeBase::Logout(AController* Exiting)
{
	if (IsValid(m_Player)) {
		m_Player->Release();
	}
	for(const TObjectPtr<AMonsterCharacter>& monster : m_Monsters)
	{
		if (IsValid(monster)) {
			monster->Release();
		}
	}
	m_Monsters.Empty(0);
}

void AMissileBehaviorGameModeBase::OnResetMonsters()
{
	for (const TObjectPtr<AMonsterCharacter>& monster : m_Monsters) {
		monster->Reset();
	}
}
