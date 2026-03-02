#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Vesper/Character/Stats/CharacterStats.h"
#include "Vesper/Character/Enemy/AI/EnemyAIState.h"
#include "EnemyCharacter.generated.h"

class UEnemyAIComponent;

UCLASS()
class VESPER_API AEnemyCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AEnemyCharacter();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    FCharacterStats BaseStats;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats")
    FCharacterStats CurrentStats;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Identity")
    FName EnemyName;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
    UEnemyAIComponent* AIComponent;

    UFUNCTION(BlueprintCallable, Category = "Combat")
    float ExecuteAction(EEnemyAction Action, FCharacterStats& TargetStats);

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void ReceiveDamage(float Amount);

    UFUNCTION(BlueprintPure, Category = "Combat")
    bool IsDead() const;

    UPROPERTY(BlueprintReadOnly, Category = "Animation")
    bool bIsAttacking;

    UPROPERTY(BlueprintReadOnly, Category = "Animation")
    bool bIsDefending;

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
