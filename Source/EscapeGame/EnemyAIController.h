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
#include "EnemyAIController.generated.h"

// Delegates
//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTestDelegate);
//DECLARE_DELEGATE(FTestDelegate);

UCLASS()
class ESCAPEGAME_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

	AEnemyAIController(const class FObjectInitializer& ObjectInitializer);

	/* Called whenever the controller possesses a character bot */
	virtual void Possess(class APawn* InPawn) override;
	virtual void UnPossess() override;

	bool bShouldWander;
	bool bEventActive;
	bool bTargetLocationSet;

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

//	UPROPERTY(EditDefaultsOnly, Category = "AI")
//		FName ManualLocationKeyName;
	
public:

	//void TestFunction();

//	UPROPERTY(BlueprintAssignable, Category = "Test")
//		FTestDelegate OnTestDelegate;

	UFUNCTION(BlueprintCallable, Category = "AI")
		void FindWaypoint();

	AEnemyWaypoint* GetWaypoint();
	UBTTaskNode* SetNextWaypoint;

	bool IsTargetLocationSet();
	void SetTargetLocation(FVector location);
	FVector GetTheTargetLocation();
	
	AFirstPersonCharacterController* GetTargetEnemy();
	void SetTargetEnemy(APawn* NewTarget);

	void SetWaypoint(AEnemyWaypoint* NewWaypoint);

	UFUNCTION(BlueprintCallable, Category = "AI")
	bool GetShouldWander();
	void SetShouldWander(bool ShouldWander);

	//void SetManualLocation(FVector location);
	//FVector GetManualLocation();

	void SetBlackboardEnemyType(EEnemyType NewType);

	UFUNCTION(BlueprintCallable, Category = "AI")
	bool IsEventActive();
	void SetEventActive(bool _b);

	void DrawDebugLineToTarget();

	/** Returns BehaviorComp subobject **/
	FORCEINLINE UBehaviorTreeComponent* GetBehaviorComp() const { return BehaviorComp; }
	FORCEINLINE UBlackboardComponent* GetBlackboardComp() const { return BlackboardComp; }
};
