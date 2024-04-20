// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Props/TalesHealthBarUserWidget.h"

#include "Character/TalesCharacter.h"
#include "Components/WrapBox.h"
#include "Inventory/TalesInventoryComponent.h"
#include "Inventory/Props/TalesHeartUserWidget.h"
#include "Kismet/GameplayStatics.h"

void UTalesHealthBarUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	auto OwnCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	ATalesCharacter* TalesCharacter = Cast<ATalesCharacter>(OwnCharacter);
	if(TalesCharacter)
	{
		MaxHealth = TalesCharacter->GetTalesInventoryComponent()->GetHeartMax();
		NowHealth = TalesCharacter->GetTalesInventoryComponent()->GetHeartNow();
	}
}

void UTalesHealthBarUserWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UTalesHealthBarUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	InitialHealth(MaxHealth, NowHealth);
}

void UTalesHealthBarUserWidget::InitialHealth(float InMaxHealth, float InNowHealth)
{
	if(HealthBarWarpBox && ensureAlways(HeartWidget))
	{
		HealthBarWarpBox->ClearChildren();
		float temp = InNowHealth;
		for(int i = 0; i < FMath::CeilToInt32(InMaxHealth); ++i)
		{
			auto Widget = CreateWidget(this, HeartWidget);
			HealthBarWarpBox->AddChildToWrapBox(Widget);
			UTalesHeartUserWidget* TalesHeartUserWidget = Cast<UTalesHeartUserWidget>(Widget);
			if(TalesHeartUserWidget)
			{
				if(temp >= 0.9f)
				{
					TalesHeartUserWidget->SetImage(1.0);
					temp -= 1.f;
				}
				else if(temp >= 0.4f){
					TalesHeartUserWidget->SetImage(0.5);
					temp -= 0.5f;
				}
				else
				{
					TalesHeartUserWidget->SetImage(0);
				}
			}	
		}
	}
}
