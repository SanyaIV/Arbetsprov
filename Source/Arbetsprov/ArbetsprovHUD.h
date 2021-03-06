// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ArbetsprovHUD.generated.h"

UCLASS()
class AArbetsprovHUD : public AHUD
{
	GENERATED_BODY()

public:
	AArbetsprovHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

	/** Set the color of the crosshair */
	void SetCrosshairColor(FLinearColor Color);

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

	/** Color for the crosshair */
	FLinearColor CrosshairColor = FLinearColor::White;

};
