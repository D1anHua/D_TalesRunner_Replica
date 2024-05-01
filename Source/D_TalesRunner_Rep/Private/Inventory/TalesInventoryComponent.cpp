// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/TalesInventoryComponent.h"
#include "Character/TalesCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Inventory/TalesInventorInteractUI.h"
#include "Inventory/TalesInventoryInterface.h"
#include "Inventory/TalesInventoryUserWidget.h"
#include "Inventory/Props/TalesMoney.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UTalesInventoryComponent::UTalesInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
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
		SetSwardSlot(this->OnUseSwardSlot);
		SetShieldSlot(this->OnUseShieldSlot);
	}

	if(ensureAlways(InventoryWidgetClass) && GetOwnerRole() == ROLE_AutonomousProxy)
	{
		 InventoryWidget = CreateWidget<UTalesInventoryUserWidget>(UGameplayStatics::GetPlayerController(GetWorld(), 0), InventoryWidgetClass);
	}
}

void UTalesInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	PrimaryInteractTraceByFoot();
}

void UTalesInventoryComponent::SetSwardSlot(FTalesInventoryItemSlot NewSwardSlot)
{
	OnUseSwardSlot = NewSwardSlot;
	if(OnUseSwardSlot.IsValid() && OnUseSwardSlot.ItemType == Sward)
	{
		auto Row = OnUseSwardSlot.GetRow();
		if(Row)
		{
			TalesCharacterOwner->SetSwardMesh(Row->Mesh);
		}
	}else if(!OnUseSwardSlot.IsValid())
	{
		// 拆掉Mesh
		TalesCharacterOwner->SetSwardMesh(nullptr);
	}
}

void UTalesInventoryComponent::SetShieldSlot(FTalesInventoryItemSlot NewShieldSlot)
{
	OnUseShieldSlot = NewShieldSlot;
	if(OnUseShieldSlot.IsValid() && OnUseShieldSlot.ItemType == Shield)
	{
		auto Row = OnUseShieldSlot.GetRow();
		if(Row)
		{
			TalesCharacterOwner->SetShieldMesh(Row->Mesh);
		}
	}
	else if(!OnUseShieldSlot.IsValid())
	{
		TalesCharacterOwner->SetShieldMesh(nullptr);
	}
}

bool UTalesInventoryComponent::PackageDataDecrease(FTalesInventoryItemSlot TargetName, int delta)
{
	if(delta > 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Inventory Component : Package Data Changes Doesn't support delta > 0"))
		return false;
	}
	auto ItemType = TargetName.ItemType;
	TMultiMap<FName, FTalesInventoryItemSlot>* PickedColumnItems;
	switch(ItemType)
	{
	case Sward:
		PickedColumnItems = &PackageDatas.Sward;
		break;
	case Shield:
		PickedColumnItems = &PackageDatas.Shield;
		break;
	case Eatable:
		TargetName.Quantity = delta;
		PackageDataChange_Eatable(TargetName);
		return true;
	default:
		return false;			
	}
	// 只对 sward 和 shield
	auto FindAns = PickedColumnItems->Find(TargetName.ItemRowHandle.RowName);
	while(FindAns != nullptr &&  FindAns->ItemRowHandle.RowName == TargetName.ItemRowHandle.RowName)
	{
		if(*FindAns == TargetName)
		{
			PickedColumnItems->RemoveSingle(TargetName.ItemRowHandle.RowName, TargetName);
			return true;
		}
		else
		{
			++FindAns;
		}
	}
	return false;
}
// ------------------------------------------------  Main Data -------------------------------------------------
void UTalesInventoryComponent::AddItemToPackage(AActor* HitActor)
{
	auto ItemActor = Cast<ATalesInventoryItem>(HitActor);
	if(ItemActor)
	{
		TMultiMap<FName ,FTalesInventoryItemSlot>* PickedColumnItems;
		switch(ItemActor->Item.ItemType)
		{
		case Sward:
			PickedColumnItems = &PackageDatas.Sward;
			break;
		case Shield:
			PickedColumnItems = &PackageDatas.Shield;
			break;
		case Eatable:
			PickedColumnItems = &PackageDatas.Eatable;
			PackageDataChange_Eatable(ItemActor->Item);
			return;
		default:
			return;			
		}
		// Add Item, Just Sward and Shield
		// 注: Sward 不合并 quantity, 直接装
		// Sward和shield没有 Stack的概念
		PickedColumnItems->AddUnique(ItemActor->Item.ItemRowHandle.RowName, ItemActor->Item);	
	}
}

