// Copyright 2019 Sanya Larsson All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/Gun.h"
#include "GravityGun.generated.h"

/**
 * Representa a Gravity Gun, based on the Gun class.
 */
UCLASS()
class ARBETSPROV_API AGravityGun : public AGun
{
	GENERATED_BODY()
	
public:
	/** Using FObjectInitializer form of construction because no-argument constructor leads to multiple default super constructors. */
	AGravityGun(const FObjectInitializer& ObjectInitializer);

	/** Tick function */
	virtual void Tick(float DeltaTime) override;

	/** Beam attack that pushes objects. Calls Super first. */
	virtual void PrimaryAction() override;

	/** Pulls objects to the gravity gun. Calls Super first. */
	virtual void SecondaryAction() override;

private:
	/** Returns the location of the muzzle + offset in its forward direction */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	FVector GetGravityCenter() const;

	/**
	* Linetrace to find the first visible object.
	* @param Hit - Upon return will contain the Hit result of the linetrace.
	* @return Whether something was hit by the linetrace or not.
	*/
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	bool FindFirstObjectInReach(FHitResult& Hit) const;

	/** Grab the closest object. */
	UFUNCTION(BlueprintCallable, Category = "Action")
	void Grab() const;

	/** Release any grabbed object */
	UFUNCTION(BlueprintCallable, Category = "Action")
	void Release() const;

	/** Push the closest object. */
	UFUNCTION(BlueprintCallable, Category = "Action")
	void Push() const;

	/** Pull any grabbed object towards the gravity center of the gravity gun, offsetting for object radius. */
	UFUNCTION(BlueprintCallable, Category = "Action")
	void PullIfGrabbing() const;

	/** Physics Handle Component handles most of the grabbing/pulling functionality of the gravity gun. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Physics Handle", meta = (AllowPrivateAccess = "True"))
	class UPhysicsHandleComponent* PhysicsHandle = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float MaxReachDistance = 2000.f;
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float MinPushForce = 0.f;
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float MaxPushForce = 750000.f;
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float MinPullSpeed = 1.f;
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float MaxPullSpeed = 50.f;
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float MuzzleOffset = 50.f;

};
