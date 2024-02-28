// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/DCharacterBase.h"

#include "Components/AudioComponent.h"
#include "NiagaraComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// TODO: Debug system include File, Delete later

// Sets default values
ADCharacterBase::ADCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

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

	// JetPack Setting
	JetPackComp = CreateDefaultSubobject<USkeletalMeshComponent>("JetPackComp");
	JetPackComp->SetupAttachment(GetMesh(), TEXT("BackpackSocket"));
	
	JetPackThrusterComp = CreateDefaultSubobject<UNiagaraComponent>("JetPackThrusterComp");
	JetPackThrusterComp->SetupAttachment(JetPackComp);

	JetPackThrusterAudioComp = CreateDefaultSubobject<UAudioComponent>("JetPackSFX");
	// TODO: Params Setting
	SettingDefaultParams();
}

// Called when the game starts or when spawned
void ADCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	SprintLineNiagaraComp->Deactivate();
	JetPackThrusterComp->Deactivate();
	JetPackThrusterAudioComp->Activate();
	JetPackThrusterAudioComp->Stop();
}

void ADCharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	UpdateSprintFOVTrack.BindDynamic(this, &ADCharacterBase::UpdateSprintFov);

	//If we have a float curve, bind it's graph to our update function
	if(SprintFovChangeFloatCurve)
	{
		SprintTimeLineComp->AddInterpFloat(SprintFovChangeFloatCurve, UpdateSprintFOVTrack);
	}
}

// Called to bind functionality to input
void ADCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	const APlayerController* PC = GetController<APlayerController>();
	const ULocalPlayer* LP = PC->GetLocalPlayer();

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	Subsystem->ClearAllMappings();

	// Add Mappings for our game, more complex games may have multiple Contexts that are added/removed at runtime.
	Subsystem->AddMappingContext(PCInputMapping, 0);

	// New Enhanced Input System
	UEnhancedInputComponent* InputComp = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	// General
	if(ensureAlways(Input_Move))	 InputComp->BindAction(Input_Move,   ETriggerEvent::Triggered, this, &ADCharacterBase::MoveFunc);
	if(ensureAlways(Input_Jump))	 InputComp->BindAction(Input_Jump,   ETriggerEvent::Triggered, this, &ADCharacterBase::Jump);

	// Sprint while key is held
	if(ensureAlways(Input_Sprint))	 InputComp->BindAction(Input_Sprint, ETriggerEvent::Started,   this, &ADCharacterBase::SprintStart);
	if(ensureAlways(Input_Sprint))	 InputComp->BindAction(Input_Sprint, ETriggerEvent::Completed, this, &ADCharacterBase::SprintStop);
}


void ADCharacterBase::MoveFunc(const FInputActionInstance& Instance)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.f;
	ControlRot.Roll  = 0.f;

	// Get Value from input (Combined value from up down right left keys) and convert to Vector2D
	const FVector2d AxisValue = Instance.GetValue().Get<FVector2d>();

	// Move forward / back
	AddMovementInput(ControlRot.Vector(), AxisValue.X);

	// Move Right / Left
	const FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);
	AddMovementInput(RightVector, AxisValue.Y);
}

void ADCharacterBase::SprintStart(const FInputActionInstance& Instance)
{
	// TODO: Can follow the method in the RPG tutorial to set it up later.
	// Now just complete in this function
	if(GetVelocity().Length() != 0.f)
	{
		bIsSprint = true;
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
		GetCharacterMovement()->MaxAcceleration = SprintAcceleration;
		// TimeLine to Control Fov
		SprintTimeLineComp->Play();
		SprintLineNiagaraComp->Activate();
		if(ensureAlways(SprintShake))
		{
			CurrentSprintShake =  GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(SprintShake, 1);
		}
		JetPackThrusterComp->Activate();
		JetPackThrusterAudioComp->Play();
		// TODO: Add Camera Shake later, But not use today.

	}	
}

void ADCharacterBase::SprintStop(const FInputActionInstance& Instance)
{
	if(bIsSprint)
	{
		GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed;
		GetCharacterMovement()->MaxAcceleration = DefaultAcceleration;
		bIsSprint = false;
		SprintTimeLineComp->Reverse();
		SprintLineNiagaraComp->Deactivate();
		if(CurrentSprintShake)
		{
			GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StopCameraShake(CurrentSprintShake, true);
			CurrentSprintShake = nullptr;
		}
		JetPackThrusterComp->Deactivate();
		JetPackThrusterAudioComp->Stop();
	}
}

void ADCharacterBase::UpdateSprintFov(float TimelineOutput)
{
	float CurrentFov = TimelineOutput * (SprintFov - DefaultFov) + DefaultFov;
	CameraComp->SetFieldOfView(CurrentFov);
}

// Called every frame
void ADCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// TODO: Delete later
void ADCharacterBase::SettingDefaultParams()
{
	GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed;
	GetCharacterMovement()->MaxAcceleration = DefaultAcceleration;
	CameraComp->SetFieldOfView(DefaultFov);
}
