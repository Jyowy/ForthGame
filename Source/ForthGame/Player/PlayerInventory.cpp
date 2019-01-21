// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerInventory.h"

UPlayerInventory::UPlayerInventory()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UPlayerInventory::BeginPlay()
{
	Super::BeginPlay();

	if (LightWeaponSlot == NULL)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Light Weapon class"));
	}
	if (MainWeaponSlot == NULL)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Main Weapon class"));
	}
	if (MeleeWeaponSlot == NULL)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Melee Weapon class"));
	}
}

void UPlayerInventory::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

TSubclassOf<APlayerWeapon> UPlayerInventory::GetWeapon(EWeaponType WeaponSlot)
{
	TSubclassOf<APlayerWeapon> weapon = NULL;

	switch (WeaponSlot)
	{
	case EWeaponType::LightWeapon:
		weapon = LightWeaponSlot;
		break;
	case EWeaponType::MainWeapon:
		weapon = MainWeaponSlot;
		break;
	case EWeaponType::MeleeWeapon:
		weapon = MeleeWeaponSlot;
		break;
	}

	if (weapon == NULL)
	{
		UE_LOG(LogTemp, Warning, TEXT("Inventory has no %d weapon slot"), (int) WeaponSlot);
	}

	return weapon;
}