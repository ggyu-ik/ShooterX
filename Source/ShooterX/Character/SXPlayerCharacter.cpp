#include "SXPlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ShooterX/Input/SXInputConfig.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ShooterX/Animation/SXAnimInstance.h"
#include "Particles/ParticleSystemComponent.h"
#include "ShooterXPlayGround/ShooterXPlayGround/SXPlayerCharacterMaterialManager.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"

ASXPlayerCharacter::ASXPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = 400.f;
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->bInheritPitch = true;
	SpringArmComponent->bInheritYaw = true;
	SpringArmComponent->bInheritRoll = false;
	SpringArmComponent->bDoCollisionTest = true;
	SpringArmComponent->SetRelativeLocation(FVector(0.f, 25.f, 25.f));

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->bUsePawnControlRotation = false;

	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponent"));
	ParticleSystemComponent->SetupAttachment(RootComponent);
	ParticleSystemComponent->SetAutoActivate(false);
}

void ASXPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (IsValid(PlayerController) == true)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (IsValid(Subsystem) == true)
		{
			Subsystem->AddMappingContext(PlayerCharacterInputMappingContext, 0);
		}
	}

	const USXPlayerCharacterMaterialManager* CDO = GetDefault<USXPlayerCharacterMaterialManager>();
	int32 RandomIndex = FMath::RandRange(0, (CDO->PlayerCharacterMeshMaterialPaths.Num() / 2) - 1);
	RandomIndex *= 2; // 0번, 2번, 4번마다 머테리얼이 기본/블랙/화이트로 나뉘어짐.
	CurrentPlayerCharacterMeshMaterialPath01 = CDO->PlayerCharacterMeshMaterialPaths[RandomIndex];
	CurrentPlayerCharacterMeshMaterialPath02 = CDO->PlayerCharacterMeshMaterialPaths[RandomIndex + 1];
	AssetStreamableHandle = UAssetManager::GetStreamableManager().RequestAsyncLoad
	(
		{ CurrentPlayerCharacterMeshMaterialPath01, CurrentPlayerCharacterMeshMaterialPath02 },
		FStreamableDelegate::CreateLambda([this]() -> void
		{
			AssetStreamableHandle->ReleaseHandle();
			TSoftObjectPtr<UMaterialInstance> LoadedMaterialInstance01(CurrentPlayerCharacterMeshMaterialPath01);
			TSoftObjectPtr<UMaterialInstance> LoadedMaterialInstance02(CurrentPlayerCharacterMeshMaterialPath02);
			if (LoadedMaterialInstance01.IsValid() == true && LoadedMaterialInstance02.IsValid() == true)
			{
				GetMesh()->SetMaterial(0, LoadedMaterialInstance02.Get());
				GetMesh()->SetMaterial(1, LoadedMaterialInstance01.Get());
					// 반대로 꽂아줘야함.
			}
		}
		)
	);
}

void ASXPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (IsValid(EnhancedInputComponent) == true)
	{
		EnhancedInputComponent->BindAction(PlayerCharacterInputConfig->Move, ETriggerEvent::Triggered, this, &ThisClass::InputMove);
		EnhancedInputComponent->BindAction(PlayerCharacterInputConfig->Look, ETriggerEvent::Triggered, this, &ThisClass::InputLook);
		EnhancedInputComponent->BindAction(PlayerCharacterInputConfig->Jump, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(PlayerCharacterInputConfig->Jump, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		EnhancedInputComponent->BindAction(PlayerCharacterInputConfig->AttackMelee, ETriggerEvent::Started, this, &ThisClass::InputAttackMelee);
	}
}

void ASXPlayerCharacter::InputMove(const FInputActionValue& InValue)
{
	FVector2D MovementVector = InValue.Get<FVector2D>();

	const FRotator ControlRotation = GetController()->GetControlRotation();
	const FRotator ControlRotationYaw(0.f, ControlRotation.Yaw, 0.f);

	const FVector ForwardVector = FRotationMatrix(ControlRotationYaw).GetUnitAxis(EAxis::X);
	const FVector RightVector = FRotationMatrix(ControlRotationYaw).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardVector, MovementVector.X);
	AddMovementInput(RightVector, MovementVector.Y);
}

void ASXPlayerCharacter::InputLook(const FInputActionValue& InValue)
{
	FVector2D LookVector = InValue.Get<FVector2D>();

	AddControllerYawInput(LookVector.X);
	AddControllerPitchInput(LookVector.Y);
}

void ASXPlayerCharacter::InputAttackMelee(const FInputActionValue& InValue)
{
	if (GetCharacterMovement()->IsFalling())
	{
		return;
	}

	if (0 == CurrentComboCount)
	{
		BeginAttack();
	}
	else
	{
		ensure(FMath::IsWithinInclusive<int32>(CurrentComboCount, 1, MaxComboCount));
		bIsAttackKeyPressed = true;
	}
}

