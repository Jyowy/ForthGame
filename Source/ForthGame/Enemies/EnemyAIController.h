// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class FORTHGAME_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

protected:


public:
	UPROPERTY(BlueprintReadWrite, Category = AIController, meta = (ExposeOnSpawn = true))
		UBehaviorTree *BehaviorTree;

};
