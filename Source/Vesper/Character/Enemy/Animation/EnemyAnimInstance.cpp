#include "Vesper/Character/Enemy/Animation/EnemyAnimInstance.h"
#include "Vesper/Character/Enemy/EnemyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    APawn* OwnerPawn = TryGetPawnOwner();
    if (!OwnerPawn) return;

    // Speed from movement component
    Speed = OwnerPawn->GetVelocity().Size();

    // Read animation flags from EnemyCharacter
    if (AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(OwnerPawn))
    {
        bIsAttacking = Enemy->bIsAttacking;
        bIsDefending = Enemy->bIsDefending;
    }
}
