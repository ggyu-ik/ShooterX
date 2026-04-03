#include "ShooterX/Component/SXHPTextWidgetComponent.h"
#include "ShooterX/UI/UW_HPText.h"

USXHPTextWidgetComponent::USXHPTextWidgetComponent()
{
}

void USXHPTextWidgetComponent::InitWidget()
{
	Super::InitWidget();

	UUW_HPText* HPTextWidgetInstance = Cast<UUW_HPText>(GetWidget());
	if (IsValid(HPTextWidgetInstance) == true && IsValid(GetOwner()) == true)
	{
		HPTextWidgetInstance->SetOwningActor(GetOwner());
	}

	/*
		왜 생성자에서 미리 OwningActor 속성을 초기화하지 않을까?
		UWidgetComponent::InitWidget() 함수 호출 이후에서야 오너 액터를 알 수 있기 때문.
		오너 액터의 BeginPlay() 함수 호출 이후에 비로소
		서브오브젝트인 위젯 컴포넌트의 InitWidget() 함수가 호출됨.
	*/
}
