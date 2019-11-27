// Copyright 2019 Sanya Larsson All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

UENUM()
enum class EGunState : uint8
{
	NoTarget,
	Target, // TODO: Rename to HasTarget
	Reloading,
	OutOfAmmo,
	Dropped,
	Holstered
};

/** Class representing a gun. Gun-like weapons inherit from this class. */
UCLASS()
class ARBETSPROV_API AGun : public AActor
{
	GENERATED_BODY()
	
public:
	/** Using FObjectInitializer form of construction because no-argument constructor leads to multiple default constructors for inheriting classes */
	AGun(const FObjectInitializer& ObjectInitializer);

	/**
	 * Method representing the primary action of the gun e.g. shooting a bullet.
	 * @return A boolean value representing whether the action could be carried out.
	 */
	UFUNCTION(BlueprintCallable, Category = "Actions")
	virtual bool PrimaryAction();

	/**
	 * Method representing the secondary action of the gun e.g. toggling semi/automatic.
	 * @return A boolean value representing whether the action could be carried out.
	 */
	UFUNCTION(BlueprintCallable, Category = "Actions")
	virtual bool SecondaryAction();

	// TODO: Create interface for objects that can be picked up / interacted with.

	/**
	 * Pick up the weapon and set Owner.
	 * @param NewOwner - The Actor that picked up the gun.
	 * @return Pointer to the gun that is being picked up.
	 * @warning Assumes NewOwner takes care of attachment.
	 */
	UFUNCTION(BlueprintCallable, Category = "Actions")
	AGun* PickUp(AActor* NewOwner);

	/** Method to drop the gun from the owner to the world. */
	UFUNCTION(BlueprintCallable, Category = "Actions")
	void Drop();

	/**
	 * Gets the current state of the gun.
	 * @return The enum representing the state.
	 */
	UFUNCTION(BlueprintCallable, Category = "State")
	EGunState GetGunState() const;

	/**
	 * Gets the color which the gun wishes the crosshair to be in.
	 * @return The color the crosshair should be.
	 */
	UFUNCTION(BlueprintCallable, Category = "Crosshair")
	FLinearColor GetCrosshairColor() const;

protected:
	/**
	 * Attempts to find the location and direction that the player is looking based on which pawn owns the weapon.
	 * @param WorldLocation - Location in the world that the center of the player's viewport is.
	 * @param WorldDirection - Direction in the world that the center of the player's viewport is looking.
	 * @return Whether or not it was successful in finding a location and direction.
	 */
	UFUNCTION(BlueprintCallable, Category = "Viewport")
	bool GetPlayerLookLocationAndDirection(FVector& WorldLocation, FVector& WorldDirection) const;

	/**
	 * Gets the location of the muzzle on the gun in world space.
	 * @return The location of the muzzle on the gun in world space or ZeroVector if no mesh.
	 */
	UFUNCTION(BlueprintCallable, Category = "Muzzle")
	FVector GetMuzzleLocation() const;

	/**
	 * Gets the rotation of the muzzle on the gun in world space.
	 * @return The location of the muzzle on the gun in world space or ZeroRotator if no mesh.
	 */
	UFUNCTION(BlueprintCallable, Category = "Muzzle")
	FRotator GetMuzzleRotation() const;

	/**
	 * Sets the state of the gun.
	 * @param State - The state to set to.
	 */
	UFUNCTION(BlueprintCallable, Category = "State")
	void SetGunState(EGunState State);

private:
	UPROPERTY(VisibleDefaultsOnly, Category = "Mesh")
	USkeletalMeshComponent* GunMesh = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "State")
	EGunState GunState = EGunState::NoTarget;

	UPROPERTY(EditDefaultsOnly, Category = "HUD")
	TMap<EGunState, FLinearColor> CrosshairColorsByState;
};
