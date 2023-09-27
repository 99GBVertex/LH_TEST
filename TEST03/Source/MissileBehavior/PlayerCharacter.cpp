// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "MonsterCharacter.h"
#include "Missile.h"

#include "Camera/CameraActor.h"

#define IsEnd(x, y, max) (x < y && x + 1 == y) && ((x + 1 == max) && y != -1)

APlayerCharacter::APlayerCharacter()
{
	if (!HasAnyFlags(RF_ClassDefaultObject))
	{
		if (m_MissileClass == nullptr) {
			// default Missile
			static ConstructorHelpers::FClassFinder<AMissile> MissileFinder(TEXT("/Game/AMissile.AMissile_C"));
			if (MissileFinder.Succeeded())
			{
				m_MissileClass = MissileFinder.Class;
			}
		}
	}
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void APlayerCharacter::PostActorCreated()
{
	Super::PostActorCreated();
}

void APlayerCharacter::Init()
{
	Super::Init();

	if (GetWorld()) {
		m_MainCam = GetWorld()->SpawnActor<ACameraActor>(ACameraActor::StaticClass());
		m_MainCam->SetActorLocationAndRotation(m_MainCamLocation.ToVector(), m_MainCamRotation);
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetViewTarget(m_MainCam);
	}

	if (m_FoundTargetActors.Num() == 0) {
		SetupTargets();
	}

	m_MissilePool.Empty();
	m_MissilePool.Reserve(m_MaxMissileCount);
	for (int idx = 0; idx < m_MaxMissileCount; idx++) {	
		TObjectPtr<AMissile> curMissile = nullptr;
		FTransform missileSpawnTransform(FRotator::ZeroRotator, GetLocation().ToVector(), FVector::ZeroVector);
		curMissile = GetWorld()->SpawnActor<AMissile>(m_MissileClass, missileSpawnTransform);
		curMissile->OnMissileDetacted.AddUniqueDynamic(this, &ThisClass::OnMissileDetacted);
		curMissile->SetActorHiddenInGame(true);
		m_MissilePool.Emplace(curMissile);
	}
}

void APlayerCharacter::Release()
{
	for (const TObjectPtr<AMissile>& missile : m_MissilePool) {
		if (IsValid(missile)) {
			missile->Release();
		}
	}
	m_MissilePool.Empty();
	m_MainCam->Destroy();
	m_MainCam = nullptr;
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bShootAble) {
		return;
	}

	if (m_CurrentTime == 0 || m_CurrentTime >= m_ShootMissileIntervalTime) {
		m_ShootMissileIntervalTime *= 100;
		m_CurrentTime = FMath::Frac(m_ShootMissileIntervalTime);
		m_ShootMissileIntervalTime *= 0.01;
		ShootMissile();
	}
	m_CurrentTime += DeltaTime;
}

void APlayerCharacter::SetupTargets()
{
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName(TEXT("Monster")), m_FoundTargetActors);
}

void APlayerCharacter::ShootMissile()
{
	//1. �� ����Ʈ ��Ͽ��� Ÿ���� ã�´�.
	// + �ٸ� ������ �ְ� �̹� �������� ����̶�� ����. Ÿ���� �����ϸ� �ݷ��Ѵ�.
	TObjectPtr<const AMonsterCharacter> target = nullptr;
	for (const AActor* actor : m_FoundTargetActors)
	{
		if (!IsValid(actor)) continue;
		if (actor->IsA<AMonsterCharacter>())
		{
			const AMonsterCharacter* curTarget = Cast<AMonsterCharacter>(actor);
			if (curTarget)
			{
				if (curTarget->HasFollower())
				{
					if (m_FoundTargetActors.Last() == actor) {
						target = curTarget;
						break;
					}
					continue;
				}
				else if (curTarget->GetMonsterState() != EMonsterState::Move) {
					continue;
				}
				else {
					target = curTarget;
					break;
				}
			}
		}
	}

	if(!target) return;

	// 2. �� ����Pool���� ������ �°� �߻� ������ �Ѵ�. �� ���� ������ �ݷ��Ѵ�.
	const int32 ActivateRemainderIdx = m_MissileActiveIndexIterator % m_MaxMissileCount;
	const int32 DectiveRemainderIdx = m_MissileDectiveIndexIterator % m_MaxMissileCount;
	if (IsEnd(ActivateRemainderIdx, DectiveRemainderIdx, m_MaxMissileCount))
	{
		return;
	}

	TObjectPtr<AMissile> curMissile = nullptr;
	int32 missilePoolIdx = (m_MissileActiveIndexIterator % m_MaxMissileCount);	
	if (missilePoolIdx >= m_MissilePool.Num()) {
		return;
	}
	curMissile = m_MissilePool[missilePoolIdx];	
	if (!curMissile) {
		return;
	}

	m_MissileActiveIndexIterator+=1;
	curMissile->SetActorHiddenInGame(false);	

	// 3. ���� ��� Follower�� �� �ڽ����� ��� �� ���� ������ ��û.
	if (target) {
		curMissile->SetMissile(this, target);
	}
}

void APlayerCharacter::ShootActivate(bool bActivate)
{
	bShootAble = bActivate;
}

void APlayerCharacter::OnMissileDetacted(AMissile* missile)
{
	if(!IsValid(missile))
		return;

	AActor* parents = Cast<AActor>(this);	
	missile->SetActorRelativeLocation(parents->GetActorLocation());
	missile->SetActorHiddenInGame(true);
	m_MissileDectiveIndexIterator+=1;
}
