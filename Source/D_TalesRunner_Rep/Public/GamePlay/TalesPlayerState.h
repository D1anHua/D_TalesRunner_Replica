// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/TalesPawnData.h"
#include "GameFramework/PlayerState.h"
#include "TalesPlayerState.generated.h"

class ATalesPlayerControllerBase;
class UTalesPawnData;
/**
 * 
 */
UCLASS()
class D_TALESRUNNER_REP_API ATalesPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ATalesPlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = "Tales | PlayerState")
	ATalesPlayerControllerBase* GetTalesPlayerController() const;

	template <class T>
	const T* GetPawnData() const { return Cast<T>(PawnData); }

	void SetPawnData(const UTalesPawnData* InPawnData);
	
	//! begin AActor Interface
	virtual void PreInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Reset() override;
	//~ End AActor Interface

protected:
	//! Begin AplayerState interface
	virtual void CopyProperties(APlayerState* PlayerState) override;
	//~ End AplayerState interface

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	UFUNCTION()
	void OnRep_PawnData();

protected:
	UPROPERTY(ReplicatedUsing = OnRep_PawnData)
	TObjectPtr<const UTalesPawnData> PawnData;

private:
	
};
