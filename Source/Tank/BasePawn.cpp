// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePawn.h"
#include "Components/SkeletalMeshComponent.h"



#include <Components/StaticMeshComponent.h>
#include <Components/ArrowComponent.h>
#include <Components/BoxComponent.h>
#include "HealthComponent.h"

#include "Cannon.h"
#include <Kismet/KismetMathLibrary.h>
#include <Kismet/GameplayStatics.h>
#include "AmmoBox.h"


#include "Components/BoxComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "TankPlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Cannon.h"
#include "HealthComponent.h"
#include "DamageTaker.h"
#include "Components/ArrowComponent.h"

// Sets default values


ABasePawn::ABasePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	//TurretMesh->SetupAttachment(BodyMesh);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	HealthComponent->OnDamaged.AddUObject(this, &ABasePawn::DamageTaked);
	HealthComponent->OnDie.AddUObject(this, &ABasePawn::Die);
}


/*
танк подбит
*/
void ABasePawn::Die()
{
	Destroy();
} 

/*
попали в танк, минусуем здоровье
*/
void ABasePawn::DamageTaked(float DamageValue)
{
	UE_LOG(LogTemp, Warning, TEXT("Tank %s taked damage:%f Health:%f"), *GetName(),	DamageValue, HealthComponent->GetHealth());
}

void ABasePawn::TakeDamage(FDamageData DamageData)
{
	HealthComponent->TakeDamage(DamageData);
}


// Called when the game starts or when spawned
//void ABasePawn::BeginPlay()
//{
//	Super::BeginPlay();
//	
//}
//
//// Called every frame
//void ABasePawn::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}
//
//// Called to bind functionality to input
//void ABasePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
//{
//	Super::SetupPlayerInputComponent(PlayerInputComponent);
//
//}

