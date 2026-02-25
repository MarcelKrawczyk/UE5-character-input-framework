// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplorationCharacter.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AExplorationCharacter::AExplorationCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("Spring Arm");
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);
	
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = false;
}

// Called when the game starts or when spawned
void AExplorationCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AExplorationCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AExplorationCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// SUBSYSTEM SETUP - Add input mapping context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMapping, 0);
		}
	}
	
	if (UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AExplorationCharacter::Move);
		
		Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &AExplorationCharacter::Look);
		
		Input->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AExplorationCharacter::Jump);
	}
}
	void AExplorationCharacter::Move(const FInputActionValue& InputValue)
	{
		FVector2D InputVector = InputValue.Get<FVector2D>();
	
		if (IsValid(Controller))
		{
			// FORWARD DIRECTION
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);
			
			const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
			const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
			
			// ADDMING MOVEMENT BASED BY THE INPUT
			AddMovementInput(ForwardDirection, InputVector.Y);
			AddMovementInput(RightDirection, InputVector.X);
		}
	}
	
	void AExplorationCharacter::Look(const FInputActionValue& InputValue)
	{
		FVector2D InputVector = InputValue.Get<FVector2D>();
		
		if (IsValid(Controller))
		{
			AddControllerYawInput(InputVector.X);
			AddControllerPitchInput(InputVector.Y);
		}
	}
	
	void AExplorationCharacter::Jump()
	{
		ACharacter::Jump();
	}