// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy.h"
#include "Components/InputComponent.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	SetState(EEnemyState::Idle);

	Health = MaxHealth;
}

void AEnemy::SetState(EEnemyState NewState)
{
	State = NewState;
}

void AEnemy::TryToChangeState(EEnemyState NewState)
{
	if (State != NewState && State != EEnemyState::Dead)
	{
		SetState(NewState);
	}
}

EEnemyState AEnemy::GetState()
{
	return State;
}

void AEnemy::TryToSetState(EEnemyState NewState)
{
	TryToChangeState(NewState);
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemy::Death()
{
	TryToChangeState(EEnemyState::Dead);
	UE_LOG(LogTemp, Warning, TEXT("Enemy dead"));
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);
}

void AEnemy::UpdateHealth(float PHealth)
{
	Health = PHealth;
	if (Health == 0.0f)
	{
		Death();
	}
}

void AEnemy::ReceiveDamage(float PPower)
{
	float damage = PPower - Defense;
	float remainingHealth = FGenericPlatformMath::Max(0.0f, Health - damage);
	UE_LOG(LogTemp, Warning, TEXT("%.0f damage received"), damage);
	UpdateHealth(remainingHealth);
}

bool AEnemy::IsStillAlive()
{
	bool alive = State != EEnemyState::Dead;
	return alive;
}

EShootingResult AEnemy::ReceiveShoot(float PPower)
{
	EShootingResult shootingResult;

	if (PPower <= Defense)
	{
		shootingResult = EShootingResult::NoDamage;
	}
	else
	{
		ReceiveDamage(PPower);
		bool isDead = IsStillAlive();
		shootingResult = isDead ? EShootingResult::Kill : EShootingResult::Damage;
	}

	return shootingResult;
}