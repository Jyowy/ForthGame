// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

class UInputComponent;

enum class EShootingResult : uint8
{
	NoDamage	UMETA(DisplayName = "No Damage"),
	Damage		UMETA(DisplayName = "Damage"),
	Kill		UMETA(DisplayName = "Kill"),
	Num			UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Idle		UMETA(DisplayName = "Idle"),
	Walking		UMETA(DisplayName = "Walking"),
	Running		UMETA(DisplayName = "Running"),
	Chasing		UMETA(DisplayName = "Chasing"),
	Searching	UMETA(DisplayName = "Searching"),
	Fighting	UMETA(DisplayName = "Fighting"),
	Dead		UMETA(DisplayName = "Dead"),
	Num			UMETA(Hidden)
};

UCLASS(BlueprintType)
class FORTHGAME_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemy();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enemy)
		float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enemy)
		float Power;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enemy)
		float Defense;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enemy)
		float VisionRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enemy)
		float VisionAngle;

protected:

	EEnemyState State;
	float Health;

	virtual void BeginPlay() override;

	void SetState(EEnemyState NewState);
	void TryToChangeState(EEnemyState NewState);

	void Death();
	void UpdateHealth(float PHealth);
	void ReceiveDamage(float PPower);
	bool IsStillAlive();

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = Enemy)
	EEnemyState GetState();
	UFUNCTION(BlueprintCallable, Category = Enemy)
	void TryToSetState(EEnemyState NewState);
	EShootingResult ReceiveShoot(float PPower);

};
