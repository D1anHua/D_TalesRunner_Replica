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
	FORCEINLINE FTalesInventoryItemSlot GetSwardSlot() const { return OnUseSwardSlot; }
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FORCEINLINE FTalesInventoryItemSlot GetShieldSlot() const { return OnUseShieldSlot; }
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FORCEINLINE FTalesInventoryPackageDatas GetPackagesDatas() const { return PackageDatas; }
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FORCEINLINE UTalesInventoryUserWidget* GetInventoryUserWidget() const { return InventoryWidget; }
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FORCEINLINE void SetHeartNow(const float NowHeart){ InventoryHeartNow = NowHeart > InventoryMaxHeart ? InventoryMaxHeart : NowHeart; }
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FORCEINLINE void SetHeartMax(const float MaxHeart){ InventoryMaxHeart = MaxHeart; }
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SetSwardSlot(struct FTalesInventoryItemSlot NewSwardSlot);
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SetShieldSlot(struct FTalesInventoryItemSlot NewShieldSlot);

	/*!
	 * 用于修改PackageData中某一项的数据 
	 * @param TargetSlot 想要增加或者删除的目标的Name
	 * @param Delta  增加或者删除的量, 目前是只能减少
	 * @return 存在返回true
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool PackageDataDecrease(FTalesInventoryItemSlot TargetSlot ,int Delta);

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory|Data")
	struct FTalesInventoryItemSlot OnUseSwardSlot;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory|Data")
	struct FTalesInventoryItemSlot OnUseShieldSlot;

	// Main Data
	UPROPERTY()
	FTalesInventoryPackageDatas PackageDatas;

	// @TODO 后面要加到接口里面, 每个物体的拾取的操作都不一样
	UFUNCTION(BlueprintCallable)
	void AddItemToPackage(AActor* HitActor);

	UFUNCTION(BlueprintCallable)
	void PickKeyPressed();
private:
	void PrimaryInteractTraceBySight();
	void PrimaryInteractTraceByFoot();
	// void SetInventoryData(TMultiMap<FName, FTalesInventoryItemSlot>& Data, FTalesInventoryItemSlot NewData, FTalesInventoryItemSlot OldData);
	void PackageDataChange_Eatable(FTalesInventoryItemSlot ItemEatable);
};
