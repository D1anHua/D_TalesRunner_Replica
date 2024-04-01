// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TalesCharacterBase.h"

#include "Character/TalesCharacterMovementComponent.h"
#include "AttributeSystem/TalesAbilitySystemCompBase.h"
#include "Character/TalesPawnExtensionComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/GameFrameworkComponentManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "System/TalesGameplayTags.h"

static FName NAME_TalesCharacterCollisionProfile_Capsule(TEXT("TalesPawnCapsule"));
static FName NAME_TalesCharacterCollisionProfile_Mesh(TEXT("TalesPawnMesh"));

FSharedRepMovement::FSharedRepMovement()
{
	RepMovement.LocationQuantizationLevel = EVectorQuantization::RoundTwoDecimals;
}

bool FSharedRepMovement::FillForCharacter(ACharacter* Character)
{
	if(USceneComponent* PawnRootComponent = Character->GetRootComponent())
	{
		UCharacterMovementComponent* CharacterMovement = Character->GetCharacterMovement();
		RepMovement.Location = FRepMovement::RebaseOntoLocalOrigin(PawnRootComponent->GetComponentLocation(), Character);
		RepMovement.Rotation = PawnRootComponent->GetComponentRotation();

		RepMovement.LinearVelocity = CharacterMovement->Velocity;
		RepMovementMode = CharacterMovement->PackNetworkMovementMode();
		bProxyIsJumpForceApplied = Character->bProxyIsJumpForceApplied || (Character->JumpForceTimeRemaining > 0.f);

		// Timestamp is sent as zero if unused
		if((CharacterMovement->NetworkSmoothingMode == ENetworkSmoothingMode::Linear) || CharacterMovement->bNetworkAlwaysReplicateTransformUpdateTimestamp)
		{
			RepTimeStamp =CharacterMovement->GetServerLastTransformUpdateTimeStamp();
		}
		else
		{
			RepTimeStamp = 0.f;
		}
		return true;
	}
	return false;
}

bool FSharedRepMovement::Equals(const FSharedRepMovement& Other, ACharacter* Character) const
{
	if (RepMovement.Location != Other.RepMovement.Location || RepMovement.Rotation != Other.RepMovement.Rotation)
	{
		return false;
	}
	if (RepMovement.LinearVelocity != Other.RepMovement.LinearVelocity)
	{
		return false;
	}
	if (RepMovementMode != Other.RepMovementMode)
	{
		return false;
	}
	if( bProxyIsJumpForceApplied != Other.bProxyIsJumpForceApplied)
	{
		return false;
	}
	if(bIsCrouched != Other.bIsCrouched)
	{
		return false;
	}

	return true;
}

bool FSharedRepMovement::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
	bOutSuccess = true;
	RepMovement.NetSerialize(Ar, Map, bOutSuccess);
	Ar << RepMovementMode;
	Ar << bProxyIsJumpForceApplied;
	Ar << bIsCrouched;

	uint8 bHasTimeStamp = (RepTimeStamp != 0.f);
	Ar.SerializeBits(&bHasTimeStamp, 1);
	if(bHasTimeStamp)
	{
		Ar << RepTimeStamp;
	}

	return true;
}

/////////////////////////////////////////////////////////////////////
/// ATalesCharacterBase
ATalesCharacterBase::ATalesCharacterBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UTalesCharacterMovementComponent>(CharacterMovementComponentName))
{
	// Avoid ticking characters if possible
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	NetCullDistanceSquared = 9000000.f;

	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	check(CapsuleComp);
	CapsuleComp->InitCapsuleSize(40.f, 90.f);
	CapsuleComp->SetCollisionProfileName(NAME_TalesCharacterCollisionProfile_Capsule);

	UTalesCharacterMovementComponent* TalesMoveComp = CastChecked<UTalesCharacterMovementComponent>(GetCharacterMovement());
	TalesMoveComp->GravityScale = 1.f;
	TalesMoveComp->MaxAcceleration = 2400.f;
	TalesMoveComp->BrakingFrictionFactor = 1.0f;
	TalesMoveComp->GroundFriction = 6.0f;
	TalesMoveComp->bUseControllerDesiredRotation = false;
	TalesMoveComp->bOrientRotationToMovement = false;
	TalesMoveComp->RotationRate = FRotator(0.f, 720.f, 0.f);
	TalesMoveComp->bAllowPhysicsRotationDuringAnimRootMotion = false;

	PawnExtComponent = CreateDefaultSubobject<UTalesPawnExtensionComponent>(TEXT("PawnExtensionComp"));
	PawnExtComponent->OnAblitySystemInitialized_RegisterAndCall(FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::OnAbilitySystemInitialized));
	PawnExtComponent->OnAblitySystemUninitialized_Register(FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::OnAbilitySystemUnInitialized));

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	BaseEyeHeight = 80.f;
	CrouchedEyeHeight = 50.f;
}

