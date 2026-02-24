// Fill out your copyright notice in the Description page of Project Settings.


#include "Vesper/Systems/GameStateManager.h"
#include "Kismet/GameplayStatics.h"

void UGameStateManager::StartCombat(FName EnemyGroupID)
{
	// zapisz dane
	LastLevelName = "ExplorationWorld";
	// LastPlayerPosition = ...

	// przełącz level
	UGameplayStatics::OpenLevel(GetWorld(), "BattleArena");
}

void UGameStateManager::EndCombat(bool bPlayerWon)
{
	// aktualizuj dane

	// wróć do świata
	UGameplayStatics::OpenLevel(GetWorld(), *LastLevelName.ToString());
}

