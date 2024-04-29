// Fill out your copyright notice in the Description page of Project Settings.


#include "..\..\Public\Character\TalesCharacter.h"

#include "Components/AudioComponent.h"
#include "NiagaraComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Blueprint/UserWidget.h"
#include "Character/TalesCharacterMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Inventory/TalesInventorInteractUI.h"
#include "Inventory/TalesInventoryComponent.h"
#include "Kismet/GameplayStatics.h"

// TODO: Debug system include File, Delete later

// Sets default values
ATalesCharacter::ATalesCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UTalesCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

	TalesCharacterMovementComponent = Cast<UTalesCharacterMovementComponent>(GetCharacterMovement());
	TalesCharacterMovementComponent->SetIsReplicated(true);

	InventoryComponent = CreateDefaultSubobject<UTalesInventoryComponent>("TalesInventoryComponent");
	
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	// Sprint Setting
	SprintTimeLineComp = CreateDefaultSubobject<UTimelineComponent>("SprintTimeLineComp");
	SprintLineNiagaraComp = CreateDefaultSubobject<UNiagaraComponent>("SprintLineVFXComp");
	SprintLineNiagaraComp->SetupAttachment(CameraComp);
	SprintLineNiagaraComp->SetRelativeRotation(FRotator(270.f, 0.f, 0.f));
	SprintLineNiagaraComp->SetRelativeScale3D(FVector(3.f, 3.f, 3.f));

	// // JetPack Setting
	// JetPackComp = CreateDefaultSubobject<USkeletalMeshComponent>("JetPackComp");
	// JetPackComp->SetupAttachment(GetMesh(), TEXT("BackpackSocket"));
	//
	// JetPackThrusterComp = CreateDefaultSubobject<UNiagaraComponent>("JetPackThrusterComp");
	// JetPackThrusterComp->SetupAttachment(JetPackComp);
	//
	// JetPackThrusterAudioComp = CreateDefaultSubobject<UAudioComponent>("JetPackSFX");

	// MeshComponent
	SwardMesh = CreateDefaultSubobject<UStaticMeshComponent>("SwardMesh");
	SwardMesh->SetupAttachment(GetMesh(), TEXT("SwardSocket"));
	
	ShieldMesh = CreateDefaultSubobject<UStaticMeshComponent>("ShieldMesh");
	ShieldMesh->SetupAttachment(GetMesh(), TEXT("ShieldSocket"));
	
}

// Called when the game starts or when spawned
void ATalesCharacter::BeginPlay()
{
	Super::BeginPlay();

	AddInputMappingContext(PCInputMapping, 0);
	
	SprintLineNiagaraComp->Deactivate();
	// Add Mappings for our game, more complex games may have multiple Contexts that are added/removed at runtime.
	// JetPackThrusterComp->Deactivate();
	// JetPackThrusterAudioComp->Activate();
	// JetPackThrusterAudioComp->Stop();
	// Interact Component
	InteractUI = CreateWidget<UTalesInventorInteractUI>(UGameplayStatics::GetPlayerController(GetWorld(),0), InteractUIClass);
}

void ATalesCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	UpdateSprintFOVTrack.BindDynamic(this, &ATalesCharacter::UpdateSprintFov);

	//If we have a float curve, bind it's graph to our update function
	if(SprintFovChangeFloatCurve)
	{
		SprintTimeLineComp->AddInterpFloat(SprintFovChangeFloatCurve, UpdateSprintFOVTrack);
	}

	if(TalesCharacterMovementComponent)
	{
		TalesCharacterMovementComponent->OnEnterClimbStateDelegate.BindUObject(this, &ThisClass::OnPlayerEnterClimbState);
		TalesCharacterMovementComponent->OnExitClimbStateDelegate.BindUObject(this, &ThisClass::OnPlayerExitClimbState);
	}
}

void ATalesCharacter::AddInputMappingContext(UInputMappingContext* ContextToAdd, int32 InPriority)
{
	if(!ContextToAdd) return;

	if(APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(ContextToAdd, InPriority);
		}
	}
}

void ATalesCharacter::RemoveInputMappingContext(UInputMappingContext* ContextToAdd)
{
	if(!ContextToAdd) return;
	if(APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if(UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(ContextToAdd);
		}
	}
}

// Called to bind functionality to input
void ATalesCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	// New Enhanced Input System
	UEnhancedInputComponent* InputComp = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	// General
	if(ensureAlways(Input_Move))	 InputComp->BindAction(Input_Move,   ETriggerEvent::Triggered, this, &ATalesCharacter::MoveFunc);
	if(ensureAlways(Input_ClimbMove)) InputComp->BindAction(Input_ClimbMove, ETriggerEvent::Triggered, this, &ATalesCharacter::ClimbMoveFunc);
	if(ensureAlways(Input_Jump))	 InputComp->BindAction(Input_Jump,   ETriggerEvent::Triggered, this, &ATalesCharacter::Jump);
	if(ensureAlways(Input_ClimbHop)) InputComp->BindAction(Input_ClimbHop, ETriggerEvent::Triggered, this, &ATalesCharacter::ClimbHop);
	// if(ensureAlways(Input_Jump))	 InputComp->BindAction(Input_Jump,   ETriggerEvent::Completed, this, &ATalesCharacter::StopJumping);
	if(ensureAlways(Input_LookMouse))InputComp->BindAction(Input_LookMouse, ETriggerEvent::Triggered, this, &ATalesCharacter::LookMouse);
	
	// Sprint while key is held
	if(ensureAlways(Input_Sprint))	 InputComp->BindAction(Input_Sprint, ETriggerEvent::Started,   this, &ATalesCharacter::SprintStart);
	if(ensureAlways(Input_Sprint))	 InputComp->BindAction(Input_Sprint, ETriggerEvent::Completed, this, &ATalesCharacter::SprintStop);

}



