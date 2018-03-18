// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"

#include "GameFramework/Actor.h"
#include "GameSaver.generated.h"


UCLASS(Blueprintable)
class ESCAPEGAME_API AGameSaver : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameSaver();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		AActor* SpawnActorCopy(AActor* ref);


	
	
};
