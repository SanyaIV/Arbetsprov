// Copyright 2019 Sanya Larsson All Rights Reserved.


#include "GravityGun.h"
#include "Components/PrimitiveComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

AGravityGun::AGravityGun(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	PhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("Physics Handle")); 
}

void AGravityGun::Tick(float DeltaTime)
{
	FHitResult Hit;
	if(FindClosestObjectInReach(Hit) && Hit.GetComponent()->IsSimulatingPhysics())
	{
		SetGunState(EGunState::Target);
	}
	else
	{
		SetGunState(EGunState::NoTarget);
	}

	PullGrabbedObject();
}

void AGravityGun::PrimaryAction()
{
	Super::PrimaryAction();

	ReleaseGrabbedObject();
	PushObject();
}

void AGravityGun::SecondaryAction()
{
	Super::SecondaryAction();

	if(!PhysicsHandle->GetGrabbedComponent())
	{
		GrabObject();
	}
	else
	{
		ReleaseGrabbedObject();
	}
}

FVector AGravityGun::GetGravityCenter() const 
{
	return GetMuzzleLocation() + GetMuzzleRotation().Vector() * MuzzleOffset;
}

bool AGravityGun::FindClosestObjectInReach(FHitResult& Hit) const
{
	FVector Location, Direction;
	bool bSuccess = GetPlayerLookLocationAndDirection(Location, Direction);

	// Fall back to using the Gun's POV for lince trace if we can't find the Player's POV.
	// Player's POV is prefered since the Gun might be angled away from the center slightly.
	if(!bSuccess)
	{
		Location = GetGravityCenter();
		Direction = GetMuzzleRotation().Vector();
	}

	return GetWorld()->LineTraceSingleByChannel(
		Hit,
		Location,
		Location + Direction * MaxReachDistance,
		ECollisionChannel::ECC_Visibility,
		FCollisionQueryParams(FName(TEXT("")), false, GetOwner())
	);
}

void AGravityGun::GrabObject() const
{
	FHitResult Hit;
	bool bHitSomething = FindClosestObjectInReach(Hit);

	if (bHitSomething && PhysicsHandle && Hit.GetComponent() && Hit.GetComponent()->IsSimulatingPhysics())
	{
		PhysicsHandle->GrabComponentAtLocation(
			Hit.GetComponent(),
			NAME_None,
			Hit.GetComponent()->GetCenterOfMass()
		);
	}
}

void AGravityGun::ReleaseGrabbedObject() const 
{
	PhysicsHandle->ReleaseComponent();
}

void AGravityGun::PushObject() const 
{
	FHitResult Hit;
	bool bHitSomething = FindClosestObjectInReach(Hit);

	if (bHitSomething && Hit.GetComponent() && Hit.GetComponent()->IsSimulatingPhysics())
	{
		float Distance = FVector::Distance(GetGravityCenter(), Hit.Location);
		float PushForce = FMath::Lerp(MinPushForce, MaxPushForce, (MaxReachDistance - Distance) / MaxReachDistance);
		Hit.GetComponent()->AddImpulseAtLocation(GetMuzzleRotation().Vector() * PushForce, Hit.Location);
	}
}

void AGravityGun::PullGrabbedObject() const
{
	if (!PhysicsHandle) return;
	if (PhysicsHandle->GetGrabbedComponent())
	{
		float Distance = FVector::Distance(GetGravityCenter(), PhysicsHandle->GetGrabbedComponent()->GetComponentLocation());
		float PullSpeed = FMath::Lerp(MinPullSpeed, MaxPullSpeed, (MaxReachDistance - Distance) / MaxReachDistance);
		PhysicsHandle->SetInterpolationSpeed(PullSpeed);

		float ComponentRadius = PhysicsHandle->GetGrabbedComponent()->Bounds.SphereRadius;
		PhysicsHandle->SetTargetLocation(GetGravityCenter() + GetMuzzleRotation().Vector() * ComponentRadius);
	}
}
