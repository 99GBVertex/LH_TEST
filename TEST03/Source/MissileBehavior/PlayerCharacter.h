// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "PlayerCharacter.generated.h"

class AMissile;
/**
 * 
 */
UCLASS()
class MISSILEBEHAVIOR_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

protected:
	// ~Begin AActor
	virtual void PostActorCreated() override;
	virtual void Tick(float DeltaTime) override;
	// ~End AActor

public:
	//~ Begin BaseCharacter
	virtual void Init() override;
	virtual void Release() override;
	//~ End BaseCharacter	

	void SetupTargets();
	void ShootMissile();
	void ShootActivate(bool bActivate);

	UFUNCTION()
		void OnMissileDetacted(AMissile* missile);

protected:
	UPROPERTY(BlueprintReadOnly)
		TObjectPtr<ACameraActor> m_MainCam;
	UPROPERTY(EditAnywhere, Category = "Player Character Info")
		FNewLocation m_MainCamLocation = FNewLocation(-150.f, -150.f, 275.f);
	UPROPERTY(EditAnywhere, Category = "Player Character Info")
		FRotator m_MainCamRotation = FRotator(20.0f, 45.0f, 0.0f);
private:
	UPROPERTY(VisibleAnywhere, Category = "Player Base Info")
		TArray<AActor*> m_FoundTargetActors;
	UPROPERTY(EditAnywhere, Category = "Player Base Info")
		TSubclassOf<AMissile> m_MissileClass;
	UPROPERTY(VisibleAnywhere, Category = "Player Base Info")
		TArray<TObjectPtr<AMissile>> m_MissilePool;

	UPROPERTY(EditAnywhere, Category = "Player Base Info")
		float m_ShootMissileIntervalTime = 1.f;
	UPROPERTY(VisibleAnywhere, Category = "Player Base Info")
		float m_CurrentTime = 0.f;
	
	UPROPERTY(EditAnywhere, Category = "Player Base Info")
		int32 m_MaxMissileCount = 20;

	UPROPERTY(VisibleAnywhere, Category = "Player Base Info")
		int32 m_MissileActiveIndexIterator = 0;
	UPROPERTY(VisibleAnywhere, Category = "Player Base Info")
		int32 m_MissileDectiveIndexIterator = -1;

	UPROPERTY(VisibleAnywhere, Category = "Player Base Info")
		bool bShootAble = false;

	
};