UTalesAbilitySystemCompBase* ATalesCharacterBase::GetTalesAbilitySystemComponent() const
{
	return Cast<UTalesAbilitySystemCompBase>(GetAbilitySystemComponent());
}

UAbilitySystemComponent* ATalesCharacterBase::GetAbilitySystemComponent() const
{
	return PawnExtComponent->GetTalesAbilitySystemCompBase();
}

void ATalesCharacterBase::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	if(const UTalesAbilitySystemCompBase* TalesASC = GetTalesAbilitySystemComponent())
	{
		TalesASC->GetOwnedGameplayTags(TagContainer);
	}
}

bool ATalesCharacterBase::HasMatchingGameplayTag(FGameplayTag TagToCheck) const
{
	if(const UTalesAbilitySystemCompBase* TalesASC = GetTalesAbilitySystemComponent())
	{
		return TalesASC->HasMatchingGameplayTag(TagToCheck);
	}

	return false;
}

bool ATalesCharacterBase::HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	if(const UTalesAbilitySystemCompBase* TalesASC = GetTalesAbilitySystemComponent())
	{
		return TalesASC->HasAllMatchingGameplayTags(TagContainer);
	}
	return false;
}

bool ATalesCharacterBase::HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	if(const UTalesAbilitySystemCompBase* TalesASC = GetTalesAbilitySystemComponent())
	{
		return TalesASC->HasAnyMatchingGameplayTags(TagContainer);
	}
	return false;
}

void ATalesCharacterBase::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	// Register
	UGameFrameworkComponentManager::AddGameFrameworkComponentReceiver(this);
}

// Called when the game starts or when spawned
void ATalesCharacterBase::BeginPlay()
{
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(this, UGameFrameworkComponentManager::NAME_GameActorReady);
	Super::BeginPlay();
	//@ TODO: SignificanceManager
}

void ATalesCharacterBase::Reset()
{
	DisableMovementAndCollision();

	K2_OnReset();

	UnInitAndDestory();
}

void ATalesCharacterBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	//@ TODO: SignificanceManager
	UGameFrameworkComponentManager::RemoveGameFrameworkComponentReceiver(this);
	Super::EndPlay(EndPlayReason);
}

void ATalesCharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ThisClass, ReplicatedAcceleration, COND_SimulatedOnly);
}

void ATalesCharacterBase::PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker)
{
	Super::PreReplication(ChangedPropertyTracker);
	if(UCharacterMovementComponent* MovementComponent = GetCharacterMovement())
	{
		// Compress Acceleration:  XY components as direction + magnitude, Z component as direct value
		const double MaxAccel = MovementComponent->MaxAcceleration;
		const FVector CurrentAccell = MovementComponent->GetCurrentAcceleration();
		double AccelXYRadians, AccelXYMagnitude;
		// 正交坐标 -> 笛卡尔坐标 换成方向和大小
		FMath::CartesianToPolar(CurrentAccell.X, CurrentAccell.Y, AccelXYMagnitude, AccelXYRadians);

		ReplicatedAcceleration.AccelXYRadians   = FMath::FloorToInt((AccelXYRadians / TWO_PI) * 255.0);
		ReplicatedAcceleration.AccelXYMagnitude = FMath::FloorToInt((AccelXYMagnitude / MaxAccel) * 255.0);
		ReplicatedAcceleration.AccelZ			= FMath::FloorToInt((CurrentAccell.Z / MaxAccel) * 127.0);
	}
}

void ATalesCharacterBase::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	PawnExtComponent->HandlePlayerStateReplicated();
}

