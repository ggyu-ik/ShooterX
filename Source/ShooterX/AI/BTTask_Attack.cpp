// BTTask_Attack.cpp


#include "ShooterX/AI/BTTask_Attack.h"
#include "ShooterX/Controller/SXAIController.h"
#include "ShooterX/Character/SXNonPlayerCharacter.h"

UBTTask_Attack::UBTTask_Attack()
{
	bNotifyTick = true;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ASXAIController* AIController = Cast<ASXAIController>(OwnerComp.GetAIOwner());
	checkf(IsValid(AIController) == true, TEXT("Invalid AIController."));

	ASXNonPlayerCharacter* NPC = Cast<ASXNonPlayerCharacter>(AIController->GetPawn());
	checkf(IsValid(NPC) == true, TEXT("Invalid NPC."));

	if (NPC->bIsNowAttacking == false)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		/*
			공격 Task는 공격 애니메이션이 끝날 때까지 대기해야 하는 지연 Task임.
			공격 애니메이션 재생 동안에는 ExecuteTask()가 InProgress를 일단 반환하고 
			공격 애니메이션이 끝났을 때 태스크도 끝날 수 있게 알려줘야함.
			이를 알려주는 함수가 FinishLatentTask() 함수.
			Task에서 이 함수를 나중에 호출해주지 않으면
			비헤이비어 시스템은 해당 Task에 계속 머물게됨.
		*/
	}
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ASXAIController* AIController = Cast<ASXAIController>(OwnerComp.GetAIOwner());
	checkf(IsValid(AIController) == true, TEXT("Invalid AIController."));

	ASXNonPlayerCharacter* NPC = Cast<ASXNonPlayerCharacter>(AIController->GetPawn());
	checkf(IsValid(NPC) == true, TEXT("Invalid NPC."));

	NPC->BeginAttack();

	return EBTNodeResult::InProgress;
}
