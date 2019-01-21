// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "PlayerWeapon.h"

#include "PlayerInventory.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	LightWeapon	UMETA(DisplayName = "Light Weapon"),
	MainWeapon	UMETA(DisplayName = "Main Weapon"),
	MeleeWeapon	UMETA(DisplayName = "Melee Weapon"),
	Num		UMETA(Hidden)
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FORTHGAME_API UPlayerInventory : public UActorComponent
{
	GENERATED_BODY()

public:	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
	TSubclassOf<APlayerWeapon> LightWeaponSlot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
		TSubclassOf<APlayerWeapon> MainWeaponSlot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Inventory)
		TSubclassOf<APlayerWeapon> MeleeWeaponSlot;

	UPlayerInventory();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	TSubclassOf<APlayerWeapon> GetWeapon(EWeaponType WeaponSlot);
		
};
