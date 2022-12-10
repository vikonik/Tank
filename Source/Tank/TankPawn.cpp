// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPawn.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
// Sets default values
ATankPawn::ATankPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("RootComponent"));
	RootComponent = BoxCollision;
									  
	BodyMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BodyMesh"));
	BodyMesh->SetupAttachment(BoxCollision);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	TurretMesh->SetupAttachment(BodyMesh);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(BoxCollision);
	SpringArm->bDoCollisionTest = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;
	SpringArm->bInheritYaw = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

}

// Called when the game starts or when spawned
void ATankPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATankPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("Hello")));
	MoveFunction(&DeltaTime);

	float yawRotation = RotationSpeed * targetRotateRigthAxisValue * DeltaTime;
FRotator currentRotation = GetActorRotation();
yawRotation = currentRotation.Yaw + yawRotation;
FRotator newRotation = FRotator(0, yawRotation, 0);
SetActorRotation(newRotation);
}


/*
Выносим в отдельную функцию
*/
void ATankPawn::MoveFunction(float *DeltaTime) {
	FVector CurrentPosition = GetActorLocation();
	FVector ForwardVector = GetActorForwardVector();
	FVector RightVector = GetActorRightVector();
	FVector movePosition = CurrentPosition + ForwardVector * MoveSpeed * targetForwardAxisValue * *DeltaTime;
	//	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("movePosition: %s, CurrentPosition: %s, ForwardVector %s, MoveSpeed %f, targetForwardAxisValue %f ")
	//		, *movePosition.ToString(), *ForwardVector.ToString(), *CurrentPosition.ToString(), MoveSpeed, targetForwardAxisValue));
	SetActorLocation(movePosition, true);
	CurrentPosition = GetActorLocation();
	movePosition = CurrentPosition + RightVector * RotationSpeed * targetRigthAxisValue * *DeltaTime;
	SetActorLocation(movePosition, true);

}

// Called to bind functionality to input
void ATankPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATankPawn::MoveForward(float Value) {
	targetForwardAxisValue = Value;
//	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("MoveForward.. %s"), *GetActorLocation().ToString()));
//	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("MoveForward.. %f"), Value));
}

void ATankPawn::MoveRight(float Value) {
	targetRigthAxisValue = Value;
}

void ATankPawn::RotateRight(float Value)
{
	targetRotateRigthAxisValue = Value;
}
