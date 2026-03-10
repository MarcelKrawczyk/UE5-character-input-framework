#pragma once

#include "CoreMinimal.h"
#include "CardTypes.h"
#include "UObject/Object.h"
#include "CardBase.generated.h"

class ABattleCharacter;
class AEnemyCharacter;

UCLASS(Blueprintable, BlueprintType)
class VESPER_API UCardBase : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Card")
	FText CardName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Card")
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Card")
	int32 Cost = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Card")
	ECardTargetType TargetType = ECardTargetType::None;

public:

	// --- LOGIKA KARTY ---
	UFUNCTION(BlueprintNativeEvent, Category="Card")
	void Execute(ABattleCharacter* User, AEnemyCharacter* Target);
};
