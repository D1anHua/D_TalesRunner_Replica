// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Props/TalesMoney.h"

// Sets default values
ATalesMoney::ATalesMoney()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	MoneyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MoneyMesh"));
	RootComponent = MoneyMesh;
}

// Called when the game starts or when spawned
void ATalesMoney::BeginPlay()
{
	Super::BeginPlay();
	
}

#if WITH_EDITOR
void ATalesMoney::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	if(MoneyTable)
	{
		FString ContextString = "Searching for Mesh";
		FTalesMoneyData* Row = MoneyTable->FindRow<FTalesMoneyData>(RowName, ContextString);

		if(Row)
		{
			MoneyData.Name = Row->Name;
			MoneyData.Amount = Row->Amount;
			MoneyData.Mesh = Row->Mesh;
			MoneyMesh->SetStaticMesh(MoneyData.Mesh);
		}

		RowNames = MoneyTable->GetRowNames();
	}
}

TArray<FName> ATalesMoney::GetMoneyTypeName() const
{
	return RowNames;
}
#endif

