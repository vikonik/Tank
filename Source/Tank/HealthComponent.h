// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameStructs.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TANK_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()
	DECLARE_EVENT(UHealthComponent, FOnDie)
	DECLARE_EVENT_OneParam(UHealthComponent, FOnHealthChanged, float)

public:	
	// Sets default values for this component's properties
	UHealthComponent();
	FOnDie OnDie;
	FOnHealthChanged OnDamaged;
	void TakeDamage(FDamageData DamageData);
	float GetHealth() const;
	float GetHealthState() const;
	void AddHealth(float AddiditionalHealthValue);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health values")
		float MaxHealth = 10.0f;
	UPROPERTY()
		float CurrentHealth;


public:	
	// Called every frame
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
