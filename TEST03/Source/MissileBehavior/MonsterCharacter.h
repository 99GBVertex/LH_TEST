// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "MonsterCharacter.generated.h"

UENUM()
enum class EMonsterState : uint8
{
	None,
	Move,
	Death,
};
/**
 * 
 */
UCLASS()
class MISSILEBEHAVIOR_API AMonsterCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:	
	AMonsterCharacter();

protected:
	// ~Begin AActor
	virtual void Tick(float DeltaTime) override;
	// ~End AActor

public:
	UFUNCTION()
		void OnHitted();

	void Reset();
	void UpdateState();

	EMonsterState GetMonsterState()	{ return m_MonsterState; }
	EMonsterState GetMonsterState() const { return m_MonsterState; }

private:
	FNewLocation m_Direction = FNewLocation(1.f, 0.f, 0.f);

	UPROPERTY(EditAnywhere, Category="Monster Base Info")
		float m_Velocity = 3.f;
	UPROPERTY(EditAnywhere, Category = "Monster Base Info")
		float m_MoveChangeTime = 1.f;
	UPROPERTY(VisibleAnywhere, Category = "Monster Base Info")
		float m_CurrentTime = 0.f;
	UPROPERTY(VisibleAnywhere, Category = "Monster Base Info")
		EMonsterState m_MonsterState = EMonsterState::None;
};