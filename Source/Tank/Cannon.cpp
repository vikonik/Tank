// Fill out your copyright notice in the Description page of Project Settings.


#include "Cannon.h"
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"
#include "Projectile.h"
#include "Engine/Engine.h"
#include "DrawDebugHelpers.h"

// Sets default values
ACannon::ACannon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	USceneComponent* sceeneCpm = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = sceeneCpm;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cannon mesh"));
	Mesh->SetupAttachment(RootComponent);
	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Spawn point"));
	ProjectileSpawnPoint->SetupAttachment(Mesh);
}

/*
Собственно сам выстрел
*/
void ACannon::BulletShot() {
	AProjectile* projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass,
		ProjectileSpawnPoint->GetComponentLocation(),
		ProjectileSpawnPoint->GetComponentRotation());

	if (projectile) {
		projectile->Start();
		//GEngine->AddOnScreenDebugMessage(2, 1, FColor::Green, "projectile spaun");
	}
}

void ACannon::FireTrace()
{
	FHitResult hitResult;
	FCollisionQueryParams traceParams;
	traceParams.bTraceComplex = true;
	traceParams.bReturnPhysicalMaterial = false;

	FVector Start = ProjectileSpawnPoint->GetComponentLocation();
	FVector End = Start + ProjectileSpawnPoint->GetForwardVector() * FireRange;

	if (GetWorld()->LineTraceSingleByChannel(hitResult, Start, End, ECollisionChannel::ECC_GameTraceChannel1, traceParams))
	{
		DrawDebugLine(GetWorld(), Start, hitResult.Location, FColor::Purple, false, 1.0f, 0, 5.0f);
		if (hitResult.GetActor())
		{
			//UE_LOG(LogTemp, Warning, TEXT("trace overlap : %s"), *hitResult.GetActor()->GetName());
			hitResult.GetActor()->Destroy();
		}
	}
	else
	{
		DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.0f, 0, 5.0f);
	}
}
/**/
void ACannon::Fire()
{
	if (!IsReadyToFire() )
	{
		return;
	} 
	ReadyToFire = false;
	Bullet--;

	if (Type == ECannonType::FireProjectile)
	{
		GEngine->AddOnScreenDebugMessage(2, 1, FColor::Green, "Fire - projectile");
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Bullet is: %d"), Bullet));
		BulletShot();
	} 
	else
	{
		FireTrace();
		GEngine->AddOnScreenDebugMessage(3, 1,FColor::Green, "Fire - trace");
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Bullet is: %d"), Bullet));
	}

	GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &ACannon::Reload, 1 / FireRate, false);//FireRate - Время перезарядки
} 


/*
Будем стрелять очередью
*/
void ACannon::FireSpecial()
{
	if (!IsReadyToFire())
	{
		return;
	}

	ReadyToFire = false;
	GetWorld()->GetTimerManager().SetTimer(BurstTimer, this, &ACannon::BurstFire, BurstInterval, true, 0.0f);
}

bool ACannon::IsReadyToFire()
{
	if (Bullet <= 0) {
		if (!rechargeBegin) {//Если при нажатии не проверять на повторный выстркл, таймер перезапускается зпнова
			GEngine->AddOnScreenDebugMessage(18, 1, FColor::Green, "Recharge? plese wait...");
			GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &ACannon::Recharge, 5, false);
		}
		else {
			GEngine->AddOnScreenDebugMessage(18, 1, FColor::Green, "not ready yet...");
		}
		rechargeBegin = true;

		return false;
	}

	return ReadyToFire;
}

void ACannon::Reload()
{
	ReadyToFire = true;
}

void ACannon::BeginPlay()
{
	Super::BeginPlay();
	Reload();
}
// Called every frame
void ACannon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ACannon::BurstFire()
{
	if (CurrentBurts == BurstSize)
	{
		Bullet--;
		GetWorld()->GetTimerManager().ClearTimer(BurstTimer);
		ReadyToFire = true;
		CurrentBurts = 0;
		GEngine->AddOnScreenDebugMessage(5, 2, FColor::Green, "BurstFire");
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Bullet is: %d"), Bullet));
		return;
	}
	BulletShot();
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, "Pif-paff");//Столько раз стрельнули
	CurrentBurts++;
}

void ACannon::Recharge() {
	rechargeBegin = false;
	Bullet = 10;
	Reload();
	GEngine->AddOnScreenDebugMessage(10, 1, FColor::Green, "Recharge ready");
}

/**/
void ACannon::AddBulet() {
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, "AddBulet");
}