#include "Vesper/Character/Enemy/AI/EnemyAIComponent.h"

UEnemyAIComponent::UEnemyAIComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    CurrentState = EEnemyAIState::Random;
}

void UEnemyAIComponent::BeginPlay()
{
    Super::BeginPlay();
}

EEnemyAction UEnemyAIComponent::ChooseAction(const FCharacterStats& EnemyStats, const FCharacterStats& PlayerStats)
{
    CurrentState = EvaluateState(EnemyStats, PlayerStats);
    EEnemyAction ChosenAction = PickActionForState(CurrentState);

    UE_LOG(LogTemp, Warning,
        TEXT("[EnemyAI] State: %s | Action: %s | EnemyHP: %.0f | PlayerHP: %.0f"),
        *UEnum::GetValueAsString(CurrentState),
        *UEnum::GetValueAsString(ChosenAction),
        EnemyStats.Health,
        PlayerStats.Health);

#if !UE_BUILD_SHIPPING
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow,
            FString::Printf(TEXT("AI -> %s (%s)"),
                *UEnum::GetValueAsString(ChosenAction),
                *UEnum::GetValueAsString(CurrentState)));
    }
#endif

    return ChosenAction;
}

EEnemyAIState UEnemyAIComponent::EvaluateState(const FCharacterStats& EnemyStats, const FCharacterStats& PlayerStats) const
{
    constexpr float LowHPThreshold = 30.f;

    if (EnemyStats.Health < LowHPThreshold)
    {
        return EEnemyAIState::Defensive;
    }
    if (PlayerStats.Health < LowHPThreshold)
    {
        return EEnemyAIState::Aggressive;
    }
    return EEnemyAIState::Random;
}

EEnemyAction UEnemyAIComponent::PickActionForState(EEnemyAIState State) const
{
    switch (State)
    {
    case EEnemyAIState::Aggressive:
        {
            const float Roll = FMath::FRand();
            return (Roll < 0.7f) ? EEnemyAction::Attack : EEnemyAction::SpecialAttack;
        }
    case EEnemyAIState::Defensive:
        {
            const float Roll = FMath::FRand();
            if (Roll < 0.6f) return EEnemyAction::Defend;
            if (Roll < 0.85f) return EEnemyAction::Attack;
            return EEnemyAction::SpecialAttack;
        }
    case EEnemyAIState::Random:
    default:
        {
            const int32 Roll = FMath::RandRange(0, 2);
            switch (Roll)
            {
            case 0: return EEnemyAction::Attack;
            case 1: return EEnemyAction::Defend;
            default: return EEnemyAction::SpecialAttack;
            }
        }
    }
}
