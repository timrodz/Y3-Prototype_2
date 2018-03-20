// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/SphereComponent.h"
#include "Blueprint/UserWidget.h"
#include "../Classes/Camera/CameraComponent.h"
#include "InteractionComponent.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPEGAME_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInteractionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	USphereComponent* Collider;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	UPROPERTY(EditDefaultsOnly, Category = "Range")
		float Size = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object")
		AActor* PlayerRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object")
		AActor* CurrentActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object")
		UCameraComponent* Camera;

	TArray<AActor*> Actors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object")
		TSubclassOf<UUserWidget> IconWidget;

	UUserWidget* MainIcon;



private:

	float m_LowestLength;
	int m_LowestIndex;


	//FVector GetProjectionPoint();

	AActor* GetLowest() { return Actors[m_LowestIndex]; }

	//void ProjectActorToScreen(OUT const FVector2D& ScreenPosition, OUT const bool& Success);
	//void IsCloser(AActor* actor, OUT const float& distance, OUT const bool& closer);

	
};
