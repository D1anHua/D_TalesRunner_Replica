// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TalesInventoryInterface.h"
#include "TalesInventoryType.h"
#include "AttributeSystem/TalesAbilitySystemCompBase.h"
#include "GameFramework/Actor.h"
#include "TalesInventoroyItem.generated.h"

//! Struct to show message for Item
USTRUCT(BlueprintType)
struct FTalesInventoryMenuItem : public FTableRowBase
{
	GENERATED_BODY()

	FTalesInventoryMenuItem() : Name(FText::FromString("Null")),
								Description(FText::FromString("Nothing")),
								Thumbnails(nullptr),
								StackSize(1),
								Power(0.f),
								Mesh(nullptr){}
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
	FText			Name;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
	FText			Description;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
	UTexture2D*	    Thumbnails;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
	int32     	    StackSize;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
	float     	    Power;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
	UStaticMesh*    Mesh;
};

//! Structure for one Item
USTRUCT(BlueprintType)
struct FTalesInventoryItemSlot
{
	GENERATED_BODY()

	FTalesInventoryItemSlot() : ItemRowHandle(), Quantity(0), ItemType(1){}
	
	UPROPERTY(meta = (RowType="TalesInventoryMenuItem"), EditAnywhere, BlueprintReadWrite)
	FDataTableRowHandle		ItemRowHandle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32					Quantity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<ETalesInventoryItem>		ItemType;

	bool operator==(const FTalesInventoryItemSlot& Others)
	{
		if(this->ItemRowHandle != Others.ItemRowHandle || this->Quantity != Others.Quantity || this->ItemType != Others.ItemType)
		{
			return false;
		}
		return true;
	}
};

UCLASS()
class D_TALESRUNNER_REP_API ATalesInventoryItem : public AActor , public ITalesInventoryInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATalesInventoryItem();
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Mesh")
	TObjectPtr<UStaticMeshComponent> ItemMesh;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Item")
	FTalesInventoryItemSlot Item;
	UPROPERTY(BlueprintReadOnly)
	FTalesInventoryMenuItem MenuItem;
	
	//! Begin ITalesInventoryInterface
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "UserWidget")
	TSubclassOf<class UTalesInventorInteractUI>  InteractUserWidget;
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
	//! End ITalesInventoryInterface
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
