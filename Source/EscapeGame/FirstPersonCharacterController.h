// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FirstPersonCharacterController.generated.h"

UCLASS(Blueprintable)
class ESCAPEGAME_API AFirstPersonCharacterController : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFirstPersonCharacterController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables | New")
		bool Sprinting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables | New")
		bool Pushing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables | New")
		FVector PushVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables | New")
		FVector InspectableLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables | New")
		FRotator InspectableRotation;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	//	AActor* InspectionObject;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables | New")
		bool isInspecting;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables | New")
		float TraceLength;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Variables | New")
		float Delta;
	
};
