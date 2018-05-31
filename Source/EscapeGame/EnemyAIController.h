// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "FirstPersonCharacterController.h"
#include "EnemyCharacter.h"
#include "EnemyWaypoint.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Actions/PawnAction.h"
#include "BehaviorTree/BTTaskNode.h"
#include <vector>

// Casting messages
#include "InteractableObject.h"

#include "EnemyAIController.generated.h"

UENUM(BlueprintType)
enum class EEnemyAIMode : uint8
{
	WAITING,
	SEEN,
	HEARD_NOISE,
	CHASING,
	LAST_KNOWN_LOCATION,
	EVENT,
	WANDER
};

UCLASS()
class ESCAPEGAME_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

	AEnemyAIController(const class FObjectInitializer& ObjectInitializer);

	/* Called whenever the controller possesses a character bot */
	virtual void Possess(class APawn* InPawn) override;
	virtual void UnPossess() override;

public:
	/* New Enemy enum to define what its doing atm */
	UPROPERTY(EditAnywhere, Category = "AI")
		EEnemyAIMode CurrentMode;

private:

	std::vector <AEnemyWaypoint*> Waypoints;
	AEnemyWaypoint* DebugWaypoint;
	

	// Blackboard and Behaviour tree
	UBehaviorTreeComponent* BehaviorComp;
	UBlackboardComponent* BlackboardComp;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		FName TargetEnemyKeyName;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		FName TargetLocationKeyName;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		FName CurrentWaypointKeyName;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		FName EnemyTypeKeyName;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		FName EventLocationKeyName;
	
public:
	UFUNCTION()
	void OnHearNoise(FVector location);

	UFUNCTION(BlueprintCallable, Category = "AI")
		void FindWaypoint();

	AEnemyWaypoint* GetWaypoint();
	UBTTaskNode* SetNextWaypoint;

	bool IsTargetLocationSet();

	UFUNCTION(BlueprintCallable, Category = "AI")
	void SetTargetLocation(FVector location);
	UFUNCTION(BlueprintCallable, Category = "AI")
	FVector GetTheTargetLocation();
	
	AFirstPersonCharacterController* GetTargetEnemy();
	void SetTargetEnemy(APawn* NewTarget);

	UFUNCTION(BlueprintCallable, Category = "AI")
		void SetWaypoint(AEnemyWaypoint* NewWaypoint);

	UFUNCTION(BlueprintCallable, Category = "AI")
		void SetWaypointNull();

	UFUNCTION(BlueprintCallable, Category = "AI")
		bool GetHasHeardNoise();

	UFUNCTION(BlueprintCallable, Category = "AI")
		bool IsWandering();

	UFUNCTION(BlueprintCallable, Category = "AI")
		void SetHeardNoiseLocation(FVector location);

	UFUNCTION(BlueprintCallable, Category = "AI")
		EEnemyAIMode GetCurrentState();

	UFUNCTION(BlueprintCallable, Category = "AI")
		bool GetShouldWander();

	UFUNCTION(BlueprintCallable, Category = "AI")
		void SetShouldWander(bool ShouldWander);

	UFUNCTION(BlueprintCallable, Category = "AI")
		void SetEventLocation(FVector location);

	UFUNCTION(BlueprintCallable, Category = "AI")
		FVector GetEventLocation();

	UFUNCTION(BlueprintCallable, Category = "AI")
		bool IsEventActive();

	UFUNCTION(BlueprintCallable, Category = "AI")
		void SetEventActive(bool _b);

	UFUNCTION(BlueprintCallable, Category = "AI")
		void SetInvestigateLastKnownLocation();

	UFUNCTION(BlueprintCallable, Category = "AI")
		bool GetIsInvestigatingLastKnownLocation();

	void SetBlackboardEnemyType(EEnemyType NewType);

	void DrawDebugLineToTarget();

	/** Returns BehaviorComp subobject **/
	FORCEINLINE UBehaviorTreeComponent* GetBehaviorComp() const { return BehaviorComp; }
	FORCEINLINE UBlackboardComponent* GetBlackboardComp() const { return BlackboardComp; }
};
