// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/TalesInventoryComponent.h"
#include "Character/TalesCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Inventory/Props/TalesMoney.h"

// Sets default values for this component's properties
UTalesInventoryComponent::UTalesInventoryComponent()
{
	InventoryMoneyAmount = 0;
	InventoryMaxHeart = 8;
	InventoryHeartNow = 7.5;
}

void UTalesInventoryComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

// Called when the game starts
void UTalesInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	TalesCharacterOwner = Cast<ATalesCharacter>(GetOwner());
	if(TalesCharacterOwner)
	{
		auto CapsuleComp = TalesCharacterOwner->GetCapsuleComponent();
		CapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnCharacterOverlap);
	}
}

void UTalesInventoryComponent::OnCharacterOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto Money = Cast<ATalesMoney>(OtherActor);
	InventoryMoneyAmount += Money->MoneyData.Amount;
	MoneyAmountChangeDelegate.Broadcast(Cast<AActor>(Money), this, InventoryMoneyAmount, Money->MoneyData.Amount);
	Money->Destroy();
	GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Blue, FString::FromInt(InventoryMoneyAmount));	
}

