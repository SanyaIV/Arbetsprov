// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ArbetsprovCharacter.h"
#include "ArbetsprovProjectile.h"
#include "ArbetsprovHUD.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Engine/World.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/GameplayStatics.h"
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
	FP_Camera->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f));
	FP_Camera->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	FP_Arms = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Arms"));
	FP_Arms->SetOnlyOwnerSee(true);
	FP_Arms->SetupAttachment(FP_Camera);
	FP_Arms->bCastDynamicShadow = false;
	FP_Arms->CastShadow = false;
	FP_Arms->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	FP_Arms->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));
}

void AArbetsprovCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	const APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if(PlayerController)
	{
		FP_HUD = Cast<AArbetsprovHUD>(PlayerController->GetHUD());
	}
}

void AArbetsprovCharacter::Tick(float DeltaTime)
{
	if(FP_HUD)
	{
		FLinearColor Color = DefaultCrosshairColor;
		if(FP_Gun)
		{
			Color = FP_Gun->GetCrosshairColor();
		}

		FP_HUD->SetCrosshairColor(Color);
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
	PlayerInputComponent->BindAction("PickUp", IE_Pressed, this, &AArbetsprovCharacter::PickUpGun);
	PlayerInputComponent->BindAction("Drop", IE_Pressed, this, &AArbetsprovCharacter::DropGun);

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
	
	const bool bSuccess = FP_Gun->PrimaryAction();

	// TODO: Refactor animations, should probably be decided by the gun instance?
	// try and play a firing animation if specified
	if (bSuccess && FireAnimation != nullptr)
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
	
	const bool bSuccess = FP_Gun->SecondaryAction();

	// TODO: Refactor animations, should probably be decided by the gun instance?
	// try and play a firing animation if specified
	if (bSuccess && FireAnimation != nullptr)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = FP_Arms->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}

void AArbetsprovCharacter::PickUpGun()
{
	FHitResult Hit;
	if(LineTraceSingleByChannelFromEyes(Hit, PickUpDistance, ECollisionChannel::ECC_Visibility))
	{
		AGun* Gun = Cast<AGun>(Hit.GetActor());
		if(Gun)
		{
			PickUpGun(Gun);
		}
	}
}

void AArbetsprovCharacter::PickUpGun(AGun* Gun)
{
	if(Gun)
	{
		// If the player already has a gun, drop it. Assumes the player can only have one gun at a time.
		if(FP_Gun)
		{
			DropGun();
		}

		FP_Gun = Gun->PickUp(this);
		FP_Gun->AttachToComponent(FP_Arms, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
	}
}

void AArbetsprovCharacter::DropGun()
{
	if(FP_Gun)
	{
		FP_Gun->Drop();
		FP_Gun = nullptr; // Assumes the player doesn't have any other guns than the one dropped.
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

bool AArbetsprovCharacter::LineTraceSingleByChannelFromEyes(FHitResult& OutHit, float DistanceToCheck, ECollisionChannel TraceChannel) const
{
	const APlayerController* PlayerController = Cast<APlayerController>(GetController());

	int32 ViewportSizeX, ViewportSizeY;
	PlayerController->GetViewportSize(ViewportSizeX, ViewportSizeY);

	FVector WorldLocation, WorldDirection;
	PlayerController->DeprojectScreenPositionToWorld(ViewportSizeX * 0.5f, ViewportSizeY * 0.5f, WorldLocation, WorldDirection);

	return GetWorld()->LineTraceSingleByChannel(
		OutHit,
		WorldLocation,
		WorldLocation + WorldDirection * DistanceToCheck,
		TraceChannel,
		FCollisionQueryParams(FName(TEXT("")), false, GetOwner())
	);
}
