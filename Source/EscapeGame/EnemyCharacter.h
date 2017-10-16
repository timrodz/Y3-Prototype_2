// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/TextRenderComponent.h"
#include "EnemyCharacter.generated.h"

UENUM(BlueprintType)
enum class EEnemyType : uint8
{
	// For when we have more enemies

	Standard,
	Other,
};

UCLASS()
class ESCAPEGAME_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

	/* Last time the player was spotted */
	float LastSeenTime;

	/* Last time the player was heard */
	float LastHeardTime;

	float TimeArrivedAtTarget;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		float TimeToWaitAtTargetLocation;



	/* Resets after sense time-out to avoid unnecessary clearing of target each tick */
	bool bSensedTarget;

	bool bPatrolPointsSet;

	bool bIsCloseToTargetLocation;

	bool bTargetTimerSet;

	class AEnemyAIController* AIController;

	/* Time-out value to clear the sensed position of the player. Should be higher than Sense interval in the PawnSense component not never miss sense ticks. */
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float SenseTimeOut;

	UPROPERTY(VisibleAnywhere, Category = "AI")
		class UPawnSensingComponent* PawnSensingComp;

	UFUNCTION()
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// Called to bind functionality to input
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned

	UFUNCTION()
		void OnSeePlayer(APawn* Pawn);

	UFUNCTION()
		void OnHearNoise(APawn* PawnInstigator, const FVector& Location, float Volume);
	

public:

	AEnemyCharacter(const class FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditAnywhere, Category = "AI")
		EEnemyType EnemyType;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		class UBehaviorTree* BehaviorTree;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		UTextRenderComponent* DebugTextRender;
		
	void SetEnemyType(EEnemyType NewType);

	void SetPatrolPoints(bool b);

	bool IsCloseToTargetLocation();
};


