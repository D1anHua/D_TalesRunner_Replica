// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/TalesPawnExtensionComponent.h"
#include "Net/UnrealNetwork.h"
#include "AttributeSystem/TalesAbilitySystemCompBase.h"
#include "Components/GameFrameworkComponentManager.h"
#include "System/TalesGameplayTags.h"
#include "System/TalesLogChannels.h"

const FName UTalesPawnExtensionComponent::NAME_ActorFeatureName("PawnExtension");

UTalesPawnExtensionComponent::UTalesPawnExtensionComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);

	PawnData = nullptr;
	AbilitySystemComponent = nullptr;
}

void UTalesPawnExtensionComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UTalesPawnExtensionComponent, PawnData);
}

void UTalesPawnExtensionComponent::OnRegister()
{
	Super::OnRegister();

	const APawn* Pawn = GetPawn<APawn>();
	ensureAlwaysMsgf((Pawn != nullptr), TEXT("TalesPawnExtensionComp on [%s] can only be added to pawn actors"), *GetNameSafe(GetOwner()));

	TArray<UActorComponent*> PawnExtensionComponents;
	Pawn->GetComponents(UTalesPawnExtensionComponent::StaticClass(), PawnExtensionComponents);
	ensureAlwaysMsgf((PawnExtensionComponents.Num() == 1), TEXT("Only one ExtensionComponent should exist on [%s]"), *GetNameSafe(GetOwner()));

	// Register with the init state system early, this will only work if this is a game world
	RegisterInitStateFeature();
}

void UTalesPawnExtensionComponent::BeginPlay()
{
	Super::BeginPlay();

	// Listen for changes to all features
	BindOnActorInitStateChanged(NAME_None, FGameplayTag(), false);

	// Notifies state manager that we have spawned, then try rest of default init
	ensure(TryToChangeInitState(TalesGameplayTags::InitState_Spawned));
	CheckDefaultInitialization();
}

void UTalesPawnExtensionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnInitializeAbilitySystem();
	UnregisterInitStateFeature();
	Super::EndPlay(EndPlayReason);
}

void UTalesPawnExtensionComponent::OnRep_PawnData()
{
	CheckDefaultInitialization();
}

void UTalesPawnExtensionComponent::HandlePlayerStateReplicated()
{
	CheckDefaultInitialization();
}

void UTalesPawnExtensionComponent::SetupPlayerInputComponent()
{
	CheckDefaultInitialization();
}

void UTalesPawnExtensionComponent::OnAblitySystemInitialized_RegisterAndCall(
	FSimpleMulticastDelegate::FDelegate Delegate)
{
	if(!OnAbilitySystemInitialized.IsBoundToObject(Delegate.GetUObject()))
	{
		OnAbilitySystemInitialized.Add(Delegate);
	}
	if(AbilitySystemComponent)
	{
		Delegate.Execute();
	}
}

void UTalesPawnExtensionComponent::OnAblitySystemUninitialized_Register(FSimpleMulticastDelegate::FDelegate Delegate)
{
	if(!OnAbilitySystemUnInitialized.IsBoundToObject(Delegate.GetUObject()))
	{
		OnAbilitySystemInitialized.Add(Delegate);
	}
}

bool UTalesPawnExtensionComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager,
                                                      FGameplayTag CurrentState, FGameplayTag DesiredState) const
{
	check(Manager);

	APawn* Pawn = GetPawn<APawn>();
	if(!CurrentState.IsValid() && DesiredState == TalesGameplayTags::InitState_Spawned)
	{
		// As long as we are on a valid pawn , we count as spawned
		if(Pawn)
		{
			return true;
		}
	}

	if(CurrentState == TalesGameplayTags::InitState_Spawned && DesiredState == TalesGameplayTags::InitState_DataAvailable)
	{
		// Pawn data is required
		if(!PawnData)
		{
			return false;
		}

		const bool bHasAuthority = Pawn->HasAuthority();
		const bool bIsLocallyContolled = Pawn->IsLocallyControlled();

		if(bHasAuthority || bIsLocallyContolled)
		{
			// Check for being possessed by a controller
			if(!GetController<AController>())
			{
				return false;
			}
		}
		
		return true;
	}
	else if(CurrentState == TalesGameplayTags::InitState_DataAvailable && DesiredState == TalesGameplayTags::InitState_DataInitialized)
	{
		// Transition to initialize if all features have their data available
		return Manager->HaveAllFeaturesReachedInitState(Pawn, TalesGameplayTags::InitState_DataAvailable);
	}else if(CurrentState == TalesGameplayTags::InitState_DataInitialized && DesiredState == TalesGameplayTags::InitState_GameplayReady)
	{
		return true;	
	}
	return false;
}

void UTalesPawnExtensionComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager,
	FGameplayTag CurrentState, FGameplayTag DesiredState)
{
	if(DesiredState == TalesGameplayTags::InitState_DataInitialized)
	{
		// This is currently all handled by other components listening to this state change.
	}
}

void UTalesPawnExtensionComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	if(Params.FeatureName != NAME_ActorFeatureName)
	{
		if(Params.FeatureState == TalesGameplayTags::InitState_DataAvailable)
		{
			CheckDefaultInitialization();
		}
	}
}

void UTalesPawnExtensionComponent::CheckDefaultInitialization()
{
	// Before checking our progress, try progressing any other features we might depend on
	CheckDefaultInitializationForImplementers();

	static const TArray<FGameplayTag> StateChain = {TalesGameplayTags::InitState_Spawned, TalesGameplayTags::InitState_DataAvailable, TalesGameplayTags::InitState_DataInitialized, TalesGameplayTags::InitState_GameplayReady};
	ContinueInitStateChain(StateChain);
}

void UTalesPawnExtensionComponent::InitializeAbilitySystem(UTalesAbilitySystemCompBase* InASC, AActor* InOwnerActor)
{
	check(InASC);
	check(InOwnerActor);
	if(AbilitySystemComponent == InASC)
	{
		// The ablility system component hasn't changed
		return;
	}
	if(AbilitySystemComponent)
	{
		// Clean up the old ability system component
		UnInitializeAbilitySystem();
	}

	APawn* Pawn = GetPawnChecked<APawn>();
	AActor* ExistingAcatar = InASC->GetAvatarActor();

	UE_LOG(LogTales, Verbose, TEXT("Setting up ASC [%s] on pawn [%s] owner [%s], existing [%s] "), *GetNameSafe(InASC), *GetNameSafe(Pawn), *GetNameSafe(InOwnerActor), *GetNameSafe(ExistingAcatar));
	if((ExistingAcatar != nullptr) && (ExistingAcatar != Pawn))
	{
		UE_LOG(LogTales, Log, TEXT("Existing avatar (authority=%d)"), ExistingAcatar->HasAuthority() ? 1 : 0);

		// There is already a pawn acting as the ASC's avatar, so we need to kick it out
		// This can happen on clients if they're logged: their new pawn is spawned + possessed before the dead one is removed
		ensure(!ExistingAcatar->HasAuthority());
		if(UTalesPawnExtensionComponent* OtherExtensionComponent = FindPawnExtensionComponent(ExistingAcatar))
		{
			OtherExtensionComponent->UnInitializeAbilitySystem();	
		}
	}

	AbilitySystemComponent = InASC;
	AbilitySystemComponent->InitAbilityActorInfo(InOwnerActor, Pawn);

	if(ensure(PawnData))
	{
		// TODO: RelationshipMapping
	}

	OnAbilitySystemInitialized.Broadcast();
}

void UTalesPawnExtensionComponent::UnInitializeAbilitySystem()
{
	if(!AbilitySystemComponent)
	{
		return;
	}
	// Uninitialize the ASC if we're still the avatar actor(otherwise another pawn already did it when they become the avatar actor
	if(AbilitySystemComponent->GetAvatarActor() == GetOwner())
	{
		// TODO 不设置可以幸免的
		AbilitySystemComponent->CancelAbilities();
		AbilitySystemComponent->RemoveAllGameplayCues();
		if(AbilitySystemComponent->GetOwnerActor() != nullptr)
		{
			AbilitySystemComponent->SetAvatarActor(nullptr);
		}
		else
		{
			// 这块涉及到具体游戏的架构了
			// If the ASC Doesn't have a valid owner, we need to clear *all* actor info, not just the avator pairing
			AbilitySystemComponent->ClearActorInfo();
		}

		OnAbilitySystemInitialized.Broadcast();
	}
	AbilitySystemComponent = nullptr;
}

