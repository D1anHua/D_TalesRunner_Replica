// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/TalesInventoroyItem.h"

#include "Blueprint/UserWidget.h"
#include "Character/TalesCharacter.h"
#include "Engine/DataTable.h"
#include "Inventory/TalesInventorInteractUI.h"
#include "Inventory/TalesInventoryComponent.h"

// Sets default values
ATalesInventoryItem::ATalesInventoryItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	RootComponent = ItemMesh;
	ItemMesh->SetSimulatePhysics(true);

}

void ATalesInventoryItem::Interact_Implementation(APawn* InstigatorPawn)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, Item.ItemRowHandle.RowName.ToString());
	ATalesCharacter* InstigatorCharacter = Cast<ATalesCharacter>(InstigatorPawn);
	if(InstigatorCharacter)
	{
		InstigatorCharacter->ActivateInteractUI();
	}
}

// Called when the game starts or when spawned
void ATalesInventoryItem::BeginPlay()
{
	Super::BeginPlay();
	
}
#if WITH_EDITOR
void ATalesInventoryItem::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if(!Item.ItemRowHandle.IsNull())
	{
		FString ContextString = "Searching for Mesh";
		FTalesInventoryMenuItem* Row = Item.ItemRowHandle.GetRow<FTalesInventoryMenuItem>(ContextString);
		
		if(Row)
		{
			MenuItem.Name			= Row->Name;
			MenuItem.Description    = Row->Description;
			MenuItem.Thumbnails		= Row->Thumbnails;
			MenuItem.StackSize		= Row->StackSize;
			MenuItem.Power		    = Row->Power;
			MenuItem.Mesh			= Row->Mesh;
			ItemMesh->SetStaticMesh(MenuItem.Mesh);
		}
	}
}
#endif
