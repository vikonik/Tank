// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameStructs.h"//Это здесь надо?
#include "DamageTaker.h"
#include "BasePawn.generated.h"


class UStaticMeshComponent;
class UArrowComponent;
class ACannon;
class UHealthComponent;
class UBoxComponent;
class UParticleSystem;
class USoundBase;
class AAmmoBox;

UCLASS()
class TANK_API ABasePawn : public APawn, public IDamageTaker
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABasePawn();
	virtual void TakeDamage(FDamageData DamageData) override;
protected:
	// Called when the game starts or when spawned
	//virtual void BeginPlay() override;
	
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	//	USkeletalMeshComponent* BodyMesh;

	//UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	//	UStaticMeshComponent* BodyMesh;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	//	UStaticMeshComponent* TurretMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		UHealthComponent* HealthComponent;

	UFUNCTION()
		void Die();
	UFUNCTION()
		void DamageTaked(float DamageValue);
public:	
	//// Called every frame
	//virtual void Tick(float DeltaTime) override;

	//// Called to bind functionality to input
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
