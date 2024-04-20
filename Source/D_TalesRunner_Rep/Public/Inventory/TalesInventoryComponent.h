// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TalesInventoroyItem.h"
#include "TalesInventoryComponent.generated.h"

class UTalesInventoryUserWidget;
//! 用来存放不同界面的Array
USTRUCT(BlueprintType)
struct FTalesInventoryPackageDatas
{
	GENERATED_BODY()

	TMultiMap<FName, FTalesInventoryItemSlot> Sward;
	TMultiMap<FName, FTalesInventoryItemSlot> Shield;
	TMultiMap<FName, FTalesInventoryItemSlot> Eatable;
};

// DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FMoneyAmountChangeDelegate, AActor*, InstigateActor, UTalesInventoryComponent*, OwnComp, int32, MoneyAmount, int32, delta);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class D_TALESRUNNER_REP_API UTalesInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTalesInventoryComponent();

	// UPROPERTY(BlueprintAssignable)
	// FMoneyAmountChangeDelegate MoneyAmountChangeDelegate;
	
	virtual void InitializeComponent() override;

	//! GetFunction
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FORCEINLINE int32 GetMoneyAmount() const { return InventoryMoneyAmount; }
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FORCEINLINE float GetHeartNow() const { return InventoryHeartNow; }
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FORCEINLINE float GetHeartMax() const { return InventoryMaxHeart; }
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FORCEINLINE FTalesInventoryPackageDatas GetPackagesDatas() const { return PackageDatas; }
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FORCEINLINE UTalesInventoryUserWidget* GetInventoryUserWidget() const { return InventoryWidget; }
	
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
	TSubclassOf<UTalesInventoryUserWidget> InventoryWidgetClass;
	UPROPERTY(BlueprintReadWrite, Category = "Inventory|UI")
	UTalesInventoryUserWidget* InventoryWidget;

	UPROPERTY(Transient)
	class ATalesCharacter* TalesCharacterOwner;


	// Main Data
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory|Data")
	FTalesInventoryPackageDatas PackageDatas;

	// @TODO 后面要加到接口里面, 每个物体的拾取的操作都不一样
	UFUNCTION(BlueprintCallable)
	void AddItemToPackage(AActor* HitActor);

	UFUNCTION(BlueprintCallable)
	void PickKeyPressed();
private:
	void PrimaryInteractTraceBySight();
	void PrimaryInteractTraceByFoot();
};
