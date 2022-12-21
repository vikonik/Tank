/*

*/

#include "Turret.h"
#include "TankPlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Cannon.h"
#include "TimerManager.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
//#include "Components/BoxComponent.h
#include "Engine/StaticMesh.h"
#include "DamageTaker.h"
#include "GameStructs.h"
#include "HealthComponent.h"
#include "DrawDebugHelpers.h"
// Sets default values
/*
53-я минута на видосике. Почему преаод делает не так как в методичке? В чем разница?
В чем разница между SetupAttachment и AttachToComponent, в каком случае какой ужно использовать?
*/
ATurret::ATurret()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	HitCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit collider"));
	RootComponent = HitCollider;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret body"));
	BodyMesh->SetupAttachment(HitCollider);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turretturret"));
	TurretMesh->SetupAttachment(BodyMesh, "ADD_Parts_Here_Socket");//Программная точка установки башни
	 
	CannonSetupPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Cannon setup point"));
	CannonSetupPoint->SetupAttachment(TurretMesh);
	
	HitCollider->SetupAttachment(TurretMesh);

	UStaticMesh* bodyMeshTemp = LoadObject<UStaticMesh>(this, *BodyMeshPath);
	if (bodyMeshTemp)
		BodyMesh->SetStaticMesh(bodyMeshTemp);

	UStaticMesh * turretMeshTemp = LoadObject<UStaticMesh>(this, *TurretMeshPath);
	if (turretMeshTemp)
		TurretMesh->SetStaticMesh(turretMeshTemp);

	//TankPawn = Cast<ATankPawn>(GetPawn());


}


// Called when the game starts or when spawned
void ATurret::BeginPlay()
{
	Super::BeginPlay();
	
	SetupCannon(CannonClass);
	
	PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	FTimerHandle _targetingTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(_targetingTimerHandle, this, &ATurret::Targeting, TargetingRate, true, TargetingRate);
}



/**/
void ATurret::Destroyed()
{
	if (Cannon)
		Cannon->Destroy();
}

/**/
void ATurret::Targeting()
{
	//UE_LOG(LogTemp, Warning, TEXT("Tyrret Targeting "));
	if (!PlayerPawn) {

	return;
	}
	//UE_LOG(LogTemp, Warning, TEXT("Tyrret exist "));

	if (IsPlayerInRange())
	{
	//	UE_LOG(LogTemp, Warning, TEXT("Tyrret rotate "));
		RotateToPlayer();
	} 
	
	if(CanFire() && Cannon   && Cannon->IsReadyToFire())
	{
		UE_LOG(LogTemp, Warning, TEXT("Tyrret Fire "));
		Fire();
	}
} 

/**/ 
void ATurret::RotateToPlayer()
{
	FRotator targetRotation =  UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PlayerPawn->GetActorLocation());
	FRotator currRotation = TurretMesh->GetComponentRotation();
	targetRotation.Pitch = currRotation.Pitch;
	targetRotation.Roll = currRotation.Roll;
	TurretMesh->SetWorldRotation(FMath::Lerp(currRotation, targetRotation, TargetingSpeed));
} 

/**/
bool ATurret::IsPlayerInRange()
{
	return FVector::Distance(PlayerPawn->GetActorLocation(), GetActorLocation()) <= TargetingRange;
}
  
/**/
bool ATurret::CanFire()
{
	if (!IsPlayerSeen())
		return false;
	FVector targetingDir = TurretMesh->GetForwardVector();
	FVector dirToPlayer = PlayerPawn->GetActorLocation() - GetActorLocation();
	dirToPlayer.Normalize();
	float aimAngle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(targetingDir, dirToPlayer)));//Получаем направление стрельбы
//UE_LOG(LogTemp, Warning, TEXT("Tyrret CanFire aimAngle= %f, Accurency = %f"), aimAngle, Accurency);
	return aimAngle <= Accurency;
}

/**/
void ATurret::Fire()
{
	if (Cannon)
		Cannon->Fire();
}

void ATurret::SetupCannon(TSubclassOf<ACannon> newCamnnonClass)
{
	if (!newCamnnonClass) {
		return;
	}
	FActorSpawnParameters params;
	params.Owner = this;
	Cannon = GetWorld()->SpawnActor<ACannon>(CannonClass, params);
	Cannon->AttachToComponent(CannonSetupPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

FVector ATurret::GetEyesPosition()
{
	return CannonSetupPoint->GetComponentLocation();
}

//Проверяем видимость игрова турелью
bool ATurret::IsPlayerSeen()
{
	FVector playerPos = PlayerPawn->GetActorLocation();
	FVector eyesPos = this->GetEyesPosition();
	FHitResult hitResult;
	FCollisionQueryParams traceParams =	FCollisionQueryParams(FName(TEXT("FireTrace")), true, this);
	traceParams.bTraceComplex = true;
	traceParams.AddIgnoredActor(this);
	traceParams.bReturnPhysicalMaterial = false;
	if (GetWorld()->LineTraceSingleByChannel(hitResult, eyesPos, playerPos,
		ECollisionChannel::ECC_Visibility, traceParams))
	{
		if (hitResult.Actor.Get())
		{
			DrawDebugLine(GetWorld(), eyesPos, hitResult.Location, FColor::Cyan,
				false, 0.5f, 0, 10);
			return hitResult.Actor.Get() == PlayerPawn;
		}
	}

	DrawDebugLine(GetWorld(), eyesPos, playerPos, FColor::Cyan, false, 0.5f, 0, 10);
	return false;
}
