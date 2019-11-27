// Copyright 2019 Sanya Larsson All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/Gun.h"
#include "GravityGun.generated.h"

/**
 * Representa a Gravity Gun, inheriting from the Gun class.
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
	/** 
	* The location of the muzzle with applied offset in the gun's forward direction.
	* @return The location of the muzzle with applied offset in the gun's forward direction.
	*/
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	FVector GetGravityCenter() const;

	/**
	* Linetrace to find the closest visible object in line-of-sight.
	* @param Hit - Upon return will contain the result of the linetrace.
	* @return Whether something was hit by the linetrace or not.
	*/
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	bool FindClosestObjectInReach(FHitResult& Hit) const;

	/** Grab the closest object. */
	UFUNCTION(BlueprintCallable, Category = "Action")
	void GrabObject() const;

	/** Release any grabbed object */
	UFUNCTION(BlueprintCallable, Category = "Action")
	void ReleaseGrabbedObject() const;

	/** Push the closest object. */
	UFUNCTION(BlueprintCallable, Category = "Action")
	void PushObject() const;

	/** Pull any grabbed object towards the gravity center of the gravity gun, offsetting for object radius. */
	UFUNCTION(BlueprintCallable, Category = "Action")
	void PullGrabbedObject() const;

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
