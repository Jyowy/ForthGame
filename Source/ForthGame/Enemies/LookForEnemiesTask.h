// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "LookForEnemiesTask.generated.h"

class AEnemyAIController;
class AEnemy;

/**
 * 
 */
UCLASS()
class FORTHGAME_API ULookForEnemiesTask : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

		/** starts this task, should return Succeeded, Failed or InProgress
		*  (use FinishLatentTask() when returning InProgress)
		* this function should be considered as const (don't modify state of object) if node is not instanced! */
		virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

	AEnemyAIController *AIController;
	TArray<AActor *> TargetsToIgnore;

protected:
	/** aborts this task, should return Aborted or InProgress
	*  (use FinishLatentAbort() when returning InProgress)
	* this function should be considered as const (don't modify state of object) if node is not instanced! */
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);

public:
	
};