void ATalesCharacter::OnPlayerEnterClimbState()
{
	ensureAlways(ClimbInputMapping);
	AddInputMappingContext(ClimbInputMapping, 1);	
}

void ATalesCharacter::OnPlayerExitClimbState()
{
	RemoveInputMappingContext(ClimbInputMapping);
}

void ATalesCharacter::MoveFunc(const FInputActionInstance& Instance)
{
	FRotator ControlRot = GetControlRotation();
	// ControlRot.Pitch = 0.f;
	// ControlRot.Roll  = 0.f;
	// GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, ControlRot.ToString());

	// Get Value from input (Combined value from up down right left keys) and convert to Vector2D
	const FVector2d AxisValue = Instance.GetValue().Get<FVector2d>();

	// Move forward / back
	const FVector ForVector = FRotationMatrix(ControlRot).GetUnitAxis(EAxis::X);
	AddMovementInput(ControlRot.Vector(), AxisValue.X);

	// Move Right / Left
	const FVector RightVector = FRotationMatrix(ControlRot).GetUnitAxis(EAxis::Y);
	AddMovementInput(RightVector, AxisValue.Y);
}

void ATalesCharacter::ClimbMoveFunc(const FInputActionInstance& Instance)
{
	FVector2d MovementVector = Instance.GetValue().Get<FVector2d>();

	const FVector ForwardDirection = FVector::CrossProduct(-TalesCharacterMovementComponent->GetClimbableSurfaceNormal(), GetActorRightVector());

	const FVector RightDirection = FVector::CrossProduct(-TalesCharacterMovementComponent->GetClimbableSurfaceNormal(), -GetActorUpVector());

	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);
}

void ATalesCharacter::SprintStart(const FInputActionInstance& Instance)
{
	// TODO: Can follow the method in the RPG tutorial to set it up later.
	// Now just complete in this function
	TalesCharacterMovementComponent->SprintPressed();
	if(GetVelocity().Length() != 0.f)
	{
		bIsSprint = true;
		// GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
		// GetCharacterMovement()->MaxAcceleration = SprintAcceleration;
		// TimeLine to Control Fov
		SprintTimeLineComp->Play();
		SprintLineNiagaraComp->Activate();
		if(ensureAlways(SprintShake))
		{
			CurrentSprintShake =  GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(SprintShake, 1);
		}
		// JetPackThrusterComp->Activate();
		// JetPackThrusterAudioComp->Play();
		// TODO: Add Camera Shake later, But not use today.

	}	
}

void ATalesCharacter::SprintStop(const FInputActionInstance& Instance)
{
	TalesCharacterMovementComponent->SprintReleased();
	if(bIsSprint)
	{
		// GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed;
		// GetCharacterMovement()->MaxAcceleration = DefaultAcceleration;
		bIsSprint = false;
		SprintTimeLineComp->Reverse();
		SprintLineNiagaraComp->Deactivate();
		if(CurrentSprintShake)
		{
			GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StopCameraShake(CurrentSprintShake, true);
			CurrentSprintShake = nullptr;
		}
		// JetPackThrusterComp->Deactivate();
		// JetPackThrusterAudioComp->Stop();
	}
}

void ATalesCharacter::LookMouse(const FInputActionInstance& Instance)
{
	const FVector2D Value = Instance.GetValue().Get<FVector2D>();
	AddControllerYawInput(Value.X);
	AddControllerPitchInput(Value.Y);
}

void ATalesCharacter::ClimbHop(const FInputActionInstance& Instance)
{
	if(TalesCharacterMovementComponent)
	{
		TalesCharacterMovementComponent->RequestHopping();
	}
}

void ATalesCharacter::Jump()
{
	bPressedTalesJump = true;
	Super::Jump();

	bPressedJump = false;
	// UE_LOG(LogTemp, Warning, TEXT("Jump is Server: %d"), HasAuthority());
}

void ATalesCharacter::StopJumping()
{
	bPressedTalesJump = false;
	Super::StopJumping();
}

void ATalesCharacter::UpdateSprintFov(float TimelineOutput)
{
	float CurrentFov = TimelineOutput * (SprintFov - DefaultFov) + DefaultFov;
	CameraComp->SetFieldOfView(CurrentFov);
}

void ATalesCharacter::ActivateInteractUI()
{
	if(InteractUI == nullptr)
	{
		InteractUI = CreateWidget<UTalesInventorInteractUI>(GetWorld(), InteractUIClass);	
	}
	
	if(!InteractUI->IsInViewport())
	{
		InteractUI->AddToViewport();
	}
}

void ATalesCharacter::UnActivateInteractUI()
{
	if(InteractUI)
	{
		InteractUI->RemoveFromParent();
	}
}

FCollisionQueryParams ATalesCharacter::GetIgnoreCharacterParams() const
{
	FCollisionQueryParams Params;

	TArray<AActor*> CharacterChileren;
	GetAllChildActors(CharacterChileren);
	Params.AddIgnoredActors(CharacterChileren);
	Params.AddIgnoredActor(this);

	return Params;
}

void ATalesCharacter::SetSwardMesh(UStaticMesh* InSwardMesh)
{
	this->SwardMesh->SetStaticMesh(InSwardMesh);
}

void ATalesCharacter::SetShieldMesh(UStaticMesh* InShieldMesh)
{
	this->ShieldMesh->SetStaticMesh(InShieldMesh);
}


// Called every frame
void ATalesCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

