#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Vesper/Character/Enemy/AI/EnemyAIState.h"
#include "Vesper/Character/Stats/CharacterStats.h"
#include "EnemyAIComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class VESPER_API UEnemyAIComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UEnemyAIComponent();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
    EEnemyAIState CurrentState;

    UFUNCTION(BlueprintCallable, Category = "AI")
    EEnemyAction ChooseAction(const FCharacterStats& EnemyStats, const FCharacterStats& PlayerStats);

protected:
    virtual void BeginPlay() override;

private:
    EEnemyAIState EvaluateState(const FCharacterStats& EnemyStats, const FCharacterStats& PlayerStats) const;
    EEnemyAction PickActionForState(EEnemyAIState State) const;
};
