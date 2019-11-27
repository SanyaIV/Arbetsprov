// Copyright 2019 Sanya Larsson All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

UENUM()
enum class EGunState : uint8
{
	NoTarget,
	Target,
	Reloading,
	OutOfAmmo
};

/** Class representing a gun. Gun-like weapons inherit from this class. */
UCLASS()
class ARBETSPROV_API AGun : public AActor
{
	GENERATED_BODY()
	
public:
	/** Using FObjectInitializer form of construction because no-argument constructor leads to multiple default constructors for inheriting classes */
	AGun(const FObjectInitializer& ObjectInitializer);

	/** Method representing the primary action of the gun e.g. shooting a bullet. */
	UFUNCTION(BlueprintCallable, Category = "Actions")
	virtual void PrimaryAction();
	/** Method representing the secondary action of the gun e.g. toggling semi/automatic. */
	UFUNCTION(BlueprintCallable, Category = "Actions")
	virtual void SecondaryAction();

	UFUNCTION(BlueprintCallable, Category = "State")
	EGunState GetGunState() const;

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
	* @return The location of the muzzle on the gun in world space.
	*/
	UFUNCTION(BlueprintCallable, Category = "Muzzle")
	FVector GetMuzzleLocation() const;

	/**
	* Gets the rotation of the muzzle on the gun in world space.
	* @return The location of the muzzle on the gun in world space.
	*/
	UFUNCTION(BlueprintCallable, Category = "Muzzle")
	FRotator GetMuzzleRotation() const;

	UFUNCTION(BlueprintCallable, Category = "State")
	void SetGunState(EGunState State);

private:
	UPROPERTY(VisibleDefaultsOnly, Category = "Mesh")
	USkeletalMeshComponent* WeaponMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	class USoundBase* PrimaryActionSound;
	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	class USoundBase* SecondaryActionSound;

	EGunState GunState = EGunState::NoTarget;

	UPROPERTY(EditDefaultsOnly, Category = "HUD")
	TMap<EGunState, FLinearColor> CrosshairColorsByState;
};
