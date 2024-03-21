// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/PawnComponent.h"
#include "Character/TalesPawnData.h"
#include "Components/GameFrameworkInitStateInterface.h"
#include "TalesPawnExtensionComponent.generated.h"

class UTalesAbilitySystemCompBase;
/**
 * 
 */
UCLASS()
class D_TALESRUNNER_REP_API UTalesPawnExtensionComponent : public UPawnComponent, public IGameFrameworkInitStateInterface
{
	GENERATED_BODY()

public:

	UTalesPawnExtensionComponent(const FObjectInitializer& ObjectInitializer);

	//! The name of the overall feature, this one depends on other named component featrues
	static const FName NAME_ActorFeatureName;
	//! Get the current ASC
	UFUNCTION(BlueprintPure, Category = "Tales|Pawn")
    UTalesAbilitySystemCompBase* GetTalesAbilitySystemCompBase() const { return AbilitySystemComponent; }

	//! Begin IGameFrameworkInitStateInterface interface
	virtual FName GetFeatureName() const override { return NAME_ActorFeatureName; };
	virtual bool CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const override;
	virtual void HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) override;
	virtual void OnActorInitStateChanged(const FActorInitStateChangedParams& Params) override;
	virtual void CheckDefaultInitialization() override;
	//! Begin IGameFrameworkInitStateInterface interface

	//! Returns the pawn extension component if one exists on the specified actor;
	UFUNCTION(BlueprintPure, Category = "Tales|Pawn")
	static UTalesPawnExtensionComponent* FindPawnExtensionComponent(const AActor* Actor){ return (Actor ? Actor->FindComponentByClass<UTalesPawnExtensionComponent>() : nullptr); }

	//! Gets the pawn data, which is used to specify pawn properties in data
	template<class T>
	const T* GetPawnData() const { return Cast<T>(PawnData); }
	
	//! Should be called by the owning pawn to become the avatar of the ability system
	void InitializeAbilitySystem(UTalesAbilitySystemCompBase* InASC, AActor* InOwnerActor);
	//! Should be called by the owning pawn to remove itself as the avatar of the ability system
	void UnInitializeAbilitySystem();

	//! Should be Called by the owning pawn when the pawn's controller changes.
	void HandlePlayerStateReplicated();
	//! Should be called by the owning pawn when thw input component is setup
	void SetupPlayerInputComponent();

	//! Register with the OnAbilitySystemInitialized delegate and broadcast if our pawn has been registered with the ability system component
	void OnAblitySystemInitialized_RegisterAndCall(FSimpleMulticastDelegate::FDelegate Delegate);
	//! Register with the OnAbilitySystemUnInitialized delegate and broadcast if our pawn has been unregistered with the ability system component
	void OnAblitySystemUninitialized_Register(FSimpleMulticastDelegate::FDelegate Delegate);

protected:
	virtual void OnRegister() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	UFUNCTION()
	void OnRep_PawnData();

	//! Delegate fired when our pawn becomes the ability system's avatar actor
	FSimpleMulticastDelegate OnAbilitySystemInitialized;

	//! Delegate fired when our pawn is removed as the ability system's avatar actor
	FSimpleMulticastDelegate OnAbilitySystemUnInitialized;

	//! Pawn data used to create the pawn, Specified from a spawn function or on a placed instace
	UPROPERTY(EditAnywhere, ReplicatedUsing = OnRep_PawnData, Category = "Tales|Pawn");
	TObjectPtr<const UTalesPawnData> PawnData;

	//! Pointer to the ability system component that is cached for convenience(方便)
	UPROPERTY()
	TObjectPtr<UTalesAbilitySystemCompBase> AbilitySystemComponent;
};
