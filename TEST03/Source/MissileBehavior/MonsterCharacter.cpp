// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterCharacter.h"

AMonsterCharacter::AMonsterCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void AMonsterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_CurrentTime == 0 || m_CurrentTime > m_MoveChangeTime) {
		m_CurrentTime -= m_MoveChangeTime;
		const float dirX = FMath::RandRange(-1.f, 1.f);
		const float dirY = FMath::RandRange(-1.f, 1.f);
		const float dirZ = FMath::RandRange(-1.f, 1.f);
		m_Direction.SetLocation(FNewLocation(dirX, dirY, dirZ));
	}
	m_CurrentTime += DeltaTime;

	FNewLocation addPos = (m_Direction * m_Velocity * DeltaTime);
	m_CharacterLocation += addPos;
	SetActorLocation(m_CharacterLocation.ToVector());
}

void AMonsterCharacter::OnHitted()
{
	m_Follower = nullptr;
	m_MonsterState = EMonsterState::Death;
	UpdateState();
}

void AMonsterCharacter::Reset()
{
	m_Follower = nullptr;
	m_MonsterState = EMonsterState::Move;
	UpdateState();
}

void AMonsterCharacter::UpdateState()
{
	switch (m_MonsterState)
	{
	default:
	case EMonsterState::None:
		break;
	case EMonsterState::Move:
		SetActorHiddenInGame(false);
		break;
	case EMonsterState::Death:
		SetActorHiddenInGame(true);
		break;
	}
}
