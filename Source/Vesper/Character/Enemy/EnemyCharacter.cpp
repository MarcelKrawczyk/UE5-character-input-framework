#include "Vesper/Character/Enemy/EnemyCharacter.h"
#include "Vesper/Character/Enemy/AI/EnemyAIComponent.h"

AEnemyCharacter::AEnemyCharacter()
{
    PrimaryActorTick.bCanEverTick = true;
    AIComponent = CreateDefaultSubobject<UEnemyAIComponent>(TEXT("EnemyAI"));
    bIsAttacking = false;
    bIsDefending = false;
}

void AEnemyCharacter::BeginPlay()
{
    Super::BeginPlay();
    CurrentStats = BaseStats;
}

void AEnemyCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

float AEnemyCharacter::ExecuteAction(EEnemyAction Action, FCharacterStats& TargetStats)
{
    bIsAttacking = false;
    bIsDefending = false;
    float DamageDealt = 0.f;

    switch (Action)
    {
    case EEnemyAction::Attack:
        {
            bIsAttacking = true;
            DamageDealt = CurrentStats.Strength;
            TargetStats.Health = FMath::Max(0.f, TargetStats.Health - DamageDealt);
            UE_LOG(LogTemp, Log, TEXT("[%s] Attack -> dealt %.0f dmg (target HP: %.0f)"),
                *EnemyName.ToString(), DamageDealt, TargetStats.Health);
            break;
        }
    case EEnemyAction::Defend:
        {
            bIsDefending = true;
            UE_LOG(LogTemp, Log, TEXT("[%s] Defend -> bracing"), *EnemyName.ToString());
            break;
        }
    case EEnemyAction::SpecialAttack:
        {
            bIsAttacking = true;
            DamageDealt = CurrentStats.Strength * 1.5f;
            TargetStats.Health = FMath::Max(0.f, TargetStats.Health - DamageDealt);
            UE_LOG(LogTemp, Log, TEXT("[%s] SpecialAttack -> dealt %.0f dmg (target HP: %.0f)"),
                *EnemyName.ToString(), DamageDealt, TargetStats.Health);
            break;
        }
    }
    return DamageDealt;
}

void AEnemyCharacter::ReceiveDamage(float Amount)
{
    if (bIsDefending)
    {
        Amount *= 0.5f;
    }
    CurrentStats.Health = FMath::Max(0.f, CurrentStats.Health - Amount);
    UE_LOG(LogTemp, Log, TEXT("[%s] Received %.0f dmg -> HP: %.0f"),
        *EnemyName.ToString(), Amount, CurrentStats.Health);
    bIsDefending = false;
}

bool AEnemyCharacter::IsDead() const
{
    return CurrentStats.Health <= 0.f;
}
