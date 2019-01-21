// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class UCameraComponent;
class UInputComponent;
class UPlayerInventory;
class APlayerWeapon;
enum class EWeaponType : uint8;

UCLASS()
class FORTHGAME_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

		APlayerWeapon *LightWeapon;
	APlayerWeapon *MainWeapon;
	APlayerWeapon *MeleeWeapon;
	APlayerWeapon *HoldingWeapon;
	EWeaponType HoldingWeaponType;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
		class UPlayerInventory *Inventory;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
		class USceneComponent *RightHand;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
		class USceneComponent *LeftHand;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
		class UStaticMesh *ArmsMesh;

	APlayerCharacter();

protected:

	class UCameraComponent *Camera;

	FVector RightHandRelativePosition;
	FVector LeftHandRelativePosition;

	float CDeltaTime;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float axis);
	void MoveRight(float axis);
	void LookUp(float axis);
	void Turn(float axis);

	void StartJump();
	void EndJump();

	void InitializeWeapons();
	void SetLightWeapon();
	void SetMainWeapon();
	void SetMeleeWeapon();

	void HideCurrentWeapon();
	void SetNewWeapon(EWeaponType WeaponType);
	void ChangeWeapon(EWeaponType WeaponType);

	void Shoot(float axis);
	void SpecialShoot(float axis);

public:

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = Camera)
		float TurnSpeed;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = Camera)
		float LookUpSpeed;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void GetAimMarkerPosition(FVector &MarkerPosition);
	void GetForwardVector(FVector &ForwardVector);

};