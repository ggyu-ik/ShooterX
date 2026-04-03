#pragma once

#include "CoreMinimal.h"
#include "SXCharacterBase.h"
#include "InputActionValue.h"
#include "SXPlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class USXInputConfig;
class UInputMappingContext;

struct FStreamableHandle;

UCLASS()
class SHOOTERX_API ASXPlayerCharacter : public ASXCharacterBase
{
	GENERATED_BODY()

#pragma region Override ACharacter

public:
	ASXPlayerCharacter();

	virtual void BeginPlay() override;

protected:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TObjectPtr<UCameraComponent> CameraComponent;

#pragma endregion

#pragma region Input

private:
	void InputMove(const FInputActionValue& InValue);

	void InputLook(const FInputActionValue& InValue);

	void InputAttackMelee(const FInputActionValue& InValue);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TObjectPtr<USXInputConfig> PlayerCharacterInputConfig;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess))
	TObjectPtr<UInputMappingContext> PlayerCharacterInputMappingContext;

#pragma endregion

#pragma region KillCount

public:
	UParticleSystemComponent* GetParticleSystem() const { return ParticleSystemComponent; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UParticleSystemComponent> ParticleSystemComponent;

#pragma endregion

#pragma region MeshMaterial

protected:
	FSoftObjectPath CurrentPlayerCharacterMeshMaterialPath01 = FSoftObjectPath();

	FSoftObjectPath CurrentPlayerCharacterMeshMaterialPath02 = FSoftObjectPath();

	TSharedPtr<FStreamableHandle> AssetStreamableHandle = nullptr;

#pragma endregion
};