// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
#include "PlayerInventory.h"
#include "PlayerWeapon.h"
#include "Engine/World.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (Inventory == NULL)
	{
		UE_LOG(LogTemp, Error, TEXT("Player has no inventory"));
	}
	if (RightHand == NULL)
	{
		UE_LOG(LogTemp, Error, TEXT("Player has no right hand socket"));
	}
	else
	{
		RightHandRelativePosition = RightHand->RelativeLocation;
	}
	if (LeftHand == NULL)
	{
		UE_LOG(LogTemp, Error, TEXT("Player has no left hand socket"));
	}
	else
	{
		LeftHandRelativePosition = LeftHand->RelativeLocation;
	}

	Camera = FindComponentByClass<UCameraComponent>();
	if (Camera == NULL)
	{
		UE_LOG(LogTemp, Error, TEXT("Player has no camera component"));
	}

	InitializeWeapons();
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CDeltaTime = DeltaTime;
}

void APlayerCharacter::MoveForward(float axis)
{
	FVector forward = GetActorForwardVector();
	forward.Z = 0.0f;
	AddMovementInput(forward, axis);
}

void APlayerCharacter::MoveRight(float axis)
{
	FVector right = GetActorRightVector();
	right.Z = 0.0f;
	AddMovementInput(right, axis);
}

void APlayerCharacter::LookUp(float axis)
{
	float LookUpRotation = axis * LookUpSpeed * CDeltaTime;
	AddControllerPitchInput(LookUpRotation);
	//UE_LOG(LogTemp, Warning, TEXT("LookUp %.5f, %.5f, %.0f"), axis, LookUpRotation, LookUpSpeed);
}

void APlayerCharacter::Turn(float axis)
{
	float TurnRotation = axis * TurnSpeed * CDeltaTime;
	AddControllerYawInput(TurnRotation);
	//UE_LOG(LogTemp, Warning, TEXT("Turn %.5f, %.5f, %.0f"), axis, TurnRotation, TurnSpeed);
}

void APlayerCharacter::StartJump()
{
	Jump();
}

void APlayerCharacter::EndJump()
{
	StopJumping();
}

void APlayerCharacter::InitializeWeapons()
{
	TSubclassOf<APlayerWeapon> weapon;
	USceneComponent *scene = GetRootComponent();
	EAttachmentRule attachmentRules = EAttachmentRule::KeepRelative;
	FAttachmentTransformRules rules(EAttachmentRule::KeepRelative, true);

	weapon = Inventory->GetWeapon(EWeaponType::LightWeapon);
	if (weapon != NULL)
	{
		LightWeapon = GetWorld()->SpawnActor<APlayerWeapon>(weapon);
		LightWeapon->AttachToComponent(scene, rules);
		LightWeapon->SetActorEnableCollision(false);
		LightWeapon->SetActorHiddenInGame(true);
		LightWeapon->SetPlayer(this);
	}
	weapon = Inventory->GetWeapon(EWeaponType::MainWeapon);
	if (weapon != NULL)
	{
		MainWeapon = GetWorld()->SpawnActor<APlayerWeapon>(weapon);
		MainWeapon->AttachToComponent(scene, rules);
		MainWeapon->SetActorEnableCollision(false);
		MainWeapon->SetActorHiddenInGame(true);
		MainWeapon->SetPlayer(this);
	}
	weapon = Inventory->GetWeapon(EWeaponType::MeleeWeapon);
	if (weapon != NULL)
	{
		MeleeWeapon = GetWorld()->SpawnActor<APlayerWeapon>(weapon);
		MeleeWeapon->AttachToComponent(scene, rules);
		MeleeWeapon->SetActorEnableCollision(false);
		MeleeWeapon->SetActorHiddenInGame(true);
		MeleeWeapon->SetPlayer(this);
	}

	SetNewWeapon(EWeaponType::MainWeapon);
}

