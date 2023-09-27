// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"

ABaseCharacter::ABaseCharacter()
{

}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	Init();
}

void ABaseCharacter::Init()
{
	SetLocation(m_StartLocation);
	SetActorLocation(m_StartLocation.ToVector());
}

void ABaseCharacter::Release()
{
	Destroy();
}

FNewLocation ABaseCharacter::GetLocation() const
{
	return m_CharacterLocation;
}

void ABaseCharacter::SetLocation(const FNewLocation InValue) const
{
	m_CharacterLocation.SetLocation(InValue);
}

void ABaseCharacter::SetLocation(float InX, float InY, float InZ) const
{
	m_CharacterLocation.SetLocation(FNewLocation(InX, InY, InZ));
}

void ABaseCharacter::SetLocation(const FVector InValue) const
{
	m_CharacterLocation.SetLocation(FNewLocation(InValue.X, InValue.Y, InValue.Z));
}

void ABaseCharacter::AddLocation(const FNewLocation InValue) const
{
	m_CharacterLocation += InValue;
}


