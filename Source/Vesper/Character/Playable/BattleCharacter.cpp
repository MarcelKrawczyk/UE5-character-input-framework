#include "Vesper/Character/Playable/BattleCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Vesper/Systems/TurnManager.h"
#include "Kismet/GameplayStatics.h"

ABattleCharacter::ABattleCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	CachedTurnManager = nullptr;
}

void ABattleCharacter::BeginPlay()
{
	Super::BeginPlay();
	CurrentStats = BaseStats;

	// Find TurnManager in the level at startup
	FindTurnManager();
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
	// Guard: only act during player turn
	if (!CachedTurnManager)
	{
		FindTurnManager();
	}

	if (CachedTurnManager)
	{
		// Block input if it's not the player's turn or battle is over
		if (CachedTurnManager->CurrentPhase != ETurnPhase::PlayerTurn)
		{
			return;
		}
	}

	// Calculate damage from Strength stat
	const float Damage = CurrentStats.Strength;

	UE_LOG(LogTemp, Warning, TEXT("[BattleCharacter] FirstSkill activated! Dealing %.0f damage"), Damage);

#if !UE_BUILD_SHIPPING
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green,
			FString::Printf(TEXT("FirstSkill! Damage: %.0f"), Damage));
	}
#endif

	// Send damage to TurnManager to end player turn
	if (CachedTurnManager)
	{
		CachedTurnManager->EndPlayerTurn(Damage);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[BattleCharacter] No TurnManager found! Cannot end turn."));
	}
}

void ABattleCharacter::FindTurnManager()
{
	// Search all actors in the level for one with a TurnManager component
	TArray<AActor*> AllActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), AllActors);

	for (AActor* Actor : AllActors)
	{
		UTurnManager* TM = Actor->FindComponentByClass<UTurnManager>();
		if (TM)
		{
			CachedTurnManager = TM;
			UE_LOG(LogTemp, Log, TEXT("[BattleCharacter] Found TurnManager on actor: %s"), *Actor->GetName());
			return;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("[BattleCharacter] TurnManager not found in level!"));
}