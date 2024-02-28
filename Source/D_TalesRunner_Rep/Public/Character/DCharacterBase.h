// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/TimelineComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DCharacterBase.generated.h"

class UCameraComponent;
class UInputMappingContext;
class USpringArmComponent;
class UInputAction;
class UNiagaraComponent;
struct FInputActionInstance;

UCLASS()
class D_TALESRUNNER_REP_API ADCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	ADCharacterBase();

protected:
	/* Enhanced Input, PCInputMapping using for PC Game */
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* PCInputMapping;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* Input_Move;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* Input_Jump;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* Input_Sprint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<USpringArmComponent> SpringArmComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UCameraComponent>    CameraComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<USkeletalMeshComponent> JetPackComp;

	// Sprint related Comp
	// Timeline Comp to control FOV change, when sprint
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sprint|VFX")
	TObjectPtr<UTimelineComponent> SprintTimeLineComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sprint|VFX")
	TObjectPtr<UNiagaraComponent> SprintLineNiagaraComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "JetPack|VFX")
	TObjectPtr<UNiagaraComponent> JetPackThrusterComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "JetPack|SFX")
	TObjectPtr<UAudioComponent> JetPackThrusterAudioComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CameraShake")
	TSubclassOf<UCameraShakeBase> SprintShake;
	
	/* Enhanced Input Function*/
	void MoveFunc(const FInputActionInstance& Instance);
	void SprintStart(const FInputActionInstance& Instance);
	void SprintStop(const FInputActionInstance& Instance);

	
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;


public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
// TODO: should be private
protected:
	// Default(walk) Speed.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Params")
	float DefaultSpeed			= 600.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Params")
	float DefaultAcceleration     = 800.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Params")
	float DefaultFov				= 120.f;
	// Sprint Setting
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Params")
	bool  bIsSprint				= false;;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Params")
	float SprintSpeed				= 1000.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Params")
	float SprintAcceleration		= 1200.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Params")
	float SprintFov				= 140.f;

	//Function to update FOV when Sprint
public:
	UPROPERTY(EditAnywhere, Category = "Sprint|Timeline")
	UCurveFloat* SprintFovChangeFloatCurve;
	
protected:
	// Track used for Sprint Fov Change
	FOnTimelineFloat UpdateSprintFOVTrack;
	UFUNCTION()
	void UpdateSprintFov(float TimelineOutput);
	
private:
	// TODO: Change if the entire character file complete.
	inline void SettingDefaultParams();
	
	UPROPERTY()
	UCameraShakeBase* CurrentSprintShake;
};
