// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ArbetsprovCharacter.generated.h"

class UInputComponent;

UCLASS(config=Game)
class AArbetsprovCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AArbetsprovCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	float BaseLookUpRate;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	class UAnimMontage* FireAnimation;

	/** Returns FP_Arms subobject **/
	FORCEINLINE class USkeletalMeshComponent* GetFP_Arms() const { return FP_Arms; }
	/** Returns FP_Camera subobject **/
	FORCEINLINE class UCameraComponent* GetFP_Camera() const { return FP_Camera; }

protected:
	virtual void BeginPlay();

	/** Triggers primary action for the weapon. */
	void OnWeaponPrimary();

	/** Triggers secondary action for the weapon. */
	void OnWeaponSecondary();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

private:
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<class AGun> GunBlueprint;

	UPROPERTY(VisibleDefaultsOnly, Category = "Mesh")
	class USkeletalMeshComponent* FP_Arms;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FP_Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon", meta = (AllowPrivateAccess = "True"))
	class AGun* FP_Gun;
};

