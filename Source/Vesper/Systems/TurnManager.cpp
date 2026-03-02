#include "Vesper/Systems/TurnManager.h"
#include "Vesper/Character/Playable/BattleCharacter.h"
#include "Vesper/Character/Enemy/EnemyCharacter.h"
#include "Vesper/Character/Enemy/AI/EnemyAIComponent.h"

UTurnManager::UTurnManager()
{
    PrimaryComponentTick.bCanEverTick = false;
    CurrentPhase = ETurnPhase::PlayerTurn;
    TurnCount = 0;
    PlayerCharacter = nullptr;
    EnemyCharacter = nullptr;
}

void UTurnManager::BeginPlay()
{
    Super::BeginPlay();
}

void UTurnManager::StartBattle()
{
    if (!PlayerCharacter || !EnemyCharacter)
    {
        UE_LOG(LogTemp, Error, TEXT("[TurnManager] PlayerCharacter or EnemyCharacter is null!"));
        return;
    }
    TurnCount = 0;
    UE_LOG(LogTemp, Warning, TEXT("=== BATTLE START === Player HP: %.0f | Enemy HP: %.0f"),
        PlayerCharacter->CurrentStats.Health, EnemyCharacter->CurrentStats.Health);
    BeginPlayerTurn();
}

void UTurnManager::BeginPlayerTurn()
{
    TurnCount++;
    CurrentPhase = ETurnPhase::PlayerTurn;
    OnTurnChanged.Broadcast(CurrentPhase);
    UE_LOG(LogTemp, Warning, TEXT("--- Turn %d: PLAYER ---"), TurnCount);

#if !UE_BUILD_SHIPPING
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan,
            FString::Printf(TEXT("Turn %d: YOUR TURN"), TurnCount));
    }
#endif
}

void UTurnManager::EndPlayerTurn(float DamageDealtToEnemy)
{
    if (CurrentPhase != ETurnPhase::PlayerTurn)
    {
        UE_LOG(LogTemp, Warning, TEXT("[TurnManager] EndPlayerTurn called outside player turn."));
        return;
    }
    if (DamageDealtToEnemy > 0.f)
    {
        EnemyCharacter->ReceiveDamage(DamageDealtToEnemy);
    }
    if (EnemyCharacter->IsDead())
    {
        CurrentPhase = ETurnPhase::BattleOver;
        UE_LOG(LogTemp, Warning, TEXT("=== BATTLE OVER - PLAYER WINS ==="));
        OnBattleEnded.Broadcast(true);
        OnTurnChanged.Broadcast(CurrentPhase);
        return;
    }
    BeginEnemyTurn();
}

void UTurnManager::BeginEnemyTurn()
{
    CurrentPhase = ETurnPhase::EnemyTurn;
    OnTurnChanged.Broadcast(CurrentPhase);
    UE_LOG(LogTemp, Warning, TEXT("--- Turn %d: ENEMY ---"), TurnCount);

#if !UE_BUILD_SHIPPING
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red,
            FString::Printf(TEXT("Turn %d: ENEMY TURN"), TurnCount));
    }
#endif

    if (EnemyCharacter->AIComponent)
    {
        EEnemyAction Action = EnemyCharacter->AIComponent->ChooseAction(
            EnemyCharacter->CurrentStats, PlayerCharacter->CurrentStats);
        EnemyCharacter->ExecuteAction(Action, PlayerCharacter->CurrentStats);
    }
    CheckBattleOver();
}

void UTurnManager::CheckBattleOver()
{
    if (PlayerCharacter->CurrentStats.Health <= 0.f)
    {
        CurrentPhase = ETurnPhase::BattleOver;
        UE_LOG(LogTemp, Warning, TEXT("=== BATTLE OVER - ENEMY WINS ==="));
        OnBattleEnded.Broadcast(false);
        OnTurnChanged.Broadcast(CurrentPhase);
        return;
    }
    BeginPlayerTurn();
}
