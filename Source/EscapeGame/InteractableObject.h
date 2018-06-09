// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableObject.generated.h"

UCLASS()
class ESCAPEGAME_API AInteractableObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractableObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction")
		bool Interact(AActor* ActorInstigator);
		virtual bool Interact_Implementation(AActor* ActorInstigator);

		UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction")
			FVector GetInteractLocation();
		virtual FVector GetInteractLocation_Implementation();

		
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	FORCEINLINE UTexture2D* GetIcon() { return InteractIcon; }

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
		UTexture2D* InteractIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
		//The desired distance from player to interact location
		float DesiredDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
		//Be less than - The desired angle - between look-at point and interact location. (in degrees).
		float DesiredAngle;
};
