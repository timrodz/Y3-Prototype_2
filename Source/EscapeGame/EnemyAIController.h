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

//Runtime / AIModule / Classes / BehaviorTree / BehaviorTreeComponent.h

UCLASS()
class ESCAPEGAME_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

	AEnemyAIController(const class FObjectInitializer& ObjectInitializer);

	/* Called whenever the controller possesses a character bot */
	virtual void Possess(class APawn* InPawn) override;

	virtual void UnPossess() override;

	bool bShouldWander;

	std::vector <AEnemyWaypoint*> Waypoints;

	UBehaviorTreeComponent* BehaviorComp;

	UBlackboardComponent* BlackboardComp;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		FName TargetEnemyKeyName;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		FName CurrentWaypointKeyName;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		FName EnemyTypeKeyName;
	
public:

	UBTTaskNode* SetNextWaypoint;

	AEnemyWaypoint* GetWaypoint();

	AFirstPersonCharacterController* GetTargetEnemy();

	void SetWaypoint(AEnemyWaypoint* NewWaypoint);

	void SetShouldWander(bool ShouldWander);

	UFUNCTION(BlueprintCallable, Category = "AI")
	bool GetShouldWander();

	void SetTargetEnemy(APawn* NewTarget);

	void SetBlackboardEnemyType(EEnemyType NewType);

	UFUNCTION(BlueprintCallable, Category = "AI")
	void FindWaypoint();

	/** Returns BehaviorComp subobject **/
	FORCEINLINE UBehaviorTreeComponent* GetBehaviorComp() const { return BehaviorComp; }

	FORCEINLINE UBlackboardComponent* GetBlackboardComp() const { return BlackboardComp; }
};
