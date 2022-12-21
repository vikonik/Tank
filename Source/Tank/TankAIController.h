// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TankPawn.h"
#include "TankAIController.generated.h"

/**
 * 
 */

class ATankPawn;

UCLASS()
class TANK_API ATankAIController : public AAIController
{
	GENERATED_BODY()
protected:
	UPROPERTY()
		ATankPawn* TankPawn;
	UPROPERTY()
		APawn* PlayerPawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Move params|Patrol points" , Meta = (MakeEditWidget = true)) 
		TArray<FVector> PatrollingPoints;

	int32 CurrentPatrolPointIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Move params|Accurency")
		float MovementAccurency;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};

