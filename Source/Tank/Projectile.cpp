// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"
#include "DamageTaker.h"
#include <Components\SceneComponent.h>
#include "Components/SphereComponent.h"
//#include "DamageTaker.h"
#include "GameStructs.h"
#include "Particles\ParticleSystem.h"
#include "Kismet\GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Particles\ParticleSystemComponent.h"
// Sets default values
AProjectile::AProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = SceneComp;
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetupAttachment(RootComponent);
	ProjectileMesh->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnMeshOverlapBegin);
	ProjectileMesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);


}

//void AProjectile::BeginPlay() {
//	Super::BeginPlay();
//}

void AProjectile::Start()
{
	GetWorld()->GetTimerManager().SetTimer(MoveTimer, this, &AProjectile::Move, MoveRate, true, 0.0f);

}

void AProjectile::Move()
{
	FVector nextPosition = GetActorLocation() + GetActorForwardVector() * MoveSpeed * MoveRate;
	SetActorLocation(nextPosition);
}

void AProjectile::OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	//	//Определяем чей снаряд
	//	AActor* owner = GetOwner();												    //Владелец снаряда это пушка
	//	AActor* ownerByOwner = (owner != nullptr) ? owner->GetOwner() : nullptr;	//Здесь определяем кто владелец владельца снаряда(кто владелец пушки, турель или танк)
	//																				//Какой-то мутный тернарник...

	//if (OtherActor != owner || OtherActor != ownerByOwner) { //Проверяем, что не попали сами в себя
	//	IDamageTaker* damageTakerActor = Cast<IDamageTaker>(OtherActor);//Пытаемся понять есть ли у актора с которым пересеклись интерфейс на получение урона
	//	if (damageTakerActor)
	//	{
	//		UE_LOG(LogTemp, Warning, TEXT("Projectile %s collided with %s. can has damage"), *GetName(), *OtherActor->GetName());
	//		FDamageData damageData;
	//		damageData.DamageValue = Damage;
	//		damageData.Instigator = owner;//владелец снаряда(пушка)
	//		damageData.DamageMaker = this;
	//		damageTakerActor->TakeDamage(damageData);
	//	} 
	//	else
	//	{		
	//		UE_LOG(LogTemp, Warning, TEXT("Projectile %s collided with %s. can't has damage, fuul destroy"), *GetName(), *OtherActor->GetName());
	//	//	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, "Projectile collided");
	//		OtherActor->Destroy();//Это разрушаем объект
	//	}

	//	//OtherActor->Destroy();//Удаляет объект в который врезался
	//	
	//}

	AActor* owner = GetOwner();
	AActor* ownerByOwner = owner != nullptr ? owner->GetOwner() : nullptr;
	if (OtherActor != owner && OtherActor != ownerByOwner)
	{
		IDamageTaker* damageTakerActor = Cast<IDamageTaker>(OtherActor);
		if (damageTakerActor)
		{
			FDamageData damageData;
			damageData.DamageValue = Damage;
			damageData.Instigator = owner;
			damageData.DamageMaker = this;
			damageTakerActor->TakeDamage(damageData);
		} 
		else
		{
			UPrimitiveComponent * mesh = Cast<UPrimitiveComponent>(OtherActor->GetRootComponent());
			if (mesh)
			{
				if (mesh->IsSimulatingPhysics())
				{
					FVector forceVector = OtherActor->GetActorLocation() - GetActorLocation();
					forceVector.Normalize();
					mesh->AddImpulse(forceVector * PushForce, NAME_None, true);
				}
			}
		} 
		Destroy();
	}

//Destroy();//Это разрушаем снаряд


}



