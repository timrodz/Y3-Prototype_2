// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyAIController.h"
#include "EnemyCharacter.h"
#include "EnemyWaypoint.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EngineUtils.h"
#include "Kismet/KismetSystemLibrary.h" // for draw debug line

AEnemyAIController::AEnemyAIController(const class FObjectInitializer& ObjectInitializer)
{
	BehaviorComp = ObjectInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorComp"));
	BlackboardComp = ObjectInitializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComp"));

	/* Match with the AI/ZombieBlackboard */
	CurrentWaypointKeyName = "CurrentWaypoint";
	EnemyTypeKeyName = "EnemyType";
	TargetEnemyKeyName = "TargetEnemy";

	DebugWaypoint = nullptr;

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

void AEnemyAIController::SetShouldWander(bool ShouldWander)
{
	bShouldWander = ShouldWander;
}

bool AEnemyAIController::GetShouldWander()
{
	return bShouldWander;
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

void AEnemyAIController::DrawDebugLineToTarget()
{
	if (DebugWaypoint != nullptr)
	{
		//Debug Line
		AEnemyCharacter* Character = Cast<AEnemyCharacter>(GetPawn());
		FVector start = Character->GetActorLocation();
		FVector end = DebugWaypoint->GetActorLocation() + FVector(0, 0, 80);
		UKismetSystemLibrary::DrawDebugLine(Character, start, end, FLinearColor::Red, 0.05f, 5.0f);
	}	
}

void AEnemyAIController::FindWaypoint()
{
	UE_LOG(LogTemp, Error, TEXT("Find waypoint called"));

	Waypoints.clear();

	// Find all waypoints in map
	for (TActorIterator<AEnemyWaypoint> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		AEnemyWaypoint *WP = *ActorItr;
		// Push to waypoints vector
		Waypoints.push_back(WP);

		//ClientMessage(ActorItr->GetName());
		//ClientMessage(ActorItr->GetActorLocation().ToString());
	}
	
	int size = Waypoints.size();

	if (size != 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Waypoint set"));

		// Pick random waypoint
		int rand = FMath::RandRange(0, size - 1);

		Cast<AEnemyCharacter>(GetPawn())->SetPatrolPoints(true);

		DebugWaypoint = (Waypoints[rand]);

		// Set current waypoint in BB
		SetWaypoint(Waypoints[rand]);
	}
	else // No waypoints in map
	{
		UE_LOG(LogTemp, Error, TEXT("Waypoint NOT set"));

		Cast<AEnemyCharacter>(GetPawn())->SetPatrolPoints(false);

		SetWaypoint(nullptr);
		DebugWaypoint = nullptr;
	}
}
