#include "TankFactory.h"

#include "TimerManager.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

ATankFactory::ATankFactory()
{
	PrimaryActorTick.bCanEverTick = false;
	USceneComponent* sceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = sceneComp;
	BuildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Building Mesh"));
	BuildingMesh->SetupAttachment(sceneComp);
	TankSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Cannon setup point"));
	TankSpawnPoint->AttachToComponent(sceneComp, FAttachmentTransformRules::KeepRelativeTransform);
	HitCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit collider"));
	HitCollider->SetupAttachment(sceneComp);
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health component"));
	HealthComponent->OnDie.AddUObject(this, &ATankFactory::Die);//подписываемся на акцию разрушения
	HealthComponent->OnDamaged.AddUObject(this, &ATankFactory::DamageTaked);

	UE_LOG(LogTemp, Warning, TEXT("ATankFactory is building"));
}

void ATankFactory::BeginPlay()
{
	Super::BeginPlay();
	if (LinkedMapLoader) {
		LinkedMapLoader->SetIsActivated(false);
		UE_LOG(LogTemp, Warning, TEXT("LinkedMapLoader set False"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("LinkedMapLoader is False"));
	}
		
	FTimerHandle _targetingTimerHandle;
	//GetWorld()->GetTimerManager().SetTimer(_targetingTimerHandle, this,	&ATankFactory::SpawnNewTank, SpawnTankRate, true, SpawnTankRate);
}

void ATankFactory::TakeDamage(FDamageData DamageData)
{
	//UE_LOG(LogTemp, Warning, TEXT("Take factory damage %f, health: %f"), DamageValue, HealthComponent->GetHealth());
	UE_LOG(LogTemp, Warning, TEXT("Take factory damage"));

	HealthComponent->TakeDamage(DamageData);
}


void ATankFactory::DamageTaked(float DamageValue)
{
	UE_LOG(LogTemp, Warning, TEXT("Factory %s taked damage:%f Health:%f"),
		*GetName(), DamageValue, HealthComponent->GetHealth());
}

void ATankFactory::SpawnNewTank()
{
	FTransform spawnTransform(TankSpawnPoint->GetComponentRotation(),
		TankSpawnPoint->GetComponentLocation(), FVector(1));
	ATankPawn* newTank = GetWorld()->SpawnActorDeferred<ATankPawn>(SpawnTankClass,
		spawnTransform, this, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	//
	newTank->SetPatrollingPoints(TankWayPoints);
	//
	UGameplayStatics::FinishSpawningActor(newTank, spawnTransform);
}

/**/
void ATankFactory::Die()
{
	if (LinkedMapLoader)
		LinkedMapLoader->SetIsActivated(true);
	Destroy();
}

