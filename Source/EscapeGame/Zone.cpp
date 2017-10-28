// Fill out your copyright notice in the Description page of Project Settings.

#include "Zone.h"


// Sets default values
AZone::AZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));

}

// Called when the game starts or when spawned
void AZone::BeginPlay()
{
	Super::BeginPlay();

	// Get arrays of items

	OnActorBeginOverlap.AddDynamic(this, &AZone::BeginActorOverlap);
	OnActorEndOverlap.AddDynamic(this, &AZone::EndActorOverlap);

}

// Called every frame
void AZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AZone::UpdateZoneItems()
{
	
}

bool AZone::IsEnemyInZone()
{
	return EnemyInZone;
}

bool AZone::GetHasItemToCheck()
{
	return HasItemToCheck;
}

void AZone::SetHasItemToCheck(bool _b)
{
	HasItemToCheck = _b;
}


void AZone::BeginActorOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	EnemyRef = Cast<AEnemyCharacter>(OtherActor);
	
	if (EnemyRef)
	{
		EnemyInZone = true;
	}
}

void AZone::EndActorOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (Cast<AEnemyCharacter>(OtherActor))
	{
		EnemyInZone = false;
	}

}

