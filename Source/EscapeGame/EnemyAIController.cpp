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

	/* Match with the AI Blackboard */
	CurrentWaypointKeyName = "CurrentWaypoint";
	EnemyTypeKeyName = "EnemyType";
	TargetEnemyKeyName = "TargetEnemy";
	TargetLocationKeyName = "TargetLocation";
	EventLocationKeyName = "EventLocation";

	DebugWaypoint = nullptr;
	CurrentMode = EEnemyAIMode::WAITING;

	/* Initializes PlayerState so we can assign a team index to AI */
	//bWantsPlayerState = true;
}

void AEnemyAIController::Possess(APawn * InPawn)
{
	Super::Possess(InPawn);

	// Set up behaviour tree and blackboard
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

AEnemyWaypoint* AEnemyAIController::GetWaypoint()
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

void AEnemyAIController::SetWaypointNull()
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsObject(CurrentWaypointKeyName, nullptr);
	}
}

void AEnemyAIController::SetShouldWander(bool ShouldWander)
{
	CurrentMode = EEnemyAIMode::WANDER;
}

bool AEnemyAIController::IsTargetLocationSet()
{
	return CurrentMode == EEnemyAIMode::LAST_KNOWN_LOCATION;
}

bool AEnemyAIController::GetShouldWander()
{
	return CurrentMode == EEnemyAIMode::WANDER;
}

void AEnemyAIController::SetTargetEnemy(APawn * NewTarget)
{
	//UE_LOG(LogTemp, Warning, TEXT("Set target enemy called"));
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsObject(TargetEnemyKeyName, NewTarget);
	}
}

void AEnemyAIController::SetTargetLocation(FVector location)
{
	if (BlackboardComp)
	{
		//UE_LOG(LogTemp, Error, TEXT("Setting target location"));
		BlackboardComp->SetValueAsVector(TargetLocationKeyName, location);
		CurrentMode = EEnemyAIMode::LAST_KNOWN_LOCATION;
	}
}

FVector AEnemyAIController::GetTheTargetLocation()
{
	if (BlackboardComp)
	{
		return BlackboardComp->GetValueAsVector(TargetLocationKeyName);
	}

	UE_LOG(LogTemp, Error, TEXT("No target location found on blackboard"));
	return FVector(0, 0, 0);
}

void AEnemyAIController::SetEventLocation(FVector location)
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsVector(EventLocationKeyName, location);
	}
}

FVector AEnemyAIController::GetEventLocation()
{
	if (BlackboardComp)
	{
		return BlackboardComp->GetValueAsVector(EventLocationKeyName);
	}

	UE_LOG(LogTemp, Error, TEXT("No event location found on blackboard"));
	return FVector(0, 0, 0);
}

bool AEnemyAIController::GetIsInvestigatingLastKnownLocation()
{
	return CurrentMode == EEnemyAIMode::LAST_KNOWN_LOCATION;
}

void AEnemyAIController::SetBlackboardEnemyType(EEnemyType NewType)
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsEnum(EnemyTypeKeyName, (uint8)NewType);
	}
}

bool AEnemyAIController::IsEventActive()
{
	return CurrentMode == EEnemyAIMode::EVENT;
}

void AEnemyAIController::SetEventActive(bool _b)
{
	if (_b)
		CurrentMode = EEnemyAIMode::EVENT;
	else
		CurrentMode = EEnemyAIMode::WAITING;
}

void AEnemyAIController::SetInvestigateLastKnownLocation()
{
	CurrentMode = EEnemyAIMode::LAST_KNOWN_LOCATION;
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
	//UE_LOG(LogTemp, Error, TEXT("Find waypoint called"));

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
		//UE_LOG(LogTemp, Warning, TEXT("Waypoint set"));

		// Pick random waypoint
		int rand = FMath::RandRange(0, size - 1);

		Cast<AEnemyCharacter>(GetPawn())->SetPatrolPoints(true);

		DebugWaypoint = (Waypoints[rand]);

		// Set current waypoint in blackboard
		SetWaypoint(Waypoints[rand]);
	}
	else // No waypoints in map
	{
		//UE_LOG(LogTemp, Warning, TEXT("Waypoint NOT set"));

		Cast<AEnemyCharacter>(GetPawn())->SetPatrolPoints(false);

		SetWaypoint(nullptr);
		DebugWaypoint = nullptr;
	}
}

void AEnemyAIController::OnHearNoise(FVector location)
{
	UE_LOG(LogTemp, Warning, TEXT("AI CONTROLLER - OnHearNoise"));
	//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, "HEAR NOISE");
}