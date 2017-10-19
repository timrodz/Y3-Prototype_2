// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractableObject.h"
#include "EnemyAIController.h"

// Sets default values
AInteractableObject::AInteractableObject()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AInteractableObject::BeginPlay()
{
	Super::BeginPlay();
	
	this->OnActorHit.AddDynamic(this, &AInteractableObject::OnHit);
	this->OnCollisionHit.AddDynamic(this, &AInteractableObject::OnCreateNoise);
}

// Called every frame
void AInteractableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AInteractableObject::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AInteractableObject::OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& hit)
{
	//UE_LOG(LogTemp, Log, TEXT("%s :: OnHit"), *SelfActor->GetName());

	OnCollisionHit.Broadcast(OtherActor->GetActorLocation());
}

void AInteractableObject::OnCreateNoise(FVector location)
{
	UE_LOG(LogTemp, Log, TEXT("Interactable (%s)::OnCreateNoise: %s"), *this->GetName(), *location.ToString());
}