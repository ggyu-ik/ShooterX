#include "ShooterX/Character/SXNonPlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ShooterX/Controller/SXAIController.h"
#include "ShooterX/Animation/SXAnimInstance.h"
#include "ShooterX/Component/SXStatusComponent.h"
#include "ShooterX/UI/UW_HPText.h"
#include "ShooterX/Component/SXHPTextWidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "ShooterX/Character/SXPlayerCharacter.h"
#include "ShooterX/Game/SXPlayerState.h"

ASXNonPlayerCharacter::ASXNonPlayerCharacter()
	: bIsNowAttacking(false)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 0.1f;

	AIControllerClass = ASXAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	HPTextWidgetComponent = CreateDefaultSubobject<USXHPTextWidgetComponent>(TEXT("WidgetComponent"));
	HPTextWidgetComponent->SetupAttachment(GetRootComponent());
	HPTextWidgetComponent->SetRelativeLocation(FVector(0.f, 0.f, 100.f));
	// WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	// Billboard 방식으로 보이나, 주인공 캐릭터를 가리게됨. 또한 UI와 멀어져도 동일한 크기가 유지됨.
	HPTextWidgetComponent->SetWidgetSpace(EWidgetSpace::World);
	HPTextWidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ASXNonPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (false == IsPlayerControlled())
	{
		bUseControllerRotationYaw = false;

		GetCharacterMovement()->bOrientRotationToMovement = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->RotationRate = FRotator(0.f, 480.f, 0.f);

		GetCharacterMovement()->MaxWalkSpeed = 300.f;
		// 플레이어 캐릭터보다 느리게해서 못 쫒아오게끔 하기위함.
	}
}

float ASXNonPlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamageAmount = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (StatusComponent->IsDead() == true)
	{
		ASXAIController* AIController = Cast<ASXAIController>(GetController());
		if (IsValid(AIController) == true)
		{
			AIController->EndAI();
		}

		ASXPlayerCharacter* DamageCauserCharacter = Cast<ASXPlayerCharacter>(DamageCauser);
		if (IsValid(DamageCauserCharacter) == true)
		{
			ASXPlayerState* SPlayerState = Cast<ASXPlayerState>(DamageCauserCharacter->GetPlayerState());
			if (IsValid(SPlayerState) == true)
			{
				SPlayerState->AddCurrentKillCount(1);
			}
		}	
	}

	return FinalDamageAmount;
}

void ASXNonPlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (IsValid(HPTextWidgetComponent) == true)
	{
		FVector WidgetComponentLocation = HPTextWidgetComponent->GetComponentLocation();
		FVector LocalPlayerCameraLocation = UGameplayStatics::GetPlayerCameraManager(this, 0)->GetCameraLocation();
		HPTextWidgetComponent->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(WidgetComponentLocation, LocalPlayerCameraLocation));
	}
}

void ASXNonPlayerCharacter::SetHPTextWidget(UUW_HPText* InHPTextWidget)
{
	if (IsValid(InHPTextWidget) == true)
	{
		InHPTextWidget->InitializeHPTextWidget(StatusComponent);
		StatusComponent->OnCurrentHPChanged.AddUObject(InHPTextWidget, &UUW_HPText::OnCurrentHPChange);
		StatusComponent->OnMaxHPChanged.AddUObject(InHPTextWidget, &UUW_HPText::OnMaxHPChange);
	}
}

void ASXNonPlayerCharacter::BeginAttack()
{
	USXAnimInstance* AnimInstance = Cast<USXAnimInstance>(GetMesh()->GetAnimInstance());
	checkf(IsValid(AnimInstance) == true, TEXT("Invalid AnimInstance"));

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	if (IsValid(AnimInstance) == true && IsValid(AttackMeleeMontage) == true && AnimInstance->Montage_IsPlaying(AttackMeleeMontage) == false)
	{
		AnimInstance->Montage_Play(AttackMeleeMontage);

		bIsNowAttacking = true;

		if (OnAttackMontageEndedDelegate.IsBound() == false)
		{
			OnAttackMontageEndedDelegate.BindUObject(this, &ThisClass::EndAttack);
			AnimInstance->Montage_SetEndDelegate(OnAttackMontageEndedDelegate, AttackMeleeMontage);
		}
	}
}

void ASXNonPlayerCharacter::EndAttack(UAnimMontage* InMontage, bool bInterruped)
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	bIsNowAttacking = false;

	if (OnAttackMontageEndedDelegate.IsBound() == true)
	{
		OnAttackMontageEndedDelegate.Unbind();
	}
}
