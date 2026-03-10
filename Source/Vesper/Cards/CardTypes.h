#pragma once

#include "CoreMinimal.h"
#include "CardTypes.generated.h"

UENUM(BlueprintType)
enum class ECardTargetType : uint8
{
	None,
	Self,
	SingleEnemy,
	AllEnemies
};