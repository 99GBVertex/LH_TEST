// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NewLocation.h"
#include "BaseCharacter.generated.h"

UCLASS()
class MISSILEBEHAVIOR_API ABaseCharacter : public AActor
{
	GENERATED_BODY()
	
public:
	ABaseCharacter();

	// ~Begin AActor
	virtual void BeginPlay() override;
	// ~End AActor
	
	virtual void Init();
	virtual void Release();

	FNewLocation GetLocation() const;
	void SetLocation(const FNewLocation InValue) const;
	void SetLocation(float InX, float InY, float InZ) const;
	void SetLocation(const FVector InValue) const;
	void AddLocation(const FNewLocation InValue) const;

	void SetFollower(const TObjectPtr<const AActor>& InFollower) const { m_Follower = InFollower; }
	bool HasFollower() const { return IsValid(m_Follower); }

protected:
	UPROPERTY(EditAnywhere, Category = "Base Character Info")
		FNewLocation m_StartLocation = FNewLocation(0.f, 0.f, 0.f);
	mutable FNewLocation m_CharacterLocation = FNewLocation(0.f, 0.f, 0.f);
	mutable TObjectPtr<const AActor> m_LookingTarget = nullptr;
	mutable TObjectPtr<const AActor> m_Follower = nullptr;
};
