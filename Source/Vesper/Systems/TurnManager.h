#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TurnManager.generated.h"

class ABattleCharacter;
class AEnemyCharacter;

UENUM(BlueprintType)
enum class ETurnPhase : uint8
{
    PlayerTurn   UMETA(DisplayName = "Player Turn"),
    EnemyTurn    UMETA(DisplayName = "Enemy Turn"),
    BattleOver   UMETA(DisplayName = "Battle Over")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTurnChanged, ETurnPhase, NewPhase);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBattleEnded, bool, bPlayerWon);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class VESPER_API UTurnManager : public UActorComponent
{
    GENERATED_BODY()

public:
    UTurnManager();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle")
    ABattleCharacter* PlayerCharacter;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle")
    AEnemyCharacter* EnemyCharacter;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Battle")
    ETurnPhase CurrentPhase;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Battle")
    int32 TurnCount;

    UPROPERTY(BlueprintAssignable, Category = "Battle|Events")
    FOnTurnChanged OnTurnChanged;

    UPROPERTY(BlueprintAssignable, Category = "Battle|Events")
    FOnBattleEnded OnBattleEnded;

    UFUNCTION(BlueprintCallable, Category = "Battle")
    void StartBattle();

    UFUNCTION(BlueprintCallable, Category = "Battle")
    void EndPlayerTurn(float DamageDealtToEnemy);

protected:
    virtual void BeginPlay() override;

private:
    void BeginPlayerTurn();
    void BeginEnemyTurn();
    void CheckBattleOver();
};
