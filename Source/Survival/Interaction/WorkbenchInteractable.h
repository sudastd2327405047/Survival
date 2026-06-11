#pragma once

#include "CoreMinimal.h"
#include "InteractableActor.h"
#include "WorkbenchInteractable.generated.h"

class APawn;

UCLASS()
class SURVIVAL_API AWorkbenchInteractable : public AInteractableActor
{
	GENERATED_BODY()

protected:
	virtual void OnInteractTriggered_Implementation(APawn* InstigatorPawn) override;

	// 中文注释：给蓝图保留原有“打开制造UI”入口。
	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction|Workbench")
	void OnWorkbenchInteraction(APawn* InstigatorPawn);

	// 中文注释：新增仓库UI入口，便于你在同一工作台里组合制造+仓库界面逻辑。
	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction|Workbench")
	void OnWorkbenchStashRequested(APawn* InstigatorPawn);
};
