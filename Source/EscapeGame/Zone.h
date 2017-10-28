// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyCharacter.h"
#include "Components/BoxComponent.h"
#include "Zone.generated.h"


USTRUCT()
struct FJoyStruct
{
	GENERATED_USTRUCT_BODY()


};


UCLASS()
class ESCAPEGAME_API AZone : public AActor
{
	GENERATED_BODY()

	bool HasItemToCheck;
	bool EnemyInZone;
	FVector ItemLocation;
	AEnemyCharacter* EnemyRef;
	UBoxComponent* BoxComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Items")
	TArray<AActor*> BPsToTrack;

public:	
	// Sets default values for this actor's properties
	AZone();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void UpdateZoneItems();

	bool IsEnemyInZone();

	bool GetHasItemToCheck();
	void SetHasItemToCheck(bool _b);

	UFUNCTION()
	void BeginActorOverlap(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	void EndActorOverlap(AActor* OverlappedActor, AActor* OtherActor);

};
