// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "CheckEnemiesInSight.generated.h"

class AEnemy;
class AEnemyAIController;

/**
 * 
 */
UCLASS()
class FORTHGAME_API UCheckEnemiesInSight : public UBTService_BlackboardBase
{
	GENERATED_BODY()

		virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

	AEnemy *Enemy;
	AEnemyAIController *AIController;
	TArray<AActor *> TargetsToIgnore;

	float MaxAngleDistance;
	float VisionRange;

protected:

	virtual FString GetStaticServiceDescription() const override;
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = Blackboard)
		struct FBlackboardKeySelector EnemyTarget;
};
