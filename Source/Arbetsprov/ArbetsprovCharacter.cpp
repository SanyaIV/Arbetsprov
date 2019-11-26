// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ArbetsprovCharacter.h"
#include "ArbetsprovProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "XRMotionControllerBase.h" // for FXRMotionControllerBase::RightHandSourceId
#include "Weapons/Gun.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// AArbetsprovCharacter

AArbetsprovCharacter::AArbetsprovCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FP_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("FP_Camera"));
	FP_Camera->SetupAttachment(GetCapsuleComponent());
	FP_Camera->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
	FP_Camera->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	FP_Arms = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Arms"));
	FP_Arms->SetOnlyOwnerSee(true);
	FP_Arms->SetupAttachment(FP_Camera);
	FP_Arms->bCastDynamicShadow = false;
	FP_Arms->CastShadow = false;
	FP_Arms->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
	FP_Arms->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);
}

void AArbetsprovCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	FP_Gun = GetWorld()->SpawnActor<AGun>(GunBlueprint);
	if(FP_Gun)
	{
		FP_Gun->AttachToComponent(FP_Arms, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AArbetsprovCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind weapon events
	PlayerInputComponent->BindAction("WeaponPrimary", IE_Pressed, this, &AArbetsprovCharacter::OnWeaponPrimary);
	PlayerInputComponent->BindAction("WeaponSecondary", IE_Pressed, this, &AArbetsprovCharacter::OnWeaponSecondary);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AArbetsprovCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AArbetsprovCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AArbetsprovCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AArbetsprovCharacter::LookUpAtRate);
}

void AArbetsprovCharacter::OnWeaponPrimary()
{
	if (!FP_Gun) return;
	
	FP_Gun->PrimaryAction();

	// try and play a firing animation if specified
	if (FireAnimation != nullptr)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = FP_Arms->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}

void AArbetsprovCharacter::OnWeaponSecondary()
{
	if (!FP_Gun) return;
	
	FP_Gun->SecondaryAction();

	// try and play a firing animation if specified
	if (FireAnimation != nullptr)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = FP_Arms->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}

void AArbetsprovCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AArbetsprovCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AArbetsprovCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AArbetsprovCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}
