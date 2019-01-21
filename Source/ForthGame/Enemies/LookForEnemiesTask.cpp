// Fill out your copyright notice in the Description page of Project Settings.

#include "LookForEnemiesTask.h"
#include "EnemyAIController.h"
#include "Enemy.h"

#include "Engine/World.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

EBTNodeResult::Type ULookForEnemiesTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type resultType = EBTNodeResult::InProgress;
	bNotifyTick = true;
	return resultType;
}

EBTNodeResult::Type ULookForEnemiesTask::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type resultType = EBTNodeResult::Succeeded;
	return resultType;
}

void ULookForEnemiesTask::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (AIController == NULL)
	{
		AIController = (AEnemyAIController *) OwnerComp.GetAIOwner();
	}

	AEnemy *enemy = (AEnemy *)AIController->GetPawn();

	if (TargetsToIgnore.Num() == 0)
	{
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), TargetsToIgnore);
	}

	TArray<AActor *> targets;


}