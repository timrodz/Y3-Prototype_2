// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyAIController.h"

AEnemyAIController::AEnemyAIController()
{

}

void AEnemyAIController::Possess(APawn * InPawn)
{
}

void AEnemyAIController::UnPossess()
{
}

AEnemyWaypoint * AEnemyAIController::GetWaypoint()
{
	return nullptr;
}

AFirstPersonCharacterController * AEnemyAIController::GetTargetEnemy()
{
	return nullptr;
}

void AEnemyAIController::SetWaypoint(AEnemyWaypoint * NewWaypoint)
{
}

void AEnemyAIController::SetTargetEnemy(APawn * NewTarget)
{
}

void AEnemyAIController::SetBlackboardBotType(EEnemyType NewType)
{
}
