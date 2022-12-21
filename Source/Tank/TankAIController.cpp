// Fill out your copyright notice in the Description page of Project Settings.


#include "TankAIController.h"

#include "TankPawn.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
//
void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
	TankPawn = Cast<ATankPawn>(GetPawn());
	FVector pawnLocation = TankPawn->GetActorLocation();
	MovementAccurency = TankPawn->GetMovementAccurency();
	TArray<FVector> points = TankPawn->GetPatrollingPoints();
	for (FVector point : points)
	{
		PatrollingPoints.Add(point + pawnLocation);
	} 
	
	CurrentPatrolPointIndex = 0;
}
//
void ATankAIController::Tick(float DeltaTime)
{
		Super::Tick(DeltaTime);
		TankPawn->MoveForward(1);
		FVector currentPoint = PatrollingPoints[CurrentPatrolPointIndex];
		FVector pawnLocation = TankPawn->GetActorLocation();
		if (FVector::Distance(currentPoint, pawnLocation) <= MovementAccurency)
		{
			CurrentPatrolPointIndex++;
			if (CurrentPatrolPointIndex >= PatrollingPoints.Num())
				CurrentPatrolPointIndex = 0;
		} 
		
		FVector moveDirection = currentPoint - pawnLocation;
		moveDirection.Normalize();
		FVector forwardDirection = TankPawn->GetActorForwardVector();
		FVector rightDirection = TankPawn->GetActorRightVector();
		DrawDebugLine(GetWorld(), pawnLocation, currentPoint, FColor::Green, false, 0.1f, 0, 5);
	
		float forwardAngle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(forwardDirection, moveDirection)));
		float rightAngle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(rightDirection, moveDirection)));
		float rotationValue = 0;
		if (forwardAngle > 5)
			rotationValue = 1;
		if (rightAngle > 90)
			rotationValue = -rotationValue;
		//UE_LOG(LogTemp, Warning, TEXT("AI Rotation forwardAngle: %f rightAngle: %frotationValue:% f"), forwardAngle, rightAngle, rotationValue);
	TankPawn->RotateRight(rotationValue);
}