void UTalesInventoryComponent::PackageDataChange_Eatable(const FTalesInventoryItemSlot ItemEatable)
{
		auto FindAns = PackageDatas.Eatable.Find(ItemEatable.ItemRowHandle.RowName);
		if(FindAns == nullptr)
		{
			if(ItemEatable.IsValid())
			{
				// 原先没有
				PackageDatas.Eatable.AddUnique(ItemEatable.ItemRowHandle.RowName, ItemEatable);	
			}
		}
		else
		{
			// 不考虑装满不装满的问题了
			int numTemp = FindAns->Quantity + ItemEatable.Quantity;
			FindAns->Quantity = numTemp;
			if(FindAns->Quantity <= 0)
			{
				PackageDatas.Eatable.RemoveSingle(ItemEatable.ItemRowHandle.RowName, *FindAns);	
			}
		}
}

void UTalesInventoryComponent::PickKeyPressed()
{
	// @TODO Delete, 做两遍检测
	FHitResult Hit;
	FCollisionShape CollisionShape;
	CollisionShape.SetBox(FVector3f(40.f, 40.f, 20.f));
	FVector BeginLocation = TalesCharacterOwner->GetActorLocation() -  TalesCharacterOwner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * TalesCharacterOwner->GetActorUpVector();
	FVector EndLocation = BeginLocation + TalesCharacterOwner->GetActorForwardVector() * 60;
	bool bGetHit = GetWorld()->SweepSingleByProfile(Hit, BeginLocation, EndLocation, FQuat::Identity, "InventoryItem", CollisionShape,TalesCharacterOwner->GetIgnoreCharacterParams());
	if(bGetHit && Hit.GetActor()->Implements<UTalesInventoryInterface>())
	{
		AddItemToPackage(Hit.GetActor());
		Hit.GetActor()->Destroy();
	}
}

void UTalesInventoryComponent::PrimaryInteractTraceBySight()
{
	FVector EyeLocation;
	FRotator EyeRotation;
	TalesCharacterOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	FVector End = EyeLocation + (EyeRotation.Vector() * 1000);

	FHitResult Hit;
	FCollisionShape CollisionShape;
	CollisionShape.SetSphere(20.f);
	bool bGetHit = GetWorld()->SweepSingleByProfile(Hit, EyeLocation, End, FQuat::Identity, "InventoryItem", CollisionShape,TalesCharacterOwner->GetIgnoreCharacterParams());
	if(bGetHit)
	{
		AActor* HitActor = Hit.GetActor();
		if(HitActor->Implements<UTalesInventoryInterface>())
		{
			APawn* MyPawn = Cast<APawn>(TalesCharacterOwner);
			ITalesInventoryInterface::Execute_Interact(HitActor, MyPawn);	
		}
		else
		{
			if(TalesCharacterOwner->InteractUI->IsInViewport())
			{
				TalesCharacterOwner->UnActivateInteractUI();
			}
		}
		// DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 25.f, 32, FColor::Green, false, 4.f);
	}
	// FColor DebugColor = bGetHit ? FColor::Green : FColor::Red;
	// DrawDebugLine(GetWorld(), EyeLocation, End, DebugColor, false, 2.f, 0, 2.f);	
}

void UTalesInventoryComponent::PrimaryInteractTraceByFoot()
{
	FHitResult Hit;
	FCollisionShape CollisionShape;
	CollisionShape.SetBox(FVector3f(40.f, 40.f, 20.f));
	FVector BeginLocation = TalesCharacterOwner->GetActorLocation() -  TalesCharacterOwner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * TalesCharacterOwner->GetActorUpVector();
	FVector EndLocation = BeginLocation + TalesCharacterOwner->GetActorForwardVector() * 60;
	bool bGetHit = GetWorld()->SweepSingleByProfile(Hit, BeginLocation, EndLocation, FQuat::Identity, "InventoryItem", CollisionShape,TalesCharacterOwner->GetIgnoreCharacterParams());
	if(bGetHit && Hit.GetActor()->Implements<UTalesInventoryInterface>())
	{
		AActor* HitActor = Hit.GetActor();
		APawn* MyPawn = Cast<APawn>(TalesCharacterOwner);
		ITalesInventoryInterface::Execute_Interact(HitActor, MyPawn);	
		// DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 25.f, 32, FColor::Green, false, 4.f);
	}
	else
	{
		if(TalesCharacterOwner->InteractUI && TalesCharacterOwner->InteractUI->IsInViewport())
		{
			TalesCharacterOwner->UnActivateInteractUI();
		}
	}
	// FColor DebugColor = bGetHit ? FColor::Green : FColor::Red;
	// DrawDebugLine(GetWorld(), BeginLocation, EndLocation, DebugColor, false, 2.f, 0, 2.f);	
}
void UTalesInventoryComponent::OnCharacterOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto Money = Cast<ATalesMoney>(OtherActor);
	InventoryMoneyAmount += Money->MoneyData.Amount;
	// MoneyAmountChangeDelegate.Broadcast(Cast<AActor>(Money), this, InventoryMoneyAmount, Money->MoneyData.Amount);
	Money->Destroy();
	GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Blue, FString::FromInt(InventoryMoneyAmount));	
}

