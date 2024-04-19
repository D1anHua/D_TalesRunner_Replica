// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSystem/TalesAbilitySystemCompBase.h"
#include "GameFramework/Actor.h"
#include "TalesMoney.generated.h"

USTRUCT(BlueprintType)
struct FTalesMoneyData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	FTalesMoneyData():Name("Null"), Amount(0), Mesh(nullptr)
	{}
	
	//! The 'Name' Column is the same as the XP Level
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Money")
	FName Name;

	//! Extra Money Gained at this Money Style
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Money")
	int32 Amount = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Money")
	UStaticMesh* Mesh;
};


UCLASS()
class D_TALESRUNNER_REP_API ATalesMoney : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATalesMoney();
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Mesh")
	TObjectPtr<UStaticMeshComponent> MoneyMesh;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data", meta=(GetOptions="GetMoneyTypeName"))
	FName RowName;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Data")
	TObjectPtr<UDataTable> MoneyTable;
	UPROPERTY(BlueprintReadOnly)
	FTalesMoneyData MoneyData;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
	// Helper Functions
	UPROPERTY()
	TArray<FName> RowNames;
	UFUNCTION()
	TArray<FName> GetMoneyTypeName() const;
};
