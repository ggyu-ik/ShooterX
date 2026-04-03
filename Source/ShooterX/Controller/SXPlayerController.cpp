#include "SXPlayerController.h"
#include "ShooterX/UI/SXHUD.h"
#include "ShooterX/Game/SXPlayerState.h"
#include "ShooterX/Character/SXPlayerCharacter.h"
#include "ShooterX/Component/SXStatusComponent.h"

void ASXPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(HUDWidgetClass) == true)
	{
		HUDWidget = CreateWidget<USXHUD>(this, HUDWidgetClass);
		if (IsValid(HUDWidget) == true)
		{
			HUDWidget->AddToViewport();

			ASXPlayerState* SXPlayerState = GetPlayerState<ASXPlayerState>();
			if (IsValid(SXPlayerState) == true)
			{
				HUDWidget->BindPlayerState(SXPlayerState);
			}

			ASXPlayerCharacter* PC = GetPawn<ASXPlayerCharacter>();
			if (IsValid(PC) == true)
			{
				USXStatusComponent* StatusComponent = PC->GetStatusComponent();
				if (IsValid(StatusComponent) == true)
				{
					HUDWidget->BindStatusComponent(StatusComponent);
				}
			}
		}
	}
}
