// Fill out your copyright notice in the Description page of Project Settings.

#include "Zone.h"
#include "TrackedObject.h"
#include "EnemyCharacter.h"
#include "FirstPersonCharacterController.h"


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
	// Add the BP to track as the BP type
	for (auto It = BPsToTrack.CreateConstIterator(); It; ++It)
	{
		FItemStruct* newStruct = new FItemStruct();
		ItemStructArray.Add(newStruct);
		newStruct->BPType = *It;
	}
	
	// Get all overlapping actors
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors);

	// For each overlapping actor
	// Check if the class of the overlapping actor equals the class of the particular structure in the structure array
	for (auto It = OverlappingActors.CreateConstIterator(); It; ++It)
	{
		for (int32 Index = 0; Index != ItemStructArray.Num(); ++Index)
		{
			// If so, add the overlapping item to the items array of the struct, and break out of inner loop
			if (ItemStructArray[Index]->BPType == (*It)->GetClass())
			{
				ATrackedObject* object = Cast<ATrackedObject>(*It);

				// Check if cast worked
				if (object)
				{
					// Add the tracked object to the item list and set its zone to this zone
					ItemStructArray[Index]->Items.Add(object);
					object->SetCurrentZone(this);
					break;
				}
			}
		}
	}

	// Log info
	UE_LOG(LogTemp, Warning, TEXT("%s has %d different BPs to track"), *this->GetName(), ItemStructArray.Num());
	
	for (auto It = ItemStructArray.CreateConstIterator(); It; ++It)
	{
		UE_LOG(LogTemp, Warning, TEXT("     %s array has %d tracked items"), *(*It)->BPType->GetName(), (*It)->Items.Num());
	}

	UpdateZoneItems();
}

// Called every frame
void AZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AZone::UpdateZoneItems()
{
	int HighestAlert = 0;

	// Iterate over item struct array
	for (auto It = ItemStructArray.CreateConstIterator(); It; ++It)
	{
		// If array is not empty
		if ((*It)->Items.Num() > 0)
		{
			// Iterate over item array in that struct
			for (auto It2 = (*It)->Items.CreateConstIterator(); It2; ++It2)
			{
				// If the item is valid and is active
				if ((*It2) && (*It2)->GetActiveState())
				{
					// Check its alert level
					if ((*It2)->GetAlertLevel() >= HighestAlert)
					{
						HasItemToCheck = true;
						ItemLocation = (*It2)->GetActorLocation();
						HighestAlert = (*It2)->GetAlertLevel();
						CurrentTarget = (*It2);
					}

					// If enemy is currently in the zone, recheck for events
					if (EnemyInZone && EnemyRef)
					{
						EnemyRef->CheckForActiveZoneEvents();
						UE_LOG(LogTemp, Warning, TEXT("Update and enemy in zone"));
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("Update and enemy NOT in zone"));
					}
				}
			}
		}
	}
}
bool AZone::IsEnemyInZone()
{
	return EnemyInZone;
}

bool AZone::IsPlayerInZone()
{
	return PlayerInZone;
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
		return;
	}

	PlayerRef = Cast<AFirstPersonCharacterController>(OtherActor);

	if (PlayerRef)
	{
		PlayerInZone = true;
	}
}

void AZone::EndActorOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (Cast<AEnemyCharacter>(OtherActor))
	{
		EnemyInZone = false;
		return;
	}

	PlayerRef = Cast<AFirstPersonCharacterController>(OtherActor);

	if (PlayerRef)
	{
		PlayerInZone = false;
	}

}

FVector AZone::GetItemLocation()
{
	return ItemLocation;
}

void AZone::SetItemLocation(FVector location)
{
	ItemLocation = location;
}

void AZone::SetCurrentTargetToInactive()
{
	if (CurrentTarget)
	{
		CurrentTarget->SetActiveState(false);
	}	
}

ATrackedObject * AZone::GetCurrentTarget()
{
	return CurrentTarget;
}

