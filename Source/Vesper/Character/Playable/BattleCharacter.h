#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Vesper/Character/Stats/CharacterStats.h"
#include "BattleCharacter.generated.h"

UCLASS()
class VESPER_API ABattleCharacter : public ACharacter
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	class  UCameraComponent* Camera;
	
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stats")
	FCharacterStats BaseStats;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Stats")
	FCharacterStats CurrentStats;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Cards")
	UCardBase* SelectedCard;
	
protected:
	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputMappingContext* InputMapping;
	
	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* FirstSkillAction;

public:
	ABattleCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION(BlueprintCallable)
	void SelectCard(UCardBase* Card);
	
protected:
	void FirstSkill();
};