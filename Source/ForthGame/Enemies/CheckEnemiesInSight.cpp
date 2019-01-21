// Fill out your copyright notice in the Description page of Project Settings.

#include "CheckEnemiesInSight.h"
#include "EnemyAIController.h"

#include "Enemy.h"
#include "../Player/PlayerCharacter.h"

#include "Engine/World.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Runtime/AIModule/Classes/BehaviorTree/Blackboard/BlackboardKeyType_Object.h"

void UCheckEnemiesInSight::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
	UBlackboardData* BBAsset = GetBlackboardAsset();
	if (ensure(BBAsset))
	{
		EnemyTarget.ResolveSelectedKey(*BBAsset);
	}
}

FString UCheckEnemiesInSight::GetStaticServiceDescription() const
{
	FString description;
	description = FString::Printf(TEXT("AI Enemy Service for checking enemies in sight"));
	return description;
}

void UCheckEnemiesInSight::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	// First initialization
	
	AIController = (AEnemyAIController *)OwnerComp.GetAIOwner();
	Enemy = (AEnemy *)AIController->GetPawn();

	if (TargetsToIgnore.Num() == 0)
	{
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), TargetsToIgnore);
	}
		
	VisionRange = Enemy->VisionRange;
	float maxAngle = Enemy->VisionAngle * 0.5f;
	float maxAngleRad = FMath::DegreesToRadians(maxAngle);
	MaxAngleDistance = FMath::Sin(maxAngleRad) / FMath::Sin(maxAngleRad * 0.5f);

	//

	TArray<AActor *> targets;
	FVector startLocation = Enemy->GetActorLocation();
	//DrawDebugLine(GetWorld(), startLocation, endLocation, FColor::Green, false, 2.0f, 0, 2.0f);

	FCollisionQueryParams visionSphereCheckParams = FCollisionQueryParams(FName(TEXT("Vision Check Trace")));
	visionSphereCheckParams.bTraceComplex = false;
	visionSphereCheckParams.bTraceAsyncScene = true;
	visionSphereCheckParams.bReturnPhysicalMaterial = false;
	visionSphereCheckParams.AddIgnoredActors(TargetsToIgnore);

	TArray<FOverlapResult> overlapResults;
	GetWorld()->OverlapMultiByChannel(overlapResults, startLocation, FQuat::Identity, ECollisionChannel::ECC_Pawn, FCollisionShape::MakeSphere(VisionRange), visionSphereCheckParams);
	//UE_LOG(LogTemp, Warning, TEXT("Number of overlap results = %d"), overlapResults.Num());

	APlayerCharacter *player = NULL;
	for (int i = 0; i < overlapResults.Num(); ++i)
	{
		AActor *actorHit = overlapResults[i].GetActor();
		if (actorHit != NULL && actorHit->IsA(APlayerCharacter::StaticClass()))
		{
			player = (APlayerCharacter *) actorHit;
			break;
		}
	}

	bool enemyLocated = false;

	if (player != NULL)
	{
		FVector target = player->GetActorLocation() - startLocation;
		target.Normalize();
		FVector forward = Enemy->GetActorForwardVector();
		float angleDistance = (target - forward).Size();
		if (angleDistance <= MaxAngleDistance)
		{
			enemyLocated = true;
		}
	}

	if (enemyLocated)
	{
		OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Object>(EnemyTarget.GetSelectedKeyID(), player);
		Enemy->TryToSetState(EEnemyState::Running);
		UE_LOG(LogTemp, Warning, TEXT("Enemy located!"));
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Object>(EnemyTarget.GetSelectedKeyID(), NULL);
		Enemy->TryToSetState(EEnemyState::Idle);
	}

}