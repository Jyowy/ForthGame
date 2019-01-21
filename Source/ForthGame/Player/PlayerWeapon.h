// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "PlayerWeapon.generated.h"

UENUM(BlueprintType)
enum class EFiringType : uint8
{
	Normal	UMETA(DisplayName = "Standard"),
	Num		UMETA(Hidden)
};

class UTextRenderComponent;
class UParticleSystemComponent;
class APlayerCharacter;

UCLASS()
class FORTHGAME_API APlayerWeapon : public AActor
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = Mesh)
		class USceneComponent *CMuzzlePosition;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = Mesh)
		class USceneComponent *CLeftHandSocket;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = Mesh)
		class USceneComponent *CRightHandSocket;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = Mesh)
		class UParticleSystemComponent *ShootParticles;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
		class USoundBase *ShootingSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
		class UAudioComponent *ShootingAudio;

	APlayerWeapon();

	void SetPlayer(APlayerCharacter *PPlayer);

protected:

	float CDeltaTime;
	class APlayerCharacter *Player;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void NotEnoughAmmo();
	bool CheckEnoughAmmo();
	void UpdateAmmoUI();
	void SetAmmo(float PAmmo);
	void RefillAmmo();

	bool CheckShootRate();
	void UpdateShootingTimer();
	void Shoot();
	void SpecialShoot();

public:	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		class UTextRenderComponent *CAmmoText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		float Power;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		float MaxAmmo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		float Ammo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		float AmmoPerShot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		EFiringType FiringType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		float ShootingRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		float Range;
	float ShootingTimer;

	virtual void Tick(float DeltaTime) override;

	void TryToShoot();
	void TrySpecialShoot();

};
