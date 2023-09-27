// Fill out your copyright notice in the Description page of Project Settings.


#include "Missile.h"
#include "BaseCharacter.h"
#include "MonsterCharacter.h"
#include "Kismet/KismetMathLibrary.h"

AMissile::AMissile()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void AMissile::BeginPlay()
{
	Super::BeginPlay();
	AMissile::MissileCounter += 1;
	m_MissileId = AMissile::MissileCounter;
}

// Called every frame
void AMissile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!bMoveing) 
		return;
	UpdateTarget(DeltaTime);	
}

void AMissile::SetMissile(
	const TObjectPtr<const ABaseCharacter>& InShooter,
	const TObjectPtr<const ABaseCharacter>& InTarget)
{
	if (!IsValid(InShooter)) {
		return;
	}
	if (bMoveing) {
		return;
	}
	m_Start = InShooter;
	m_CurrentLocation = m_Start->GetLocation();

	if (IsValid(InTarget)) {
		m_Target = InTarget;
		m_DestLocation = m_Target->GetLocation();
		m_Direction = (m_DestLocation - m_CurrentLocation).GetNormal();
				
		m_Target->SetFollower(this);
	}
	else {
		//delete
	}
	SetActorRotation(m_Direction.ToRotator());

	bMoveing = true;
}

void AMissile::UpdateTarget(float DeltaTime)
{
	if (IsValid(m_Target)) {
		m_DestLocation = m_Target->GetLocation();
	}

	// Compute and set current location
	m_PrevLocation = m_CurrentLocation;
	m_CurrentLocation.SetLocation(GetActorLocation());

	SetActorLocation(GetActorLocation() + (m_Direction.ToVector() * m_Velocity * DeltaTime));
	SetActorRotation(m_Direction.ToRotator());

	m_Direction = (m_DestLocation - m_CurrentLocation).GetNormal();
	
	const float m_Distance = m_CurrentLocation.GetDistanceToSelf(m_DestLocation);
	bool bCollision = false;
	const float checkLimit = 2 * (m_HitRadius + 15.f);
	if (m_Distance < checkLimit) {
		bCollision = m_CurrentLocation.IsCircleCollision(m_HitRadius, 15.f, m_DestLocation);
		if (!bCollision) {
			FNewLocation diffPrev = m_PrevLocation - m_CurrentLocation;
			diffPrev *= 0.5f;
			FNewLocation halfBoxSize = diffPrev.GetAbs();
			FBox collisionBox = FBox::BuildAABB(m_CurrentLocation.ToVector(), halfBoxSize.ToVector());
			bCollision = FMath::PointBoxIntersection(m_DestLocation.ToVector(), collisionBox);
		}
	}

	if (bCollision) {
		StopMissile();
	}
}

void AMissile::Release()
{
	SetActorHiddenInGame(true);
	m_Start = nullptr;
	m_Target = nullptr;
	bMoveing = false;

}

void AMissile::StopMissile()
{
	bMoveing = false;

	if (IsValid(m_Target))
	{
		TObjectPtr<AMonsterCharacter> targetMonster = Cast<AMonsterCharacter>(m_Target);
		if (IsValid(targetMonster)) {
			targetMonster->OnHitted();
		}
		else {
			ABaseCharacter* delTarget(m_Target);
			if (delTarget) {
				delTarget->Release();
			}
		}
	}
	if (OnMissileDetacted.IsBound()) {
		OnMissileDetacted.Broadcast(this);
	}
}
