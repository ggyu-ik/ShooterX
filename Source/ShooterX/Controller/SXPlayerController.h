#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SXPlayerController.generated.h"

class USXHUD;

UCLASS()
class SHOOTERX_API ASXPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	USXHUD* GetHUDWidget() const {return HUDWidget;}
	
private:

	virtual void BeginPlay() override;
	
	UPROPERTY()
	TObjectPtr<USXHUD> HUDWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess))
	TSubclassOf<UUserWidget> HUDWidgetClass;
};
