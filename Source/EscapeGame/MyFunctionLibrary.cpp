// Fill out your copyright notice in the Description page of Project Settings.

#include "MyFunctionLibrary.h"
#include "Engine/World.h"



AActor* UMyFunctionLibrary::SpawnActorCopy(AActor* ref, F_BP_ActorSpawnParameters spawnparams)
{
	UWorld* world = ref->GetWorld();
	FActorSpawnParameters spawnparam = F_BP_ActorSpawnParameters::Convert(spawnparams);
	/*spawnparams.Template = ref;
	spawnparams.bNoFail = true;
	spawnparams.bDeferConstruction = false;
	spawnparams.bAllowDuringConstructionScript = true;
	spawnparams.Name = *ref->GetName();*/
	FTransform* transform = new FTransform(ref->GetTransform());

	return world->SpawnActor(ref->GetClass(), transform, spawnparam);
}

