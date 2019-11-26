// Copyright 2019 Sanya Larsson All Rights Reserved.


#include "GravityGun.h"
#include "Components/SkeletalMeshComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

AGravityGun::AGravityGun(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("Physics Handle")); 
}

void AGravityGun::PrimaryAction()
{
	Super::PrimaryAction();

	// TODO: Implement push logic.
}

void AGravityGun::SecondaryAction()
{
	Super::SecondaryAction();

	// TODO: Implement pull logic.
}
