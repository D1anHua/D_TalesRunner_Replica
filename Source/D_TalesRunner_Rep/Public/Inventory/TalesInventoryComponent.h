// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TalesInventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FMoneyAmountChangeDelegate, AActor*, InstigateActor, UTalesInventoryComponent*, OwnComp, int32, MoneyAmount, int32, delta);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class D_TALESRUNNER_REP_API UTalesInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTalesInventoryComponent();

	UPROPERTY(BlueprintAssignable)
	FMoneyAmountChangeDelegate MoneyAmountChangeDelegate;
	
	virtual void InitializeComponent() override;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FORCEINLINE int32 GetMoneyAmount(){ return InventoryMoneyAmount; }

	UFUNCTION()
	void OnCharacterOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory|Money")	
	int32 InventoryMoneyAmount = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory|UI")
	TSubclassOf<UUserWidget> InventoryWidget;

	UPROPERTY(Transient)
	class ATalesCharacter* TalesCharacterOwner;
};
