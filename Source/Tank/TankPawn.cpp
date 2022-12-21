/*
* Унесли в базовый класс:
HealthComponent
void ATankPawn::TakeDamage(FDamageData DamageData)
void ATankPawn::Die()
void ATankPawn::DamageTaked(float DamageValue)


*/


#include "TankPawn.h"

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

//DECLARE_LOG_CATEGORY_EXTERN(TankLog, All, All);
//DEFINE_LOG_CATEGORY(TankLog);

// Sets default values
ATankPawn::ATankPawn()
{
	CannonPul[0] = CannonClass;
	CannonPul[1] = SecondCannonClass;
	CannonPul[2] = TherdCannonClass;

 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("RootComponent"));
	RootComponent = BoxCollision;
									  
	BodyMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BodyMesh"));
	BodyMesh->SetupAttachment(BoxCollision);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	TurretMesh->SetupAttachment(BodyMesh);
											  
	CannonSetupPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("CannonSetupPoint"));
//	CannonSetupPoint->AttachToComponent(TurretMesh);
	CannonSetupPoint->AttachToComponent(TurretMesh, FAttachmentTransformRules::KeepRelativeTransform);


	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(BoxCollision);
	SpringArm->bDoCollisionTest = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;
	SpringArm->bInheritYaw = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	/************Реализуем интерфейс здоровья урок 5 стр 17************************/
	HitCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit collider"));
	HitCollider->SetupAttachment(RootComponent);//BodyMesh
	/**********************************************************************/

}


// Called when the game starts or when spawned
void ATankPawn::BeginPlay()
{
	Super::BeginPlay();
	//Для башни
	TankController = Cast<ATankPlayerController>(GetController());
	SetupCannon(CannonClass);
	
}

// Called every frame
void ATankPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("Hello")));
	MoveFunction(&DeltaTime);

	//Вращение танка
	CurrentRightAxisValue = FMath::Lerp(CurrentRightAxisValue, targetRotateRigthAxisValue, InterpolationKey);
	//UE_LOG(LogTemp, Warning, TEXT("CurrentRightAxisValue = %f targetRotateRigthAxisValue = % f"), CurrentRightAxisValue, targetRotateRigthAxisValue);
	float yawRotation = RotationSpeed * targetRotateRigthAxisValue * DeltaTime;
	FRotator currentRotation = GetActorRotation();
	yawRotation = currentRotation.Yaw + yawRotation;	
	FRotator newRotation = FRotator(0, yawRotation, 0);
	SetActorRotation(newRotation);

	//Для башни
	if (TankController)
	{
		FVector mousePos = TankController->GetMousePos();
		RotateTurretTo(mousePos);
		//FRotator targetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), mousePos);
		//FRotator currRotation = TurretMesh->GetComponentRotation();
		//targetRotation.Pitch = currRotation.Pitch;
		//targetRotation.Roll = currRotation.Roll;
		//TurretMesh->SetWorldRotation(FMath::Lerp(currRotation, targetRotation, TurretRotationInterpolationKey));
	}
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
//void ATankPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
//{
//	Super::SetupPlayerInputComponent(PlayerInputComponent);
//
//}

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

void ATankPawn::SetupCannon(TSubclassOf<ACannon> newCannon)
{
	UE_LOG(LogTemp, Warning, TEXT("SetupCannon() "));
	if (Cannon)
	{
		Cannon->Destroy();
	} 
	CannonClass = newCannon;
	FActorSpawnParameters params;
	params.Instigator = this;
	params.Owner = this;
	Cannon = GetWorld()->SpawnActor<ACannon>(CannonClass, params);
	Cannon->AttachToComponent(CannonSetupPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	
}

void ATankPawn::Fire()
{
	if (Cannon)
	{
		Cannon->Fire();
	}
}

void ATankPawn::FireSpecial()
{
	if (Cannon)
		Cannon->FireSpecial();
	
}

/*
Переставляем местами орудия
*/
void ATankPawn::ChangeCannon()
{
	TSubclassOf<ACannon> CachedCannon;
	CachedCannon = CannonClass;
	CannonClass = SecondCannonClass;
	SecondCannonClass = TherdCannonClass;
	TherdCannonClass = CachedCannon;
	SetupCannon(CannonClass);
	if(canonCnt++ >= 3)
		canonCnt = 0;
}

/**/
FVector ATankPawn::GetTurretForwardVector()
{
	return TurretMesh->GetForwardVector();
}


/**/
void ATankPawn::RotateTurretTo(FVector TargetPosition)
{
	FRotator targetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetPosition);
	FRotator currRotation = TurretMesh->GetComponentRotation();
	targetRotation.Pitch = currRotation.Pitch;
	targetRotation.Roll = currRotation.Roll;
	TurretMesh->SetWorldRotation(FMath::Lerp(currRotation, targetRotation, TurretRotationInterpolationKey));
}

/**/
FVector ATankPawn::GetEyesPosition()
{
	return CannonSetupPoint->GetComponentLocation();
}