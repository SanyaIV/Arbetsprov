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

	/** Beam attack that pushes objects. */
	virtual bool PrimaryAction() override;

	/** Pulls objects to the gravity gun. */
	virtual bool SecondaryAction() override;

private:
	/** 
	 * The location of the center of the gravity effect and its direction.
	 * First tries to get based on Player's POV and falls back to Gun's POV.
	 * @param Center - The location of the center of the gravity effect.
	 * @param Direction - The direction in which the gravity effect is facing.
	 */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void GetGravityCenterAndDirection(FVector& Center, FVector& Direction) const;

	/**
	 * Linetrace to find the closest visible object in line-of-sight.
	 * @param Hit - Upon return will contain the result of the linetrace.
	 * @return Whether something was hit by the linetrace or not.
	 */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	bool FindClosestObjectInReach(FHitResult& Hit) const;

	/**
	 * Grab the closest object.
	 * @return Whether or not an object was grabbed.
	 */
	UFUNCTION(BlueprintCallable, Category = "Action")
	bool GrabObject() const;

	/** 
	 * Release any grabbed object
	 * @return Whether or not a grabbed object was released.
	 */
	UFUNCTION(BlueprintCallable, Category = "Action")
	bool ReleaseGrabbedObject();

	/**
	 * Push the closest object.
	 * @return Whether or not an object was pushed.
	 */
	UFUNCTION(BlueprintCallable, Category = "Action")
	bool PushObject() const;

	/**
	 * Push the grabbed object.
	 * @return Whether or not a grabbed object was pushed.
	 */
	UFUNCTION(BlueprintCallable, Category = "Action")
	bool PushGrabbedObject();

	/**
	 * Pull any grabbed object towards the gravity center of the gravity gun, offsetting for object radius.
	 * @return Whether or not a grabbed object was pulled.
	 */
	UFUNCTION(BlueprintCallable, Category = "Action")
	bool PullGrabbedObject();

	/** Physics Handle Component handles most of the grabbing/pulling functionality of the gravity gun. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Physics Handle", meta = (AllowPrivateAccess = "True"))
	class UPhysicsHandleComponent* PhysicsHandle = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float MaxReachDistance = 2500.f;
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float MinPushForce = 0.f;
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float MaxPushForce = 750000.f;
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float MinPullSpeed = 0.1f;
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float MaxPullSpeed = 15.f;
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float PlayerMuzzleOffset = 100.f;
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float MuzzleOffset = 50.f;

	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	USoundBase* PushSound;
	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	USoundBase* GrabSound;
	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	USoundBase* ReleaseSound;
	UPROPERTY(EditDefaultsOnly, Category = "Audio")
	USoundBase* NoTargetSound;

	bool bGrabbedObjectAtGravityCenter = false;
};
