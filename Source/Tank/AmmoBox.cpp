#include "AmmoBox.h"
#include "TankPawn.h"
#include "Cannon.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

AAmmoBox::AAmmoBox()
{
	PrimaryActorTick.bCanEverTick = false;
	USceneComponent* sceeneCpm =
		CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = sceeneCpm;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->OnComponentBeginOverlap.AddDynamic(this, &AAmmoBox::OnMeshOverlapBegin);
	Mesh->SetCollisionProfileName(FName("OverlapAll"));
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Mesh->SetGenerateOverlapEvents(true);
} 

/*  */
void AAmmoBox::OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor*
	OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const
	FHitResult& SweepResult)
{
	ATankPawn* playerPawn = Cast<ATankPawn>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (OtherActor == playerPawn)
	{
		playerPawn->SetupCannon(CannonClass);
		ACannon* Cannon = playerPawn->GetCannon();
		if (Cannon) {
			Cannon->AddBullet(Bullet);
			UE_LOG(LogTemp, Warning, TEXT("AddBullet %d "), Bullet);
		}
			

		Destroy();//????? ????
	}
}