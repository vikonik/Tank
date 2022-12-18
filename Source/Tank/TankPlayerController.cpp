// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPlayerController.h"
#include "DrawDebugHelpers.h"
#include "TankPawn.h"

ATankPlayerController::ATankPlayerController() {
	bShowMouseCursor = true;
}

void ATankPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("MoveForward", this, &ATankPlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ATankPlayerController::MoveRight);
	InputComponent->BindAxis("RotateRight", this, &ATankPlayerController::RotateRight);
	InputComponent->BindAction("Fire", IE_Pressed, this, &ATankPlayerController::Fire);
	InputComponent->BindAction("FireSpecial", EInputEvent::IE_Pressed, this, &ATankPlayerController::FireSpecial);
	InputComponent->BindAction("ChangeCannon", EInputEvent::IE_Pressed, this, &ATankPlayerController::ChangeCannon);

}

void ATankPlayerController::BeginPlay() {
	Super::BeginPlay();
	TankPawn = Cast<ATankPawn>(GetPawn());
}


void ATankPlayerController::MoveForward(float Value) {
	if (TankPawn) {
		TankPawn->MoveForward(Value);
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("ATankPlayerController MoveForward.. %f"), Value));
	}
}

void ATankPlayerController::MoveRight(float Value) {
	if (TankPawn) {
		TankPawn->MoveRight(Value);
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("ATankPlayerController MoveRight.. %f"), Value));
	}
}

void ATankPlayerController::RotateRight(float AxisValue)
{
	TankPawn->RotateRight(AxisValue);
}

void ATankPlayerController::Tick(float DeltaTime)
{
	FVector mouseDirection;
	DeprojectMousePositionToWorld(MousePos, mouseDirection);
	FVector pawnPos = TankPawn->GetActorLocation();
	MousePos.Z = pawnPos.Z;
	FVector dir = MousePos - pawnPos;
	dir.Normalize();
	MousePos = pawnPos + dir * 1000;
	DrawDebugLine(GetWorld(), pawnPos, MousePos, FColor::Green, false, 0.1f, 0, 5);
}


void ATankPlayerController::Fire()
{
	if (TankPawn)
		TankPawn->Fire();
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Fire")));
}

void ATankPlayerController::FireSpecial()
{
	if (TankPawn)
		TankPawn->FireSpecial();

}

/*

*/
void ATankPlayerController::ChangeCannon() {
	if (TankPawn)
		TankPawn->ChangeCannon();
}