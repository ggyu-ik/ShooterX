#pragma once

#include "CoreMinimal.h"
#include "ShooterX/Character/SXCharacterBase.h"
#include "SXNonPlayerCharacter.generated.h"

class USXHPTextWidgetComponent;
class UUW_HPText;

DECLARE_DELEGATE_TwoParams(FOnAttackMontageEnded, UAnimMontage*, bool)

UCLASS()
class SHOOTERX_API ASXNonPlayerCharacter : public ASXCharacterBase
{
	GENERATED_BODY()

	friend class UBTTask_Attack;
	
public:
	ASXNonPlayerCharacter();

	virtual void BeginPlay() override;

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void SetHPTextWidget(UUW_HPText* InHPTextWidget);
	
protected:
	virtual void BeginAttack();

	virtual void EndAttack(UAnimMontage* InMontage, bool bInterruped);

public:
	bool bIsNowAttacking;

protected:
	FOnAttackMontageEnded OnAttackMontageEndedDelegate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<USXHPTextWidgetComponent> HPTextWidgetComponent;
};
