// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InteractableObject.h"

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/TextRenderComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnemyCharacter.generated.h"

class AZone; 

UENUM(BlueprintType)
enum class EEnemyType : uint8
{
	// For when we have more enemies
	Standard,
	Other,
};

UENUM(BlueprintType)
enum class EEnemyMode : uint8
{
	WAITING,
	SENSED,
	CHASING,
	INSPECTING,
	EVENT

};

UCLASS(Blueprintable)
class ESCAPEGAME_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "AI")
	class AEnemyAIController* AIController;

	/* New Enemy enum to define what its doing atm */
	EEnemyMode CurrentMode = EEnemyMode::WAITING;

	/* Last time the player was spotted */
	float LastSeenTime;

	/* Last time the player was heard */
	float LastHeardTime;

	float TimeArrivedAtTarget;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		float TimeToWaitAtTargetLocation = 3.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Sensing")
	//Cone angle for vision 
		float PeripheralVisionAngle = 60.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Sensing")
	//Cone radius for visual sight
		float SightRadius = 2000;

	UPROPERTY(EditDefaultsOnly, Category = "Sensing")
	//Sounds heard at e.g 600 or less, will be at max importance
		float HearingThreshold = 600;

	UPROPERTY(EditDefaultsOnly, Category = "Sensing")
	//Sound importance lost at distance of e.g 1200
		float LOSHearingThreshold = 1200;

	/* Resets after sense time-out to avoid unnecessary clearing of target each tick */
	UPROPERTY(EditAnywhere, Category = "AI")
	bool bPatrolPointsSet;
	UPROPERTY(EditAnywhere, Category = "AI")
	bool bIsCloseToTargetLocation;
	UPROPERTY(EditAnywhere, Category = "AI")
	bool bTargetTimerSet;
	UPROPERTY(EditAnywhere, Category = "AI")
	bool bSensedTarget;
	UPROPERTY(EditAnywhere, Category = "AI")
	bool bIsPatrolling;
	UPROPERTY(EditAnywhere, Category = "AI")
	FVector LastLocation;

	UPROPERTY(EditAnywhere, Category = "AI")
		float StuckTimer;

	UPROPERTY(EditAnywhere, Category = "AI")
		float ChaseRadius = 200.0f;

	UPROPERTY(EditAnywhere, Category = "AI")
		float MinimumChaseTime = 3.0f;
	float MinimumChaseTimer = 0.0f;


//	bool StuckTimerSet;

	UCharacterMovementComponent* CharMovement;


	UPROPERTY(EditAnywhere, Category = "AI")
		bool DebugAIText = false;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		float StuckThreshold = 10.0f;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		float WalkSpeedDefault = 150.0f;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		float WalkSpeedSensedTarget = 250.0f;

	/* Time-out value to clear the sensed position of the player. Should be higher than Sense interval in the PawnSense component not never miss sense ticks. */
	UPROPERTY(EditDefaultsOnly, Category = "AI")
		float SenseTimeOut = 2.5f;

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

	UFUNCTION(BlueprintCallable)
		void OnSeePlayer(APawn* Pawn);

	UFUNCTION(BlueprintCallable)
		void OnHearPlayer(APawn* PawnInstigator, const FVector& Location, float Volume);

	UFUNCTION()
		void OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(BlueprintCallable)
		bool HasSensedTarget();

	UFUNCTION(BlueprintCallable)
		void SetSensedTargetTrue();

	UFUNCTION(BlueprintCallable)
		bool IsEnemyPatrolling();
	
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

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "AI")
		float TimeSinceLastSeen;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		float TargetDistanceThreshold = 60.0f;

	bool IsCloseToLocation(FVector _location);

	//void CheckIfStuck(FVector CurrentPos, FVector LastPos);

	void CheckIfStuck();

	UFUNCTION(BlueprintCallable)
		void MoveToLocation(FVector newLocation);

	//// Zone stuff

	UPROPERTY(BlueprintReadWrite, Category = "AI")
		AZone* CurrentZone;

	UFUNCTION(BlueprintCallable)
		void CheckForActiveZoneEvents();

	UFUNCTION(BlueprintCallable)
		void SetZoneEventActive();

	UFUNCTION(BlueprintCallable)
		void SetZoneEventComplete();
};


