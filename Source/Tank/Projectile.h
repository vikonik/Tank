// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class TANK_API AProjectile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AProjectile();
	void Start();
	//virtual void BeginPlay() override;
	FTimerHandle MoveTimer;
	//	FTimerHandle DeactivateTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
		class UParticleSystem* Particle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
		class UParticleSystemComponent* Template;

	void Deactivate();

	bool bIsActivation = false;

protected:
	UFUNCTION()
		void Move();

	UFUNCTION()
		void OnMeshOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
			class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
		class UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float MoveSpeed = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float MoveRate = 0.0001f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		float Damage = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
		float PushForce = 1000;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	//	class USphereComponent* SphereCollision;







	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	//	float DeactivateTime = 10.0f;
};
