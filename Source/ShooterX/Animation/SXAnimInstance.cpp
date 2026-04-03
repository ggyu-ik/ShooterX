#include "SXAnimInstance.h"
#include "ShooterX/Character/SXCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "ShooterX/Character/SXNonPlayerCharacter.h"
#include "ShooterX/Component/SXStatusComponent.h"


void USXAnimInstance::NativeInitializeAnimation()
{
	APawn* OwnerPawn = TryGetPawnOwner();
	if (IsValid(OwnerPawn) == true)
	{
		OwnerCharacter = Cast<ASXCharacterBase>(OwnerPawn);
		OwnerCharacterMovement = OwnerCharacter->GetCharacterMovement();
	}
}

void USXAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (IsValid(OwnerCharacter) && IsValid(OwnerCharacterMovement))
	{
		Velocity = OwnerCharacterMovement->Velocity;
		GroundSpeed = UKismetMathLibrary::VSizeXY(Velocity);
		float GroundAcceleration = UKismetMathLibrary::VSizeXY(OwnerCharacterMovement->GetCurrentAcceleration());
		bool bIsAccelerationNearlyZero = FMath::IsNearlyZero(GroundAcceleration);
		bShouldMove = (KINDA_SMALL_NUMBER < GroundSpeed) && !(bIsAccelerationNearlyZero);
		
		if (ASXNonPlayerCharacter* OwnerNPC = Cast<ASXNonPlayerCharacter>(OwnerCharacter))
		{
			bShouldMove = KINDA_SMALL_NUMBER < GroundSpeed;
		}
		
		bIsFalling = OwnerCharacterMovement->IsFalling();
		
		if (IsValid(OwnerCharacter->GetStatusComponent()) == true)
		{
			bIsDead = OwnerCharacter->GetStatusComponent()->IsDead();
		}
	}
}

void USXAnimInstance::AnimNotify_PostDead()
{
	if (OnPostDead.IsBound())
	{
		OnPostDead.Broadcast();
	}
}
