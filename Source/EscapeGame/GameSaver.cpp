// Fill out your copyright notice in the Description page of Project Settings.

#include "GameSaver.h"


// Sets default values
AGameSaver::AGameSaver()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGameSaver::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGameSaver::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

AActor* AGameSaver::SpawnActorCopy(AActor* ref)
{
	
	UWorld* world = GetWorld();
	FActorSpawnParameters spawnparams = FActorSpawnParameters();
	spawnparams.Template = ref;
	spawnparams.bNoFail = true;
	spawnparams.bDeferConstruction = false;
	spawnparams.bAllowDuringConstructionScript = true;
	spawnparams.Name = *ref->GetName();
	FTransform* transform = new FTransform(ref->GetTransform());

	return world->SpawnActor(ref->GetClass(), transform, spawnparams);
}
