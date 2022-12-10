// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameStructs.h"
#include "GameFramework/Actor.h"

#include "Engine/Engine.h"
#include "Cannon.generated.h"

class UArrowComponent;
UCLASS()
class TANK_API ACannon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACannon();
	void Fire();
	bool IsReadyToFire();
	void Reload();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		class UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		class UArrowComponent* ProjectileSpawnPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire params")
		float FireRate = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire params")
		float FireRange = 1000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire params")
		float FireDamage = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire params")
		ECannonType Type = ECannonType::FireProjectile;

	FTimerHandle ReloadTimerHandle;
	bool ReadyToFire = true;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
