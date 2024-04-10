// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "TalesPlayerCameraManager.generated.h"

/**
 * @brief ATalesPlayerCameraManager
 */
UCLASS()
class D_TALESRUNNER_REP_API ATalesPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

public:
	ATalesPlayerCameraManager();

	virtual void UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime) override;
private:
	UPROPERTY(EditDefaultsOnly)
	float CrouchBlendDuration = .5f;
	float CrouchBlendTime;
};
