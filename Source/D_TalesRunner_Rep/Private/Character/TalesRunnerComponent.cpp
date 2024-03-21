// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TalesRunnerComponent.h"

#include "EnhancedInputSubsystems.h"
#include "AttributeSystem/TalesAbilitySystemCompBase.h"
#include "Character/TalesPawnExtensionComponent.h"
#include "Components/GameFrameworkComponentManager.h"
#include "GameFramework/PlayerState.h"
#include "Input/TalesInputComponent.h"
#include "Input/TalesMappableConfigPair.h"
#include "GamePlay/TalesPlayerControllerBase.h"
#include "System/TalesGameplayTags.h"
#include "System/TalesLogChannels.h"

const FName UTalesRunnerComponent::NAME_BindInputsNow("BindInputsNow");
const FName UTalesRunnerComponent::NAME_ActorFeatureName("Runner");

UTalesRunnerComponent::UTalesRunnerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	bReadyToBindInputs = false;
}

bool UTalesRunnerComponent::IsReadyToBindInputs() const
{
	return bReadyToBindInputs;
}

void UTalesRunnerComponent::InitializePlayerInput(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	const APawn* Pawn = GetPawn<APawn>();
	if(!Pawn)
	{
		return;
	}
	const APlayerController* PC = GetController<APlayerController>();
	check(PC);

	const ULocalPlayer* LP = PC->GetLocalPlayer();
	check(LP);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	Subsystem->ClearAllMappings();
	if(const UTalesPawnExtensionComponent* PawnExtComp = UTalesPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		if(const UTalesPawnData* PawnData = PawnExtComp->GetPawnData<UTalesPawnData>())
		{
			if(const UTalesInputConfig* InputConfig = PawnData->InputConfig)
			{
				// Register any default input configs with the settings so that they will be applied to the player duringInputMappings
				for(const FMappableConfigPair& Pair : DefaultInputConfig)
				{
					FModifyContextOptions Options = {};
					Options.bIgnoreAllPressedKeysUntilRelease = false;

					Subsystem->AddPlayerMappableConfig(Pair.Config.LoadSynchronous(), Options);
				}
				
				UTalesInputComponent* TalesIC = Cast<UTalesInputComponent>(PlayerInputComponent);
				if(ensureAlways(TalesIC))
				{
					TArray<uint32> BindHandles;
					TalesIC->BindAbilityActions(InputConfig, this, &ThisClass::Input_AbilityInputTagPressed, &ThisClass::Input_AbilityInputTagReleased, /*out*/ BindHandles);

					TalesIC->BindNativeAction(InputConfig, TalesGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move, /*bLogIfNotFound*/ false);
				}
			}
		}

	}
	if(ensure(!bReadyToBindInputs))
	{
		bReadyToBindInputs = true;
	}

	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(const_cast<APlayerController*>(PC), NAME_BindInputsNow);
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(const_cast<APawn*>(Pawn), NAME_BindInputsNow);
}

bool UTalesRunnerComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState,
                                               FGameplayTag DesiredState) const
{
	check(Manager);

	APawn* Pawn = GetPawn<APawn>();

	if(!CurrentState.IsValid() && DesiredState == TalesGameplayTags::InitState_Spawned)
	{
		// As long as we have a real pawn, let us transition
		if(Pawn)
		{
			return true;
		}
	}
	else if(CurrentState == TalesGameplayTags::InitState_Spawned && DesiredState == TalesGameplayTags::InitState_DataAvailable)
	{
		// @TODO: Player State
		// if(!GetPlayerState<APlayerState>())
		// {
		// 	return false;
		// }

		// if we're authority or autonomous, we need to wait for a controller with register
		if(Pawn->GetLocalRole() != ROLE_SimulatedProxy)
		{
			AController* Controller = GetController<AController>();

			const bool bHasControllerPairedWithPS = (Controller != nullptr) && \
				(Controller->PlayerState != nullptr) && \
				(Controller->PlayerState->GetOwner() == Controller);

			if(!bHasControllerPairedWithPS)
			{
				return false;
			}
		}

		const bool bIsLocallyControlled = Pawn->IsLocallyControlled();
		const bool bIsBot = Pawn->IsBotControlled();

		if(bIsLocallyControlled && !bIsBot)
		{
			ATalesPlayerControllerBase* TalesPC = GetController<ATalesPlayerControllerBase>();

			// The input component and local player is required when locally controlled;
			if(!Pawn->InputComponent || !TalesPC || !TalesPC->GetLocalPlayer())
			{
				return false;
			}
		}

		return true;
	}
	else if(CurrentState == TalesGameplayTags::InitState_DataAvailable && DesiredState == TalesGameplayTags::InitState_DataInitialized)
	{
		// @TODO playerstate
		APlayerState* PS = GetPlayerState<APlayerState>();
		return PS && Manager->HasFeatureReachedInitState(Pawn, UTalesPawnExtensionComponent::NAME_ActorFeatureName, TalesGameplayTags::InitState_DataInitialized);
	}
	else if(CurrentState == TalesGameplayTags::InitState_DataInitialized && DesiredState == TalesGameplayTags::InitState_GameplayReady)
	{
		return true;	
	}
	return false;	
}

void UTalesRunnerComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState,
	FGameplayTag DesiredState)
{
	APawn* Pawn = GetPawn<APawn>();
	if(Pawn->InputComponent != nullptr)
	{
		InitializePlayerInput(Pawn->InputComponent);
	}
	if(CurrentState == TalesGameplayTags::InitState_DataAvailable && DesiredState == TalesGameplayTags::InitState_DataInitialized)
	{
		Pawn = GetPawn<APawn>();
		APlayerState* PS = GetPlayerState<APlayerState>();
		if(!ensure(Pawn && PS))
		{
			return;
		}

		const bool bIsLocallyControlled = Pawn->IsLocallyControlled();
		const UTalesPawnData* PawnData = nullptr;

		if(UTalesPawnExtensionComponent* PawnExtComp = UTalesPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
		{
			PawnData = PawnExtComp->GetPawnData<UTalesPawnData>();

			// The player state holds the persistent data for this player (state that persists across deaths and multiple pawns).
			// The ability system component and attribute sets live on the player state.
			// TODO PlayerState
		}

		if(ATalesPlayerControllerBase* TalesPC = GetController<ATalesPlayerControllerBase>())
		{
			if(Pawn->InputComponent != nullptr)
			{
				InitializePlayerInput(Pawn->InputComponent);
			}
		}

		if(bIsLocallyControlled && PawnData)
		{
			// TODO CameraComponent
		}

	}
}

void UTalesRunnerComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	if(Params.FeatureName == UTalesPawnExtensionComponent::NAME_ActorFeatureName)
	{
		if(Params.FeatureState == TalesGameplayTags::InitState_DataInitialized)
		{
			CheckDefaultInitialization();
		}
	}
}

void UTalesRunnerComponent::CheckDefaultInitialization()
{
	// static const TArray<FGameplayTag> StateChain = { TalesGameplayTags::InitState_Spawned,
	// 	                                             TalesGameplayTags::InitState_DataAvailable,
	// 												 TalesGameplayTags::InitState_DataInitialized,
	// 	                                             TalesGameplayTags::InitState_GameplayReady };

	static const TArray<FGameplayTag> StateChain = {TalesGameplayTags::InitState_Spawned, TalesGameplayTags::InitState_DataAvailable, TalesGameplayTags::InitState_DataInitialized, TalesGameplayTags::InitState_GameplayReady};
	// This will try to progress from spawned (which is only set in BeginPlay) through the data initialization stages until it gets to gameplay ready
	ContinueInitStateChain(StateChain);
}

void UTalesRunnerComponent::OnRegister()
{
	Super::OnRegister();

	if(!GetPawn<APawn>())
	{
		UE_LOG(LogTales, Error, TEXT("[ULyraHeroComponent::OnRegister] This component has been added to a blueprint whose base class is not a Pawn. To use this component, it MUST be placed on a Pawn Blueprint."));
	}
	else
	{
		// Register with the init state system early, this will only work if this is a game wrold
		RegisterInitStateFeature();
	}
}

void UTalesRunnerComponent::BeginPlay()
{
	Super::BeginPlay();

	// Listen for when the pawn extension comp changes init state
	BindOnActorInitStateChanged(UTalesRunnerComponent::NAME_ActorFeatureName, FGameplayTag(), false);

	// Notifies that we are done spawning then try next
	ensure(TryToChangeInitState(TalesGameplayTags::InitState_Spawned));
	CheckDefaultInitialization();
}

void UTalesRunnerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnregisterInitStateFeature();
	Super::EndPlay(EndPlayReason);
}

void UTalesRunnerComponent::Input_AbilityInputTagPressed(FGameplayTag InputTag)
{
	if(const APawn* Pawn = GetPawn<APawn>())
	{
		if(const UTalesPawnExtensionComponent* PawnExtComp = UTalesPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
		{
			if(UTalesAbilitySystemCompBase* TalesASC = PawnExtComp->GetTalesAbilitySystemCompBase())
			{
				// TODO
			}
		}
	}
}

void UTalesRunnerComponent::Input_AbilityInputTagReleased(FGameplayTag InputTag)
{
	const APawn* Pawn = GetPawn<APawn>();
	if(!Pawn)
	{
		return;
	}

	if(const UTalesPawnExtensionComponent* PawnExtComp = UTalesPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		if(UTalesAbilitySystemCompBase* TalesASC = PawnExtComp->GetTalesAbilitySystemCompBase())
		{
			// TODO
		}
	}
}

void UTalesRunnerComponent::Input_Move(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();
	
	AController* Controller = Pawn ? Pawn->GetController() : nullptr;

	// If the player has attempted to move again then cancel auto running
	if (ATalesPlayerControllerBase* TalesController = Cast<ATalesPlayerControllerBase>(Controller))
	{
		// LyraController->SetIsAutoRunning(false);
	}
	
	if (Controller)
	{
		const FVector2D Value = InputActionValue.Get<FVector2D>();
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, *Value.ToString());
		const FRotator MovementRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);

		if (Value.X != 0.0f)
		{
			// const FVector MovementDirection = MovementRotation.RotateVector(FVector::RightVector);
			Pawn->AddMovementInput(MovementRotation.Vector(), Value.X);
		}

		if (Value.Y != 0.0f)
		{
			const FVector MovementDirection = FRotationMatrix(MovementRotation).GetScaledAxis(EAxis::Y);
			Pawn->AddMovementInput(MovementDirection, Value.Y);
		}
	}
}
