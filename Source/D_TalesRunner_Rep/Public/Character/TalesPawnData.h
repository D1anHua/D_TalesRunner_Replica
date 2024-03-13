// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSystem/TalesAbilitySet.h"
#include "AttributeSystem/TalesAbilityTagRelationMapping.h"
#include "Engine/DataAsset.h"
#include "Input/TalesInputConfig.h"
#include "TalesPawnData.generated.h"

/*!
 * UTalesPawnData
 *
 * Non-mutable data asset that contains properties uesd to define a pawn
 */
UCLASS()
class D_TALESRUNNER_REP_API UTalesPawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UTalesPawnData(const FObjectInitializer& ObjectInitializer);

	//! Class to instantiate(实例化) for this pawn(should usually derive from ATalesPawn or ATalesCharacter)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tales|Pawn")
	TSubclassOf<APawn> PawnClass;

	//! Ability sets to grant to this pawn's ability system
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tales|Abilities")
	TArray<TObjectPtr<UTalesAbilitySet>> Abilitieses;

	//! What mapping of ablility tags to use for actions taking by this pawn
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Talse|Abilities")
	TObjectPtr<UTalesAbilityTagRelationMapping> TagRelationshipMapping;

	//! What mapping of ability tags to use for actions taking by this pawn
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category= "Tales|Abilities")
	TObjectPtr<UTalesInputConfig> InputConfig;
};
