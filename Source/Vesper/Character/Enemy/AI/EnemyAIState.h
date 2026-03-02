#pragma once

#include "CoreMinimal.h"
#include "EnemyAIState.generated.h"

// Possible AI behavioral states for the enemy
UENUM(BlueprintType)
enum class EEnemyAIState : uint8
{
    Aggressive   UMETA(DisplayName = "Aggressive"),
    Defensive    UMETA(DisplayName = "Defensive"),
    Random       UMETA(DisplayName = "Random")
};

// Possible actions the enemy can perform during its turn
UENUM(BlueprintType)
enum class EEnemyAction : uint8
{
    Attack          UMETA(DisplayName = "Attack"),
    Defend          UMETA(DisplayName = "Defend"),
    SpecialAttack   UMETA(DisplayName = "Special Attack")
};
