#include "SXGameModeBase.h"
#include "ShooterX/Controller/SXPlayerController.h"

ASXGameModeBase::ASXGameModeBase()
{
	PlayerControllerClass = ASXPlayerController::StaticClass();
}
