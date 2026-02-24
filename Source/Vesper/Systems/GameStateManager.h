// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameStateManager.generated.h"

/**
 * 
 */
UCLASS()
class VESPER_API UGameStateManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintCallable)
	void StartCombat(FName EnemyGroupID);
	
	UFUNCTION(BlueprintCallable)
	void EndCombat(bool bPlayerWon);
	
	// GLOBAL DATA
	FVector LastPlayerPosition;
	FName LastLevelName;
};
