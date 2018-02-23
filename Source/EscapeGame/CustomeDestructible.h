// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PhysicsEngine/DestructibleActor.h"
#include "CustomeDestructible.generated.h"

/**
 * 
 */
UCLASS()
class ESCAPEGAME_API ACustomeDestructible : public ADestructibleActor
{
	GENERATED_BODY()

public:

	//Sets default values for this actor's properties
	ACustomeDestructible();

	UFUNCTION(BlueprintCallable, Category = "Physics")
		void CustomHit();

	UFUNCTION(BlueprintImplementableEvent)
		void Respawn();

	UPROPERTY(BlueprintReadWrite, Category = "Spawn")
		bool bNeedsRespawning = false;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	void BeginReconstruction();

	void TickReconstruction(float deltaTime);

	void DelayedDestroy();

	UPROPERTY(EditInstanceOnly)
		bool Broken = false;
	UPROPERTY(EditInstanceOnly)
		bool Reconstructable = false;
	UPROPERTY(EditInstanceOnly)
		bool AlmostFinished = false;
	UPROPERTY(EditInstanceOnly)
		bool GravityWasEnabled;
	UPROPERTY(EditInstanceOnly)
		float TimeSinceHit = 0.0f;

	//<summary>
	//</summary>
	UPROPERTY(EditInstanceOnly, Category = "Physics")
		float VaryingTolerance = 100.0f;

	//<summary>
	// Length of reconstructing animation.
	//</summary>
	UPROPERTY(EditInstanceOnly, Category = "Physics")
		float AnimationLength = 2.5f;

	UPROPERTY(EditInstanceOnly)
		TArray<FTransform> OriginTransforms;
	UPROPERTY(EditInstanceOnly)
		TArray<FTransform> ReconstructTransforms;

	UPROPERTY(EditInstanceOnly)
		FTimerHandle CustomHitDelay;
	UPROPERTY(EditInstanceOnly)
		FTimerHandle DestroyActorDelay;
};
