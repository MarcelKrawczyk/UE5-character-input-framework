#pragma once

#include "CoreMinimal.h"
#include "CharacterStats.generated.h"

USTRUCT(BlueprintType)
struct FCharacterStats
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Health = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Mana = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Strength = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Agility = 10.f;
};
