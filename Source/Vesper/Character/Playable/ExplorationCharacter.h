#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "ExplorationCharacter.generated.h"

UCLASS()
class VESPER_API AExplorationCharacter : public ACharacter
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	class  UCameraComponent* Camera;
	
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;

protected:
	UPROPERTY(EditAnywhere, Category= "EnhancedInput")
	class UInputMappingContext* InputMapping;

	UPROPERTY(EditAnywhere, Category= "EnhancedInput")
	class UInputAction* MoveAction;
	
	UPROPERTY(EditAnywhere, Category= "EnhancedInput")
	class UInputAction* JumpAction;
	
	UPROPERTY(EditAnywhere, Category= "EnhancedInput")
	class UInputAction* LookAction;
	
public:
	AExplorationCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
protected:
	void Move(const FInputActionValue& InputValue);
	void Look(const FInputActionValue& InputValue);
	void Jump();
};