// Copyright 2019 Sanya Larsson All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

/** Class representing a gun. Gun-like weapons inherit from this class. */
UCLASS()
class ARBETSPROV_API AGun : public AActor
{
	GENERATED_BODY()
	
public:
	// Constructor
	AGun(const FObjectInitializer& ObjectInitializer);

	/** Method representing the primary action of the gun e.g. shooting a bullet. */
	UFUNCTION(BlueprintCallable, Category = "Actions")
	virtual void PrimaryAction();
	/** Method representing the secondary action of the gun e.g. toggling semi/automatic. */
	UFUNCTION(BlueprintCallable, Category = "Actions")
	virtual void SecondaryAction();

private:
	UPROPERTY(VisibleDefaultsOnly, Category = "Mesh")
	USkeletalMeshComponent* WeaponMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	class USoundBase* PrimaryActionSound;
	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	class USoundBase* SecondaryActionSound;
};
