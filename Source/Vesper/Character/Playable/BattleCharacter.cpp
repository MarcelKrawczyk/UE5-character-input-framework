#include "Vesper/Character/Playable/BattleCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

ABattleCharacter::ABattleCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ABattleCharacter::BeginPlay()
{
	Super::BeginPlay();
	CurrentStats = BaseStats;
}

void ABattleCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void ABattleCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
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
		
		Input->BindAction(FirstSkillAction, ETriggerEvent::Triggered, this, &ABattleCharacter::FirstSkill);
	}
}

void ABattleCharacter::FirstSkill()
{
	
}

