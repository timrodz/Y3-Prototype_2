// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MyFunctionLibrary.generated.h"

USTRUCT(BlueprintType)
struct F_BP_ActorSpawnParameters
{

	GENERATED_BODY()

	F_BP_ActorSpawnParameters()
	{

	}
	F_BP_ActorSpawnParameters(FActorSpawnParameters a)
	{
		Name = a.Name;
		Template = a.Template;
		Owner = a.Owner;
		Instigator = a.Instigator;
		OverrideLevel = a.OverrideLevel;
		SpawnCollisionHandlingOverride = a.SpawnCollisionHandlingOverride;
		bRemoteOwned = a.IsRemoteOwned();
		bNoFail = a.bNoFail;
		bDeferConstruction = a.bDeferConstruction;
		bAllowDuringConstructionScript = a.bAllowDuringConstructionScript;
		ObjectFlags = a.ObjectFlags;
	}

	static FActorSpawnParameters Convert(F_BP_ActorSpawnParameters a)
	{
		FActorSpawnParameters b = FActorSpawnParameters();
		b.Name = a.Name;
		b.Template = a.Template;
		b.Owner = a.Owner;
		b.Instigator = a.Instigator;
		b.OverrideLevel = a.OverrideLevel;
		b.SpawnCollisionHandlingOverride = a.SpawnCollisionHandlingOverride;
		b.bNoFail = a.bNoFail;
		b.bDeferConstruction = a.bDeferConstruction;
		b.bAllowDuringConstructionScript = a.bAllowDuringConstructionScript;

		return b;
	}

	UPROPERTY(BlueprintReadWrite, Category = "Variables")
	/* A name to assign as the Name of the Actor being spawned. If no value is specified, the name of the spawned Actor will be automatically generated using the form [Class]_[Number]. */
	FName Name;

	UPROPERTY(BlueprintReadWrite, Category = "Variables")
		/* An Actor to use as a template when spawning the new Actor. The spawned Actor will be initialized using the property values of the template Actor. If left NULL the class default object (CDO) will be used to initialize the spawned Actor. */
	AActor* Template;

	UPROPERTY(BlueprintReadWrite, Category = "Variables")
		/* The Actor that spawned this Actor. (Can be left as NULL). */
	AActor* Owner;

	UPROPERTY(BlueprintReadWrite, Category = "Variables")
		/* The APawn that is responsible for damage done by the spawned Actor. (Can be left as NULL). */
	APawn*	Instigator;

	UPROPERTY(BlueprintReadWrite, Category = "Variables")
		/* The ULevel to spawn the Actor in, i.e. the Outer of the Actor. If left as NULL the Outer of the Owner is used. If the Owner is NULL the persistent level is used. */
	class	ULevel* OverrideLevel;

	UPROPERTY(BlueprintReadWrite, Category = "Variables")
		/** Method for resolving collisions at the spawn point. Undefined means no override, use the actor's setting. */
	ESpawnActorCollisionHandlingMethod SpawnCollisionHandlingOverride;

private:

	friend class UPackageMapClient;

	/* Is the actor remotely owned. This should only be set true by the package map when it is creating an actor on a client that was replicated from the server. */
	uint16	bRemoteOwned : 1;

public:

	bool IsRemoteOwned() const { return bRemoteOwned; }

	UPROPERTY(BlueprintReadWrite, Category = "Variables")
		/* Determines whether spawning will not fail if certain conditions are not met. If true, spawning will not fail because the class being spawned is `bStatic=true` or because the class of the template Actor is not the same as the class of the Actor being spawned. */
	uint16	bNoFail : 1;

	UPROPERTY(BlueprintReadWrite, Category = "Variables")
		/* Determines whether the construction script will be run. If true, the construction script will not be run on the spawned Actor. Only applicable if the Actor is being spawned from a Blueprint. */
	uint16	bDeferConstruction : 1;

	UPROPERTY(BlueprintReadWrite, Category = "Variables")
		/* Determines whether or not the actor may be spawned when running a construction script. If true spawning will fail if a construction script is being run. */
	uint16	bAllowDuringConstructionScript : 1;

	EObjectFlags ObjectFlags;
};

/**
 * 
 */
UCLASS(Blueprintable)
class ESCAPEGAME_API UMyFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	UFUNCTION(BlueprintCallable, Category = "Utility")
		static AActor* SpawnActorCopy(AActor* ref, F_BP_ActorSpawnParameters spawnparams);
	
	
};
