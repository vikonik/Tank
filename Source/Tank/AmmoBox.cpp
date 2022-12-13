#include "AmmoBox.h"
#include "TankPawn.h"
#include "Cannon.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

AAmmoBox::AAmmoBox()
{
	PrimaryActorTick.bCanEverTick = false;
	USceneComponent* sceeneCpm = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = sceeneCpm;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->OnComponentBeginOverlap.AddDynamic(this, &AAmmoBox::OnMeshOverlapBegin);
	Mesh->SetCollisionProfileName(FName("OverlapAll"));

	Mesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);

	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Mesh->SetGenerateOverlapEvents(true);

	/*********************************************************/

} 

/*
План такой,
При срабатвании коллизии должно увеличиваться количествао патронов

*/
void AAmmoBox::OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor*	OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const
	FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, "AmmoBoxAddBulet");
	UE_LOG(LogTemp, Warning, TEXT("Projectile overlap : %s"), *OtherActor->GetName());
	UE_LOG(LogTemp, Warning, TEXT("Projectile overlap : %s"), *OtherComp->GetName());

	ATankPawn* playerPawn = Cast<ATankPawn>(GetWorld()->GetFirstPlayerController()->GetPawn());

	ATankPawn* TankPawn = Cast<ATankPawn>(OtherActor); //Получаем указатель на текущее орудие
	ACannon* Cannon = TankPawn->GetCannon();		   //--	
	Cannon->AddBulet();								   //Вызываем функцию перезараядки	
	OtherActor->Destroy();
	Destroy();


	if (OtherActor == playerPawn)
	{
		playerPawn->SetupCannon(CannonClass);
		
		Destroy();
	}
}