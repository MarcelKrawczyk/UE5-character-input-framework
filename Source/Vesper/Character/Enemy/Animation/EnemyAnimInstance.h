#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimInstance.generated.h"

UCLASS()
class VESPER_API UEnemyAnimInstance : public UAnimInstance
{
    GENERATED_BODY()

public:
    // Updated every frame from the owning EnemyCharacter
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
    float Speed;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
    bool bIsAttacking;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
    bool bIsDefending;

    virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
