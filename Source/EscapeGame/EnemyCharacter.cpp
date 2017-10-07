// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyCharacter.h"
#include "EnemyAIController.h"
#include "FirstPersonCharacterController.h"

#include "Perception/PawnSensingComponent.h"


// Sets default values
AEnemyCharacter::AEnemyCharacter(const class FObjectInitializer& ObjectInitializer)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bSensedTarget = false;
	bPatrolPointsSet = false;

	// Assign AI controller class in blueprint extension of this class

	/* Our sensing component to detect players by visibility and noise checks. */
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	PawnSensingComp->SetPeripheralVisionAngle(60.0f);
	PawnSensingComp->SightRadius = 2000;
	PawnSensingComp->HearingThreshold = 600;
	PawnSensingComp->LOSHearingThreshold = 1200;

	/* Adjusting capsule
	GetCapsuleComponent()->SetCapsuleHalfHeight(96.0f, false);
	GetCapsuleComponent()->SetCapsuleRadius(42.0f);*/

	/* These values are matched up to the CapsuleComponent above and are used to find navigation paths */
	/*GetMovementComponent()->NavAgentProps.AgentRadius = 42;
	GetMovementComponent()->NavAgentProps.AgentHeight = 192;*/

	/* Set enemy defaults */
	EnemyType = EEnemyType::Standard;
	SenseTimeOut = 2.5f;
}


// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	/* This is the earliest moment we can bind our delegates to the component */
	if (PawnSensingComp)
	{
		// Set these pawnsensing functions to call our functions
		PawnSensingComp->OnSeePawn.AddDynamic(this, &AEnemyCharacter::OnSeePlayer);
		PawnSensingComp->OnHearNoise.AddDynamic(this, &AEnemyCharacter::OnHearNoise);
	}	

	DebugTextRender = this->FindComponentByClass<UTextRenderComponent>();
	AIController = Cast<AEnemyAIController>(GetController());
	AIController->SetShouldWander(true);
	AIController->SetWaypoint(nullptr);
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UE_LOG(LogTemp, Warning, TEXT("Time since seen: %f   Time since heard: %f"), GetWorld()->TimeSeconds - LastSeenTime, GetWorld()->TimeSeconds - LastHeardTime);

	/* Check if the last time we sensed a player is beyond the time out value to prevent bot from endlessly following a player. */
	if (bSensedTarget && (GetWorld()->TimeSeconds - LastSeenTime) > SenseTimeOut
		&& (GetWorld()->TimeSeconds - LastHeardTime) > SenseTimeOut)
	{
		AIController = Cast<AEnemyAIController>(GetController());
		if (AIController)
		{
			bSensedTarget = false;
			/* Reset */
			AIController->SetTargetEnemy(nullptr);

			UE_LOG(LogTemp, Warning, TEXT("RESET TARGET TO NULL"));
		}
	}

	if (bPatrolPointsSet && !bSensedTarget)
	{
		DebugTextRender->SetText("Patrolling");
		AIController = Cast<AEnemyAIController>(GetController());
		AIController->DrawDebugLineToTarget();
	}
	else if(!bPatrolPointsSet && !bSensedTarget)
	{
		DebugTextRender->SetText("Wait");
	}
}

void AEnemyCharacter::OnSeePlayer(APawn * Pawn)
{
	/*if (!IsAlive())
	{
		return;
	}*/

	//if (!bSensedTarget)
	//{
	//	//BroadcastUpdateAudioLoop(true);
	//}

	UE_LOG(LogTemp, Error, TEXT("Player SEEN"));

	/* Keep track of the time the player was last sensed in order to clear the target */
	LastSeenTime = GetWorld()->GetTimeSeconds();
	bSensedTarget = true;

	AIController = Cast<AEnemyAIController>(GetController());
	AFirstPersonCharacterController* SensedPawn = Cast<AFirstPersonCharacterController>(Pawn);
	if (AIController)// && SensedPawn->IsAlive())
	{
		AIController->SetTargetEnemy(SensedPawn);
		DebugTextRender->SetText("Chasing-Seen");
	}
}

void AEnemyCharacter::OnHearNoise(APawn * PawnInstigator, const FVector & Location, float Volume)
{
	//if (!IsAlive())
	//{
	//	return;
	//}

	//if (!bSensedTarget)
	//{
	//	BroadcastUpdateAudioLoop(true);
	//}

	UE_LOG(LogTemp, Error, TEXT("HEARD Noise"));

	bSensedTarget = true;
	LastHeardTime = GetWorld()->GetTimeSeconds();

	AIController = Cast<AEnemyAIController>(GetController());
	if (AIController)
	{
		AIController->SetTargetEnemy(PawnInstigator);
		//AIController->SetTargetLocation(PawnInstigator->GetActorLocation()); //  change to some kind of delay??? Watch vids etc
		DebugTextRender->SetText("Chasing-Heard");
	}
}

void AEnemyCharacter::SetEnemyType(EEnemyType NewType)
{
	EnemyType = NewType;

	AIController = Cast<AEnemyAIController>(GetController());
	if (AIController)
	{
		AIController->SetBlackboardEnemyType(NewType);
	}
}

void AEnemyCharacter::SetPatrolPoints(bool b)
{
	bPatrolPointsSet = b;
}


// Called to bind functionality to input
//void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
//{
//	Super::SetupPlayerInputComponent(PlayerInputComponent);
//
//}

