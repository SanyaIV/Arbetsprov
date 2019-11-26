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