// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NewLocation.generated.h"

USTRUCT(BlueprintType)
struct MISSILEBEHAVIOR_API FNewLocation
{
	GENERATED_BODY()
public:
	FNewLocation() {
		m_PosX = 0.f;
		m_PosY = 0.f;
		m_PosZ = 0.f;
	}
	FNewLocation(float InX, float InY, float InZ) :
		m_PosX(InX), m_PosY(InY), m_PosZ(InZ) {}
	~FNewLocation() {}

	FNewLocation operator+(const FNewLocation& rhs) {
		return FNewLocation(m_PosX + rhs.m_PosX, m_PosY + rhs.m_PosY, m_PosZ + rhs.m_PosZ);
	}

	FNewLocation& operator+=(const FNewLocation& rhs) {
		m_PosX += rhs.m_PosX;
		m_PosY += rhs.m_PosY;
		m_PosZ += rhs.m_PosZ;
		return *this;
	}

	FNewLocation operator-(const FNewLocation& rhs) {
		return FNewLocation(m_PosX - rhs.m_PosX, m_PosY - rhs.m_PosY, m_PosZ - rhs.m_PosZ);
	}


	FNewLocation operator*(float rate) {
		return FNewLocation(m_PosX * rate, m_PosY * rate, m_PosZ * rate);
	}
	FNewLocation& operator*=(float rate) {
		m_PosX *= rate;
		m_PosY *= rate;
		m_PosZ *= rate;
		return *this;
	}

	FNewLocation GetLocation() {
		return FNewLocation(m_PosX, m_PosY, m_PosZ);
	}

	void SetLocation(const FVector& InValue) {
		m_PosX = InValue.X;
		m_PosY = InValue.Y;
		m_PosZ = InValue.Z;
	}
	void SetLocation(const FNewLocation& InValue) {
		m_PosX = InValue.m_PosX;
		m_PosY = InValue.m_PosY;
		m_PosZ = InValue.m_PosZ;
	}

	FNewLocation GetNormal(float Tolerance = UE_SMALL_NUMBER) {
		const float SquareSum = m_PosX * m_PosX + m_PosY * m_PosY + m_PosZ * m_PosZ;

		if (SquareSum == 1.f) {
			return *this;
		}
		else if (SquareSum < Tolerance) {
			return FNewLocation(0.f, 0.f, 0.f);
		}
		const float Scale = (float)FMath::InvSqrt(SquareSum);
		return FNewLocation(m_PosX * Scale, m_PosY * Scale, m_PosZ * Scale);
	}
	FNewLocation GetAbs()
	{
		return FNewLocation(FMath::Abs(m_PosX), FMath::Abs(m_PosY), FMath::Abs(m_PosZ));
	}

	float GetDistanceToSelf(const FNewLocation& InValue) {
		FNewLocation diff = GetLocation().operator-(InValue);
		float Distance = FMath::Sqrt(diff.GetX() * diff.GetX() + diff.GetY() * diff.GetY() + diff.GetZ() * diff.GetZ());
		return Distance;
	}

	float DotProduct(const FNewLocation& InValue) {
		return FMath::Sqrt(GetX() * InValue.m_PosX + GetY() * InValue.m_PosY + GetZ() * InValue.m_PosZ);
	}

	bool IsCircleCollision(float InThisHitRadius, float InTargetRadius, const FNewLocation& InValue) {
		if (InThisHitRadius + InTargetRadius >= GetDistanceToSelf(InValue)) {
			return true;
		}
		return false;
	}

	FVector ToVector() {
		return FVector(m_PosX, m_PosY, m_PosZ);
	}
	FVector ToVector() const {
		return FVector(m_PosX, m_PosY, m_PosZ);
	}
	FRotator ToRotator() {
		FRotator outRotator;
		outRotator.Yaw = FMath::Atan2(m_PosY, m_PosX) * 180.0f / PI;
		outRotator.Pitch = FMath::Asin(m_PosZ) * 180.0f / PI;
		outRotator.Roll = 0.0f;
		return outRotator;
	}
	FRotator ToRotator() const {
		FRotator outRotator;
		outRotator.Yaw = FMath::Atan2(m_PosY, m_PosX) * 180.0f / PI;
		outRotator.Pitch = FMath::Asin(m_PosZ) * 180.0f / PI;
		outRotator.Roll = 0.0f;
		return outRotator;
	}
		
	float GetX() { return m_PosX; }
	float GetY() { return m_PosY; }
	float GetZ() { return m_PosZ; }

	UPROPERTY(EditAnywhere) float m_PosX = 0.f;
	UPROPERTY(EditAnywhere) float m_PosY = 0.f;
	UPROPERTY(EditAnywhere) float m_PosZ = 0.f;
};