// Copyright 2019 Sanya Larsson All Rights Reserved.


#include "Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"

AGun::AGun(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon Mesh"));
	SetRootComponent(WeaponMesh);
}

void AGun::PrimaryAction()
{
	if (PrimaryActionSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, PrimaryActionSound, GetActorLocation());
	}
}

void AGun::SecondaryAction()
{
	if (SecondaryActionSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, SecondaryActionSound, GetActorLocation());
	}
}

bool AGun::GetPlayerLookLocationAndDirection(FVector& WorldLocation, FVector& WorldDirection) const
{
	APawn* Pawn = Cast<APawn>(GetOwner());
	if (!Pawn) return false;

	APlayerController* Controller = Cast<APlayerController>(Pawn->GetController());
	if (!Controller) return false;

	int32 ViewportSizeX, ViewportSizeY;
	Controller->GetViewportSize(ViewportSizeX, ViewportSizeY);
	return Controller->DeprojectScreenPositionToWorld(ViewportSizeX * 0.5f, ViewportSizeY * 0.5f, WorldLocation, WorldDirection);
}

FVector AGun::GetMuzzleLocation() const 
{
	if(WeaponMesh)
	{
		return WeaponMesh->GetSocketLocation(TEXT("Muzzle"));
	}

	return FVector::ZeroVector;
}

FRotator AGun::GetMuzzleRotation() const
{
	if (WeaponMesh)
	{
		return WeaponMesh->GetSocketRotation(TEXT("Muzzle"));
	}

	return FRotator::ZeroRotator;
}

void AGun::SetGunState(EGunState State)
{
	GunState = State;
}

EGunState AGun::GetGunState() const
{
	return GunState;
}

FLinearColor AGun::GetCrosshairColor() const
{
	return CrosshairColorsByState.FindRef(GunState);
}
