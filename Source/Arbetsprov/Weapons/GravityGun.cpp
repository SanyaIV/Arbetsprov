// Copyright 2019 Sanya Larsson All Rights Reserved.


#include "GravityGun.h"
#include "Components/PrimitiveComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

AGravityGun::AGravityGun(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	PhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("Physics Handle")); 
}

void AGravityGun::Tick(float DeltaTime)
{
	PullIfGrabbing();
}

void AGravityGun::PrimaryAction()
{
	Super::PrimaryAction();

	Release();
	Push();
}

void AGravityGun::SecondaryAction()
{
	Super::SecondaryAction();

	if(!PhysicsHandle->GetGrabbedComponent())
	{
		Grab();
	}
	else
	{
		Release();
	}
}

FVector AGravityGun::GetGravityCenter() const 
{
	return GetMuzzleLocation() + GetMuzzleRotation().Vector() * MuzzleOffset;
}

bool AGravityGun::FindFirstObjectInReach(FHitResult& Hit) const
{
	return GetWorld()->LineTraceSingleByChannel(
		Hit,
		GetGravityCenter(),
		GetGravityCenter() + GetMuzzleRotation().Vector() * MaxReachDistance,
		ECollisionChannel::ECC_Visibility,
		FCollisionQueryParams(FName(TEXT("")), false, GetOwner())
	);
}

void AGravityGun::Grab() const
{
	FHitResult Hit;
	bool bHitSomething = FindFirstObjectInReach(Hit);

	if (bHitSomething && PhysicsHandle && Hit.GetComponent() && Hit.GetComponent()->IsSimulatingPhysics())
	{
		PhysicsHandle->GrabComponentAtLocation(
			Hit.GetComponent(),
			NAME_None,
			Hit.GetComponent()->GetCenterOfMass()
		);
	}
}

void AGravityGun::Release() const 
{
	PhysicsHandle->ReleaseComponent();
}

void AGravityGun::Push() const 
{
	FHitResult Hit;
	bool bHitSomething = FindFirstObjectInReach(Hit);

	if (bHitSomething && Hit.GetComponent() && Hit.GetComponent()->IsSimulatingPhysics())
	{
		float Distance = FVector::Distance(GetGravityCenter(), Hit.Location);
		float PushForce = FMath::Lerp(0.f, MaxPushForce, (MaxReachDistance - Distance) / MaxReachDistance);
		UE_LOG(LogTemp, Warning, TEXT("%f"), PushForce)
		Hit.GetComponent()->AddImpulseAtLocation(GetMuzzleRotation().Vector() * PushForce, Hit.Location);
	}
}

void AGravityGun::PullIfGrabbing() const
{
	if (!PhysicsHandle) return;
	if (PhysicsHandle->GetGrabbedComponent())
	{
		float ComponentRadius = PhysicsHandle->GetGrabbedComponent()->Bounds.SphereRadius;
		PhysicsHandle->SetTargetLocation(GetGravityCenter() + GetMuzzleRotation().Vector() * ComponentRadius);
	}
}
