// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableObject.h"
#include "TrackedObject.generated.h"

class AZone;
/**
 * 
 */
UCLASS()
class ESCAPEGAME_API ATrackedObject : public AInteractableObject
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Tracking")
		int AlertLevel;

public:
	// Sets default values for this actor's properties
	ATrackedObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, Category = "Tracking")
		bool ActiveState;

	void SetActiveState(bool newState);
	bool GetActiveState();

	int GetAlertLevel();

	void SetCurrentZone(AZone* _zone);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction")
		bool Interact(AActor* ActorInstigator);
		virtual bool Interact_Implementation(AActor* ActorInstigator) override;

	UPROPERTY(BlueprintReadWrite, Category = "Zone")
		AZone* Zone;
};


	
	

