// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Cannon.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameStructs.h"
//#include "Engine/Engine.h"
#include "Components/ArrowComponent.h"
#include "TankPawn.generated.h"



class UStaticMeshComponent;
class USkeletalMeshComponent;
class ATankPlayerController;
class ACannon;
class UArrowComponent;

UCLASS()
class TANK_API ATankPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATankPawn();
	virtual void Tick(float DeltaTime) override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void MoveFunction(float* DeltaTime);
	void RotateRight(float Value);
	// Called to bind functionality to input
//	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	//UFUNCTION()
		void Fire();
	void FireSpecial();

	void ChangeCannon();

void SetupCannon(TSubclassOf<ACannon> newCannon);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		USkeletalMeshComponent* BodyMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		UStaticMeshComponent* TurretMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		class UBoxComponent* BoxCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float MoveSpeed = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float RotationSpeed = 100.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
		float InterpolationKey = 0.1f;

	//Для поворота башни
	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
	//	float InterpolationKey = 0.1f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret|Speed")
		float TurretRotationInterpolationKey = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cannon")
		UArrowComponent* CannonSetupPoint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret|Cannon")
		TSubclassOf<ACannon>CannonClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret|Cannon")
		TSubclassOf<ACannon>SecondCannonClass;
	UPROPERTY()
		ACannon* Cannon;



	float targetForwardAxisValue = 0.0f;
	float targetRigthAxisValue = 0.0f;
	float targetRotateRigthAxisValue = 0.0f;
	float CurrentRightAxisValue;

	UPROPERTY()
		ATankPlayerController* TankController;
public:	
	// Called every frame
//	virtual void Tick(float DeltaTime) override;



};
