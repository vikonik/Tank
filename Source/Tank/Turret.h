// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamageTaker.h"

//#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Cannon.h"

#include "BasePawn.h"

#include "Turret.generated.h"

class UStaticMeshComponent;
class UArrowComponent;
class UBoxComponent;
class UHealthComponent;
class ACannon;

UCLASS()
class TANK_API ATurret : public ABasePawn /*AActor, public IDamageTaker*/
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATurret();

	//UFUNCTION()
	//	void TakeDamage(FDamageData DamageData) ;

	//UFUNCTION()
	//	void Die();
	//UFUNCTION()
	//	void DamageTaked(float DamageValue);

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UStaticMeshComponent* BodyMesh;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UStaticMeshComponent* TurretMesh;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UArrowComponent* CannonSetupPoint;//������ ��������� ������

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
		TSubclassOf<ACannon> CannonClass;
	UPROPERTY()
		ACannon* Cannon;
	UPROPERTY()
		APawn* PlayerPawn;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
		float TargetingRange = 1000;//��������� �� ������� ����� ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
		float TargetingSpeed = 0.1f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
		float TargetingRate = 0.005f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
		float Accurency = 10;

	/************��������� ��������� ��������*****************************/
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UBoxComponent* HitCollider;//��������

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	//	class UHealthComponent* HealthComponent;
	/**********************************************************************/
	//�������� �� ����
	const FString BodyMeshPath = "StaticMesh'/Game/CSC/Meshes/SM_CSC_Tower1.SM_CSC_Tower1'";
	const FString TurretMeshPath = "StaticMesh'/Game/CSC/Meshes/SM_CSC_Gun1.SM_CSC_Gun1'";

protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;
	void Targeting();
	void RotateToPlayer();
	bool IsPlayerInRange();
	bool CanFire();
	void Fire();
	void SetupCannon(TSubclassOf<ACannon> newCamnnonClass);
};