void APlayerCharacter::HideCurrentWeapon()
{
	switch (HoldingWeaponType)
	{
	case EWeaponType::LightWeapon:
		LightWeapon->SetActorHiddenInGame(true);
		break;
	case EWeaponType::MainWeapon:
		MainWeapon->SetActorHiddenInGame(true);
		break;
	case EWeaponType::MeleeWeapon:
		MeleeWeapon->SetActorHiddenInGame(true);
		break;
	default:
		break;
	}
}

void APlayerCharacter::SetNewWeapon(EWeaponType WeaponType)
{
	APlayerWeapon *weapon;

	switch (WeaponType)
	{
	case EWeaponType::LightWeapon:
		LightWeapon->SetActorHiddenInGame(false);
		weapon = LightWeapon;
		break;
	case EWeaponType::MainWeapon:
		MainWeapon->SetActorHiddenInGame(false);
		weapon = MainWeapon;
		break;
	case EWeaponType::MeleeWeapon:
		MeleeWeapon->SetActorHiddenInGame(false);
		weapon = MeleeWeapon;
		break;
	default:
		weapon = NULL;
		break;
	}

	HoldingWeaponType = WeaponType;
	HoldingWeapon = weapon;

	if (weapon != NULL)
	{
		//USceneComponent *weaponLeftSocket = weapon->CLeftHandSocket;
		//FVector relativeLocation = LeftHandRelativePosition - weaponLeftSocket->RelativeLocation;
		//weapon->SetActorRelativeLocation(relativeLocation);
		USceneComponent *weaponRightSocket = weapon->CRightHandSocket;
		FVector relativeLocation = RightHandRelativePosition - weaponRightSocket->RelativeLocation;
		weapon->SetActorRelativeLocation(relativeLocation);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("SetNewWeapon - Non weapon was found"));
	}
}

void APlayerCharacter::ChangeWeapon(EWeaponType WeaponType)
{
	if (HoldingWeaponType != WeaponType)
	{
		HideCurrentWeapon();
		SetNewWeapon(WeaponType);
	}
}

void APlayerCharacter::SetLightWeapon()
{
	ChangeWeapon(EWeaponType::LightWeapon);
}

void APlayerCharacter::SetMainWeapon()
{
	ChangeWeapon(EWeaponType::MainWeapon);
}

void APlayerCharacter::SetMeleeWeapon()
{
	ChangeWeapon(EWeaponType::MeleeWeapon);
}

void APlayerCharacter::Shoot(float axis)
{
	if (axis > 0.0f)
	{
		HoldingWeapon->TryToShoot();
	}
}

void APlayerCharacter::SpecialShoot(float axis)
{
	if (axis > 0.0f)
	{
		HoldingWeapon->TrySpecialShoot();
	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerCharacter::LookUp);
	PlayerInputComponent->BindAxis("Turn", this, &APlayerCharacter::Turn);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacter::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &APlayerCharacter::EndJump);

	PlayerInputComponent->BindAction("LightWeapon", IE_Pressed, this, &APlayerCharacter::SetLightWeapon);
	PlayerInputComponent->BindAction("MainWeapon", IE_Pressed, this, &APlayerCharacter::SetMainWeapon);
	PlayerInputComponent->BindAction("MeleeWeapon", IE_Pressed, this, &APlayerCharacter::SetMeleeWeapon);

	PlayerInputComponent->BindAxis("Shoot", this, &APlayerCharacter::Shoot);
	PlayerInputComponent->BindAxis("SpecialShoot", this, &APlayerCharacter::SpecialShoot);

}

void APlayerCharacter::GetAimMarkerPosition(FVector &MarkerPosition)
{
	//MarkerPosition = GetActorLocation() + Camera->RelativeLocation;
	MarkerPosition = Camera->GetComponentToWorld().GetLocation();
	//MarkerPosition = FVector(0.0f, 0.0f, 0.0f);
}

void APlayerCharacter::GetForwardVector(FVector &ForwardVector)
{
	ForwardVector = GetActorForwardVector();
}