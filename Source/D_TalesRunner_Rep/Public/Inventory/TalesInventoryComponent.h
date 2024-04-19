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

	//! GetFunction
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FORCEINLINE int32 GetMoneyAmount() const { return InventoryMoneyAmount; }
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FORCEINLINE float GetHeartNow() const { return InventoryHeartNow; }
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FORCEINLINE float GetHeartMax() const { return InventoryMaxHeart; }
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FORCEINLINE void SetHeartNow(const float NowHeart){ InventoryHeartNow = NowHeart; }
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FORCEINLINE void SetHeartMax(const float MaxHeart){ InventoryMaxHeart = MaxHeart; }

	//! Bind Event
	UFUNCTION()
	void OnCharacterOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//! Money Variable
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory|Money")	
	int32 InventoryMoneyAmount = 0;

	//! Heart Variable(Health)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory|Health")	
	float InventoryHeartNow = 7.5;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory|Health")	
	float InventoryMaxHeart = 10;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory|UI")
	TSubclassOf<UUserWidget> InventoryWidget;

	UPROPERTY(Transient)
	class ATalesCharacter* TalesCharacterOwner;

private:
	void PrimaryInteractTraceBySight();
	void PrimaryInteractTraceByFoot();
};
