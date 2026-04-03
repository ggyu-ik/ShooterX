#include "SXWeapon.h"
#include "ShooterX/Component/SXPickupComponent.h"
#include "ShooterX/Character/SXPlayerCharacter.h"

ASXWeapon::ASXWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	PickupComponent = CreateDefaultSubobject<USXPickupComponent>(TEXT("PickupComponent"));
	SetRootComponent(PickupComponent);
}

void ASXWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	PickupComponent->OnPickUp.AddDynamic(this, &ThisClass::HandleOnPickUp);
}

void ASXWeapon::HandleOnPickUp(ASXPlayerCharacter* InPickUpCharacter)
{
	if (!IsValid(InPickUpCharacter))
	{
		return;
	}

	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(InPickUpCharacter->GetMesh(), AttachmentRules, FName(TEXT("hand_rSocket")));
	SetActorEnableCollision(false);
	PickupComponent->SetSimulatePhysics(false);
}