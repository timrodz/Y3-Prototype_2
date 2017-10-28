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

	OnActorBeginOverlap.AddDynamic(this, &AZone::BeginActorOverlap);
	OnActorEndOverlap.AddDynamic(this, &AZone::EndActorOverlap);

	// Make arrays

	// Create the item struct array, one struct for each BP to track
	for (auto It = BPsToTrack.CreateConstIterator(); It; ++It)
	{
		FItemStruct* newStruct = new FItemStruct();
		ItemStructArray.Add(newStruct);
	}
	
	// For each struct in the array, set the BP type as the BP in the "BPs to track" array
	for (int32 Index = 0; Index != ItemStructArray.Num(); ++Index)
	{
		ItemStructArray[Index]->BPType = BPsToTrack[Index];
	}
/*
	// Get all overlapping actors loop

		//for (int32 Index = 0; Index != ItemStructArray.Num(); ++Index)
		{
			if (Cast<ItemStructArray[Index].BPType>(overlapping actor))
			{
				ItemStructArray[Index].Items.Add(overlapping actor)
			}

		}

	
	*/

/*
	struct FItemStruct
	{
		GENERATED_USTRUCT_BODY()

			AActor* BPType;

		UPROPERTY()
			TArray<AActor*> Items;

		FItemStruct()
		{
		}
	};




		UPROPERTY(EditAnywhere, Category = "Items To Track")
			TArray<AActor*> BPsToTrack;

		TArray<FItemStruct*> ItemStructArray;
*/



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

