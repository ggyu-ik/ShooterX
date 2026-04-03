#pragma once

#include "CoreMinimal.h"
#include "ShooterX/Controller/SXUIPlayerController.h"
#include "SXUIPlayerController_Loading.generated.h"

UCLASS()
class SHOOTERX_API ASXUIPlayerController_Loading : public ASXUIPlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
};
