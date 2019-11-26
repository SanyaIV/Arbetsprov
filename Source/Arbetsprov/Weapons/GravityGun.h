// Copyright 2019 Sanya Larsson All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/Gun.h"
#include "GravityGun.generated.h"

/**
 * 
 */
UCLASS()
class ARBETSPROV_API AGravityGun : public AGun
{
	GENERATED_BODY()
	
public:
	// Constructor
	AGravityGun(const FObjectInitializer& ObjectInitializer);

	/** Beam attack that pushes objects. */
	virtual void PrimaryAction() override;

	/** Pulls objects to the gravity gun. */
	virtual void SecondaryAction() override;

private:
	/** Physics Handle Component handles most of the grabbing/pulling functionality of the gravity gun. */
	UPROPERTY()
	class UPhysicsHandleComponent* PhysicsHandle = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float MaxReachDistance = 2000.f;
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float MaxPushForce = 5000.f;
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float MaxPullForce = 5000.f;

};
