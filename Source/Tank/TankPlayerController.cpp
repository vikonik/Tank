// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPlayerController.h"

#include "TankPawn.h"

ATankPlayerController::ATankPlayerController() {

}

void ATankPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("MoveForward", this, &ATankPlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ATankPlayerController::MoveRight);
}

void ATankPlayerController::BeginPlay() {
	Super::BeginPlay();
	TankPawn = Cast<ATankPawn>(GetPawn());
}


void ATankPlayerController::MoveForward(float Value) {
	if (TankPawn) {
		TankPawn->MoveForward(Value);
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("ATankPlayerController MoveForward.. %f"), Value));
	}
}

void ATankPlayerController::MoveRight(float Value) {
	if (TankPawn) {
		TankPawn->MoveRight(Value);
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("ATankPlayerController MoveRight.. %f"), Value));
	}
}