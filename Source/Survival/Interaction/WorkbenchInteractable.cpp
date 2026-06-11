#include "WorkbenchInteractable.h"

#include "Survival/SurvivalGameInstance.h"

void AWorkbenchInteractable::OnInteractTriggered_Implementation(APawn* InstigatorPawn)
{
	Super::OnInteractTriggered_Implementation(InstigatorPawn);
	OnWorkbenchInteraction(InstigatorPawn);

	// 中文注释：交互触发时通知GI准备仓库数据，并派发给蓝图UI层。
	if (InstigatorPawn)
	{
		if (USurvivalGameInstance* SurvivalGI = InstigatorPawn->GetGameInstance<USurvivalGameInstance>())
		{
			SurvivalGI->InitializeStashIfNeeded();
			SurvivalGI->LoadStash();
			SurvivalGI->BP_RequestOpenStashUi();
		}
	}
	OnWorkbenchStashRequested(InstigatorPawn);
}
