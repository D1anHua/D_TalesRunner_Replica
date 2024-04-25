// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/TimelineComponent.h"
#include "CoreMinimal.h"
#include "InputAction.h"
#include "GameFramework/Character.h"
#include "TalesCharacter.generated.h"

class UTalesInventorInteractUI;
class UTalesInventoryComponent;
class UTalesCharacterMovementComponent;
class UCameraComponent;
class UInputMappingContext;
class USpringArmComponent;
class UInputAction;
class UNiagaraComponent;
struct FInputActionInstance;

UCLASS()
class D_TALESRUNNER_REP_API ATalesCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ATalesCharacter(const FObjectInitializer& ObjectInitializer);

	FORCEINLINE UTalesCharacterMovementComponent* GetTalesCharacterMovement() const { return TalesCharacterMovementComponent; }
	FCollisionQueryParams GetIgnoreCharacterParams() const;
	FORCEINLINE UTalesInventoryComponent* GetTalesInventoryComponent() const { return InventoryComponent; }

	void SetSwardMesh(UStaticMesh* InSwardMesh);
	void SetShieldMesh(UStaticMesh* InShieldMesh);
	
protected:
	/* Enhanced Input, PCInputMapping using for PC Game */
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* PCInputMapping;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* ClimbInputMapping;
	
	void AddInputMappingContext(UInputMappingContext* ContextToAdd, int32 InPriority);
	void RemoveInputMappingContext(UInputMappingContext* ContextToAdd);
	// Change InputMapping
	void OnPlayerEnterClimbState();
	void OnPlayerExitClimbState();

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* Input_Move;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* Input_ClimbMove;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* Input_Jump;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* Input_Sprint;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* Input_LookMouse;
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* Input_ClimbHop;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UTalesInventoryComponent> InventoryComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<USpringArmComponent> SpringArmComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<UCameraComponent>    CameraComp;
	// UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	// TObjectPtr<USkeletalMeshComponent> JetPackComp;

	// Sprint related Comp
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	TObjectPtr<UTalesCharacterMovementComponent> TalesCharacterMovementComponent;
	
	// Timeline Comp to control FOV change, when sprint
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sprint|VFX")
	TObjectPtr<UTimelineComponent> SprintTimeLineComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sprint|VFX")
	TObjectPtr<UNiagaraComponent> SprintLineNiagaraComp;
	
	// Static Mesh of Sward and Shield	
	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> SwardMesh;
	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> ShieldMesh;
	//
	// UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "JetPack|VFX")
	// TObjectPtr<UNiagaraComponent> JetPackThrusterComp;
	// UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "JetPack|SFX")
	// TObjectPtr<UAudioComponent> JetPackThrusterAudioComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CameraShake")
	TSubclassOf<UCameraShakeBase> SprintShake;

	
	/* Enhanced Input Function*/
	void MoveFunc(const FInputActionInstance& Instance);
	void ClimbMoveFunc(const FInputActionInstance& Instance);
	void SprintStart(const FInputActionInstance& Instance);
	void SprintStop(const FInputActionInstance& Instance);
	void LookMouse(const FInputActionInstance& Instance);
	void ClimbHop(const FInputActionInstance& Instance);
	
public:
	virtual void Jump() override;
	virtual void StopJumping() override;

	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
// TODO: should be private
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Params")
	float DefaultFov				= 40.f;
	// Sprint Setting
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Params")
	bool  bIsSprint				= false;;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Params")
	float SprintSpeed				= 600.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Params")
	float SprintAcceleration		= 1200.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Params")
	float SprintFov				= 45.f;

	//Function to update FOV when Sprint
public:
	UPROPERTY(EditAnywhere, Category = "Sprint|Timeline")
	UCurveFloat* SprintFovChangeFloatCurve;

	bool bPressedTalesJump;
	
protected:
	// Track used for Sprint Fov Change
	FOnTimelineFloat UpdateSprintFOVTrack;
	UFUNCTION()
	void UpdateSprintFov(float TimelineOutput);
	
private:
	UPROPERTY()
	UCameraShakeBase* CurrentSprintShake;

	//! Interact Component
public:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Interact")
	TSubclassOf<UTalesInventorInteractUI> InteractUIClass;
	
	UPROPERTY()	
	UTalesInventorInteractUI* InteractUI;

	// Interact Begin
	void ActivateInteractUI();
	void UnActivateInteractUI();
};
