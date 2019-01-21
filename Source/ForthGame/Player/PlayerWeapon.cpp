// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerWeapon.h"
#include "Runtime/Engine/Classes/Components/TextRenderComponent.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Runtime/Engine/Classes/Sound/SoundBase.h"
#include "Runtime/Engine/Classes/Components/AudioComponent.h"

#include "PlayerCharacter.h"
#include "Enemies/Enemy.h"

#include "DrawDebugHelpers.h"

// Sets default values
APlayerWeapon::APlayerWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlayerWeapon::BeginPlay()
{
	Super::BeginPlay();

	if (CMuzzlePosition == NULL)
	{
		UE_LOG(LogTemp, Error, TEXT("Weapon %s has no muzzle position"), *GetNameSafe(this))
	}
	if (ShootParticles == NULL)
	{
		UE_LOG(LogTemp, Error, TEXT("Weapon %s has no particle socket"), *GetNameSafe(this))
	}
	if (CRightHandSocket == NULL)
	{
		UE_LOG(LogTemp, Error, TEXT("Weapon %s has no right hand socket"), *GetNameSafe(this))
	}
	if (CLeftHandSocket == NULL)
	{
		UE_LOG(LogTemp, Error, TEXT("Weapon %s has no left hand socket"), *GetNameSafe(this))
	}
	if (CAmmoText == NULL)
	{
		UE_LOG(LogTemp, Error, TEXT("Weapon %s has no ammo text"), *GetNameSafe(this))
	}

	ShootingTimer = 0.0f;
	CDeltaTime = 0.0f;

	SetAmmo(MaxAmmo);

	if (ShootingAudio != NULL)
	{
		ShootingAudio->bAutoActivate = false;
		UE_LOG(LogTemp, Warning, TEXT("Shooting Audio set"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Not audio component on weapon"));
	}
}

void APlayerWeapon::SetPlayer(APlayerCharacter *PPlayer)
{
	Player = PPlayer;
}

// Called every frame
void APlayerWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CDeltaTime = DeltaTime;
	UpdateShootingTimer();
}

void APlayerWeapon::Shoot()
{
	UE_LOG(LogTemp, Warning, TEXT("Weapon fire!"));

	switch (FiringType)
	{
	case EFiringType::Normal:
		break;
	default:
		break;
	}

	FHitResult hit;
	float rayLength = Range;
	FVector startLocation;
	FVector direction;
	FVector endLocation;
	Player->GetAimMarkerPosition(startLocation);
	Player->GetForwardVector(direction);
	endLocation = startLocation + direction * rayLength;
	//DrawDebugLine(GetWorld(), startLocation, endLocation, FColor::Green, false, 2.0f, 0, 2.0f);

	FCollisionQueryParams shootTraceParams = FCollisionQueryParams(FName(TEXT("Shoot Trace")), true, Player);
	shootTraceParams.bTraceComplex = true;
	shootTraceParams.bTraceAsyncScene = true;
	shootTraceParams.bReturnPhysicalMaterial = false;

	FHitResult shootHit(ForceInit);

	GetWorld()->LineTraceSingleByChannel(shootHit, startLocation, endLocation, ECC_Pawn, shootTraceParams);
	
	if (shootHit.bBlockingHit)
	{
		AActor *actorHit = shootHit.GetActor();
		//UE_LOG(LogTemp, Warning, TEXT("Target hit!"));
		//UE_LOG(LogTemp, Warning, TEXT("Name: %s, Class: %s"), *actorHit->GetName(), *actorHit->GetClass()->GetName());
		if (actorHit->IsA(AEnemy::StaticClass()))
		{
			//UE_LOG(LogTemp, Warning, TEXT("Enemy hit!"));
			AEnemy *enemy = (AEnemy *)actorHit;
			enemy->ReceiveShoot(Power);
		}
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("No target hit"));
	}

	ShootingTimer = ShootingRate;
	SetAmmo(Ammo - AmmoPerShot);
	if (ShootingAudio != NULL)
	{
		UE_LOG(LogTemp, Warning, TEXT("Shooting Audio play"));
		ShootingAudio->Play();
	}
	if (ShootParticles != NULL)
	{
		ShootParticles->Activate(true);
	}
}

void APlayerWeapon::SpecialShoot()
{
	// Extra
}

void APlayerWeapon::NotEnoughAmmo()
{
	UE_LOG(LogTemp, Warning, TEXT("Not enough ammo!"));
}

bool APlayerWeapon::CheckEnoughAmmo()
{
	bool enoughAmmo;
	enoughAmmo = Ammo >= AmmoPerShot;
	return enoughAmmo;
}

void APlayerWeapon::UpdateAmmoUI()
{
	FLinearColor color;
	float ammoLeftRatio = Ammo / MaxAmmo;
	float colorRatio;

	if (ammoLeftRatio >= 0.75f)
	{
		colorRatio = (1.0f - ammoLeftRatio) / (1.0f - 0.75f);
		color = FLinearColor::LerpUsingHSV(FLinearColor::Blue, FLinearColor::Green, colorRatio);
	}
	else if (ammoLeftRatio >= 0.5f)
	{
		colorRatio = (0.75f - ammoLeftRatio) / (0.75f - 0.5f);
		color = FLinearColor::LerpUsingHSV(FLinearColor::Green, FLinearColor::Yellow, colorRatio);
	}
	else if (ammoLeftRatio > 0.0f)
	{
		colorRatio = (0.5f - ammoLeftRatio) / 0.5f;
		color = FLinearColor::LerpUsingHSV(FLinearColor::Yellow, FLinearColor::Red, colorRatio);
	}
	else
	{
		color = FLinearColor::Red;
	}

	CAmmoText->SetText(FString::Printf(TEXT("%.0f/%.0f"), Ammo, MaxAmmo));
	CAmmoText->SetTextRenderColor(color.ToFColor(true));
}

void APlayerWeapon::SetAmmo(float PAmmo)
{
	Ammo = PAmmo;
	UpdateAmmoUI();
}

void APlayerWeapon::RefillAmmo()
{
	SetAmmo(MaxAmmo);
}

bool APlayerWeapon::CheckShootRate()
{
	bool shootAvailable;
	shootAvailable = ShootingTimer == 0.0f;
	return shootAvailable;
}

void APlayerWeapon::UpdateShootingTimer()
{
	ShootingTimer = FGenericPlatformMath::Max(0.0f, ShootingTimer - CDeltaTime);
}

void APlayerWeapon::TryToShoot()
{
	bool canShoot;
	bool enoughAmmo;
	canShoot = CheckShootRate();
	enoughAmmo = CheckEnoughAmmo();
	if (canShoot && enoughAmmo)
	{
		Shoot();
		UpdateAmmoUI();
	}
	else if (canShoot)
	{
		NotEnoughAmmo();
	}
}

void APlayerWeapon::TrySpecialShoot()
{
	// Extra
}


