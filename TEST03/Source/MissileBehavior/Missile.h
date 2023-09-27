// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NewLocation.h"
#include "Missile.generated.h"

class ABaseCharacter;
struct NewLocation;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMissileDetacted, AMissile*, missile);


UCLASS()
class MISSILEBEHAVIOR_API AMissile : public AActor
{
	GENERATED_BODY()
	
public:	
	AMissile();

	// ~Begin AActor
	virtual void BeginPlay() override;
	// ~End AActor
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetMissile(
		const TObjectPtr<const ABaseCharacter>& InShooter,
		const TObjectPtr<const ABaseCharacter>& InTarget);

	void UpdateTarget(float DeltaTime);
	void Release();
	void StopMissile();

	FOnMissileDetacted OnMissileDetacted;

	int32 GetMissileID() const { return m_MissileId; }
private:
	static int32 MissileCounter;
	int32 m_MissileId = 0;

	UPROPERTY(VisibleAnywhere, Category = "Default Missile Info")
		FNewLocation m_CurrentLocation = FNewLocation(0.f, 0.f, 0.f);
	UPROPERTY(VisibleAnywhere, Category = "Default Missile Info")
		FNewLocation m_PrevLocation = FNewLocation(0.f, 0.f, 0.f);

	UPROPERTY(VisibleAnywhere, Category = "Default Missile Info")
		FNewLocation m_Direction = FNewLocation(0.f, 0.f, 0.f);

	UPROPERTY(VisibleAnywhere, Category = "Default Missile Info")
		FNewLocation m_DestLocation = FNewLocation(0.f, 0.f, 0.f);


	UPROPERTY(EditAnywhere, Category = "Default Missile Info")
		float m_Velocity = 3.f;
	UPROPERTY(EditAnywhere, Category = "Default Missile Info")
		float m_HitRadius = 5.f;

	UPROPERTY(VisibleAnywhere, Category = "Default Missile Info")
		TObjectPtr<const ABaseCharacter> m_Start;
	UPROPERTY(VisibleAnywhere, Category = "Default Missile Info")
		TObjectPtr<const ABaseCharacter> m_Target;

	UPROPERTY(VisibleAnywhere, Category = "Default Missile Info")
		bool bMoveing = false;
};

int32 AMissile::MissileCounter = 0;
