// Copyright 2019 Sanya Larsson All Rights Reserved.


#include "Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"

AGun::AGun(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon Mesh"));
	SetRootComponent(GunMesh);
}

bool AGun::PrimaryAction()
{
	return false;
}

bool AGun::SecondaryAction()
{
	return false;
}

AGun* AGun::PickUp(AActor* NewOwner)
{
	GunMesh->SetSimulatePhysics(false);
	SetOwner(NewOwner);
	SetGunState(EGunState::NoTarget);

	return this;
}

void AGun::Drop()
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	SetOwner(nullptr);
	GunMesh->SetSimulatePhysics(true);
	SetGunState(EGunState::Dropped);
}

bool AGun::GetPlayerLookLocationAndDirection(FVector& WorldLocation, FVector& WorldDirection) const
{
	const APawn* Pawn = Cast<APawn>(GetOwner());
	if (!Pawn) return false;

	const APlayerController* Controller = Cast<APlayerController>(Pawn->GetController());
	if (!Controller) return false;

	int32 ViewportSizeX, ViewportSizeY;
	Controller->GetViewportSize(ViewportSizeX, ViewportSizeY);
	return Controller->DeprojectScreenPositionToWorld(ViewportSizeX * 0.5f, ViewportSizeY * 0.5f, WorldLocation, WorldDirection);
}

FVector AGun::GetMuzzleLocation() const 
{
	if(GunMesh)
	{
		return GunMesh->GetSocketLocation(TEXT("Muzzle"));
	}

	return FVector::ZeroVector;
}

FRotator AGun::GetMuzzleRotation() const
{
	if (GunMesh)
	{
		return GunMesh->GetSocketRotation(TEXT("Muzzle"));
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

