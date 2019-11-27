// Copyright 2019 Sanya Larsson All Rights Reserved.


#include "GravityGun.h"
#include "Components/PrimitiveComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
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

bool AGravityGun::PrimaryAction()
{
	bool bPushSuccess = PushGrabbedObject();
	if(!bPushSuccess)
	{
		bPushSuccess = PushObject();
	}

	if (bPushSuccess && PushSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, PushSound, GetMuzzleLocation());
	}
	else if (NoTargetSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, NoTargetSound, GetMuzzleLocation());
	}
	
	return bPushSuccess;
}

bool AGravityGun::SecondaryAction()
{
	bool bSuccess = false;
	if (!PhysicsHandle->GetGrabbedComponent())
	{
		bSuccess = GrabObject();

		if (bSuccess && GrabSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, GrabSound, GetMuzzleLocation());
		}
	}
	else
	{
		bSuccess = ReleaseGrabbedObject();

		if (bSuccess && ReleaseSound)
		{
			UGameplayStatics::PlaySoundAtLocation(this, ReleaseSound, GetMuzzleLocation());
		}
	}

	if (!bSuccess && NoTargetSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, NoTargetSound, GetMuzzleLocation());
	}

	return bSuccess;
}

void AGravityGun::GetGravityCenterAndDirection(FVector& Center, FVector& Direction) const
{
	const bool bSuccess = GetPlayerLookLocationAndDirection(Center, Direction);

	if(bSuccess)
	{
		Center += Direction * PlayerMuzzleOffset;
	}
	else
	{
		Center = GetMuzzleLocation() + GetMuzzleRotation().Vector() * MuzzleOffset;
		Direction = GetMuzzleRotation().Vector();
	}
}

bool AGravityGun::FindClosestObjectInReach(FHitResult& Hit) const
{
	FVector Location, Direction;
	GetGravityCenterAndDirection(Location, Direction);

	return GetWorld()->LineTraceSingleByChannel(
		Hit,
		Location,
		Location + Direction * MaxReachDistance,
		ECollisionChannel::ECC_Visibility,
		FCollisionQueryParams(FName(TEXT("")), false, GetOwner())
	);
}

bool AGravityGun::GrabObject() const
{
	FHitResult Hit;
	const bool bHitSomething = FindClosestObjectInReach(Hit);

	if (bHitSomething && PhysicsHandle && Hit.GetComponent() && Hit.GetComponent()->IsSimulatingPhysics())
	{
		PhysicsHandle->GrabComponentAtLocation(
			Hit.GetComponent(),
			NAME_None,
			Hit.GetComponent()->GetCenterOfMass()
		);

		return true;
	}

	return false;
}

bool AGravityGun::ReleaseGrabbedObject()
{
	if(PhysicsHandle->GetGrabbedComponent())
	{
		PhysicsHandle->ReleaseComponent();
		bGrabbedObjectAtGravityCenter = false;
		PhysicsHandle->bInterpolateTarget = true;

		return true;
	}
	
	return false;
}

bool AGravityGun::PushObject() const 
{
	FHitResult Hit;
	const bool bHitSomething = FindClosestObjectInReach(Hit);

	if (bHitSomething && Hit.GetComponent() && Hit.GetComponent()->IsSimulatingPhysics())
	{
		FVector Location, Direction;
		GetGravityCenterAndDirection(Location, Direction);

		const float Distance = FVector::Distance(Location, Hit.Location);
		const float PushForce = FMath::Lerp(MinPushForce, MaxPushForce, (MaxReachDistance - Distance) / MaxReachDistance);
		Hit.GetComponent()->AddImpulseAtLocation(Direction * PushForce, Hit.Location);

		return true;
	}

	return false;
}

bool AGravityGun::PushGrabbedObject()
{
	if(PhysicsHandle && PhysicsHandle->GetGrabbedComponent())
	{
		FVector Location, Direction;
		GetGravityCenterAndDirection(Location, Direction);

		const float Distance = FVector::Distance(Location, PhysicsHandle->GetGrabbedComponent()->GetCenterOfMass());
		const float PushForce = FMath::Lerp(MinPushForce, MaxPushForce, (MaxReachDistance - Distance) / MaxReachDistance);
		PhysicsHandle->GetGrabbedComponent()->AddImpulseAtLocation(Direction * PushForce, PhysicsHandle->GetGrabbedComponent()->GetCenterOfMass());
		ReleaseGrabbedObject();

		return true;
	}

	return false;
}

bool AGravityGun::PullGrabbedObject()
{
	if (PhysicsHandle && PhysicsHandle->GetGrabbedComponent())
	{
		FVector Location, Direction;
		GetGravityCenterAndDirection(Location, Direction);
		const float ComponentRadius = PhysicsHandle->GetGrabbedComponent()->Bounds.SphereRadius;
		
		if(!bGrabbedObjectAtGravityCenter)
		{
			const float Distance = FVector::Distance(Location + Direction * ComponentRadius, PhysicsHandle->GetGrabbedComponent()->GetCenterOfMass());
			const float PullSpeed = FMath::Lerp(MinPullSpeed, MaxPullSpeed, (MaxReachDistance - Distance) / MaxReachDistance);
			PhysicsHandle->SetInterpolationSpeed(PullSpeed);

			static constexpr float DISTANCE_TO_STOP_INTERPOLATION = 5.f;
			if (Distance < DISTANCE_TO_STOP_INTERPOLATION)
			{
				PhysicsHandle->bInterpolateTarget = false;
				bGrabbedObjectAtGravityCenter = true;
			}
		}
		
		PhysicsHandle->SetTargetLocation(Location + Direction * ComponentRadius);
		
		return true;
	}

	return false;
}