void ATalesCharacterBase::OnRep_ReplicatedAcceleration()
{
	if(UTalesCharacterMovementComponent* TalesMovementComp = Cast<UTalesCharacterMovementComponent>(GetCharacterMovement()))
	{
		// Decompress Acceleration
		const double MaxAccel		   = TalesMovementComp->MaxAcceleration;
		const double AcceleXYMagnitude = double(ReplicatedAcceleration.AccelXYMagnitude) * MaxAccel / 255.0;
		const double AcceleXYRadians   = double(ReplicatedAcceleration.AccelXYRadians) * TWO_PI / 255.0;
		
		FVector UnpackedAcceleration(FVector::ZeroVector);
		FMath::PolarToCartesian(AcceleXYMagnitude, AcceleXYRadians, UnpackedAcceleration.X, UnpackedAcceleration.Y);
		UnpackedAcceleration.Z = double(ReplicatedAcceleration.AccelZ) * MaxAccel / 127.0;

		// TalesMovementComp->SetReplicatedAcceleration(UnpackedAcceleration);
	}
}


void ATalesCharacterBase::OnAbilitySystemInitialized()
{
	UTalesAbilitySystemCompBase* TalesASC = GetTalesAbilitySystemComponent();
	check(TalesASC);
	
}

void ATalesCharacterBase::OnAbilitySystemUnInitialized()
{
}

void ATalesCharacterBase::DisableMovementAndCollision()
{
	if(Controller)
	{
		Controller->SetIgnoreLookInput(true);
	}

	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	check(CapsuleComp);
	CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CapsuleComp->SetCollisionResponseToAllChannels(ECR_Ignore);

	UTalesCharacterMovementComponent* TaleMoveComp = CastChecked<UTalesCharacterMovementComponent>(GetCharacterMovement());
	TaleMoveComp->StopMovementImmediately();
	TaleMoveComp->DisableMovement();
}

void ATalesCharacterBase::UnInitAndDestory()
{
	if(GetLocalRole() == ROLE_Authority)
	{
		DetachFromControllerPendingDestroy();
		SetLifeSpan(0.1f);
	}

	if(UTalesAbilitySystemCompBase* TalesASC = GetTalesAbilitySystemComponent())
	{
		if(TalesASC->GetAvatarActor() == this)
		{
			PawnExtComponent->UnInitializeAbilitySystem();
		}
	}
	SetActorHiddenInGame(true);
}


void ATalesCharacterBase::InitializeGameplayTags()
{
	// Clear Tags that may be lingering on the AS from the previous pawn
	if(UTalesAbilitySystemCompBase* TalesASC = GetTalesAbilitySystemComponent())
	{
		for(const TPair<uint8, FGameplayTag>& TagMapping : TalesGameplayTags::MovementModeTagMap)
		{
			if(TagMapping.Value.IsValid())
			{
				TalesASC->SetLooseGameplayTagCount(TagMapping.Value, 0);
			}
		}

		for(const TPair<uint8, FGameplayTag>& TagMapping : TalesGameplayTags::CustomMovementModeTagMap)
		{
			if(TagMapping.Value.IsValid())
			{
				TalesASC->SetLooseGameplayTagCount(TagMapping.Value, 0);
			}
		}

		UTalesCharacterMovementComponent* TalseMoveMode = CastChecked<UTalesCharacterMovementComponent>(GetCharacterMovement());
		
	}
}

void ATalesCharacterBase::SetMovementModeTag(EMovementMode MovementMode, uint8 CustomMovementMode, bool bTagEnable)
{
	if(UTalesAbilitySystemCompBase* TalseASC = GetTalesAbilitySystemComponent())
	{
		const FGameplayTag* MovementModeTag = nullptr;
		if(MovementMode == MOVE_Custom)
		{
			MovementModeTag = TalesGameplayTags::CustomMovementModeTagMap.Find(CustomMovementMode);
		}
		else
		{
			MovementModeTag = TalesGameplayTags::MovementModeTagMap.Find(MovementMode);
		}

		if(MovementModeTag && MovementModeTag->IsValid())
		{
			TalseASC->SetLooseGameplayTagCount(*MovementModeTag, (bTagEnable ? 1 : 0));
		}
	}
}

