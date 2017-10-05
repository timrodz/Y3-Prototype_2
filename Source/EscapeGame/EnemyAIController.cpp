// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyAIController.h"
#include "EnemyCharacter.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

AEnemyAIController::AEnemyAIController(const class FObjectInitializer& ObjectInitializer)
{
	BehaviorComp = ObjectInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorComp"));
	BlackboardComp = ObjectInitializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComp"));

	/* Match with the AI/ZombieBlackboard */
	PatrolLocationKeyName = "PatrolLocation";
	CurrentWaypointKeyName = "CurrentWaypoint";
	EnemyTypeKeyName = "EnemyType";
	TargetEnemyKeyName = "TargetEnemy";

	/* Initializes PlayerState so we can assign a team index to AI */
	//bWantsPlayerState = true;
}

void AEnemyAIController::Possess(APawn * InPawn)
{
	Super::Possess(InPawn);

	AEnemyCharacter* EnemyChar = Cast<AEnemyCharacter>(InPawn);
	if (EnemyChar)
	{
		if (EnemyChar->BehaviorTree->BlackboardAsset)
		{
			BlackboardComp->InitializeBlackboard(*EnemyChar->BehaviorTree->BlackboardAsset);

			/* Make sure the Blackboard has the type of bot we possessed */
			SetBlackboardEnemyType(EnemyChar->EnemyType);
		}

		BehaviorComp->StartTree(*EnemyChar->BehaviorTree);
	}
}

void AEnemyAIController::UnPossess()
{
	Super::UnPossess();

	/* Stop any behavior running as we no longer have a pawn to control */
	BehaviorComp->StopTree();
}

AEnemyWaypoint * AEnemyAIController::GetWaypoint()
{
	if (BlackboardComp)
	{
		return Cast<AEnemyWaypoint>(BlackboardComp->GetValueAsObject(CurrentWaypointKeyName));
	}

	return nullptr;
}

AFirstPersonCharacterController * AEnemyAIController::GetTargetEnemy()
{
	if (BlackboardComp)
	{
		return Cast<AFirstPersonCharacterController>(BlackboardComp->GetValueAsObject(TargetEnemyKeyName));
	}

	return nullptr;
}

void AEnemyAIController::SetWaypoint(AEnemyWaypoint * NewWaypoint)
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsObject(CurrentWaypointKeyName, NewWaypoint);
	}
}

void AEnemyAIController::SetTargetEnemy(APawn * NewTarget)
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsObject(TargetEnemyKeyName, NewTarget);
	}
}

void AEnemyAIController::SetBlackboardEnemyType(EEnemyType NewType)
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsEnum(EnemyTypeKeyName, (uint8)NewType);
	}
}
