// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "Components/ActorComponent.h"
#include "Grabber.generated.h"

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPEGAME_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	// How far ahead of the player can we reach in cm
	UPROPERTY(EditDefaultsOnly, Category = "Raycast")
	float Reach = 300.f;

	UPROPERTY(EditDefaultsOnly, Category = "Pickup")
	float ThrowStrength = 100.0f;

	// Called when Left-click (Throw button) is pressed
	UFUNCTION(BlueprintCallable, Category = "Throw")
	void Throw(FVector direction);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Grab")
		void OnGrabComponent(UPrimitiveComponent* Component);
	virtual void OnGrabComponent_Implementation(UPrimitiveComponent* Component);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Grab")
		void OnReleaseComponent(UPrimitiveComponent* Component);
	virtual void OnReleaseComponent_Implementation(UPrimitiveComponent* Component);

	UFUNCTION(BlueprintCallable)
	void Release();

public:

private:

	UPhysicsHandleComponent* PhysicsHandle = nullptr;

	UInputComponent* InputComponent = nullptr;

	// Ray-cast and grab what's in reach
	void Grab();

	// Called when grab is released
	



	// Find (assumed) attached phyics handle
	void FindPhysicsHandleComponent();

	// Setup (assumed) attached input component
	void SetupInputComponent();

	// Return hit for first physics body in reach
	const FHitResult GetFirstPhysicsBodyInReach();

	// Returns current start of reach line
	FVector GetReachLineStart();

	// Returns current end of reach line
	FVector GetReachLineEnd();
	
};
