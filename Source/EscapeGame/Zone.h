// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
//#include "EnemyCharacter.h"
#include "Components/BoxComponent.h"
//#include "TrackedObject.h"
#include "Zone.generated.h"

class ATrackedObject;
class AEnemyCharacter;

USTRUCT()
struct FItemStruct
{
	GENERATED_USTRUCT_BODY()

	UClass* BPType;

	UPROPERTY()
	TArray<ATrackedObject*> Items;

	FItemStruct()
	{
	}
};


UCLASS()
class ESCAPEGAME_API AZone : public AActor
{
	GENERATED_BODY()

	bool HasItemToCheck;
	bool EnemyInZone;
	FVector ItemLocation;

	AEnemyCharacter* EnemyRef;

	ATrackedObject* CurrentTarget;

	UPROPERTY(EditAnywhere, Category = "Collision")
		UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere, Category = "Items To Track")
		TArray<UClass*> BPsToTrack;

	TArray<FItemStruct*> ItemStructArray;

public:	
	// Sets default values for this actor's properties
	AZone();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void UpdateZoneItems();

	bool IsEnemyInZone();

	bool GetHasItemToCheck();
	void SetHasItemToCheck(bool _b);

	UFUNCTION()
		void BeginActorOverlap(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
		void EndActorOverlap(AActor* OverlappedActor, AActor* OtherActor);

	FVector GetItemLocation();
	void SetItemLocation(FVector location);

	void SetCurrentTargetToInactive();
	ATrackedObject* GetCurrentTarget();

};
