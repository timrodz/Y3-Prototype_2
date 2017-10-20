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
	bIsCloseToTargetLocation = false;
	bTargetTimerSet = false;

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
	TimeToWaitAtTargetLocation = 3.0f;
	TargetDistanceThreshold = 60.0f;
	StuckTimer = 0.0f;
	StuckTimerSet = false;
	StuckThreshold = 15.0f;
	DebugAIText = false;
}

bool AEnemyCharacter::HasSensedTarget()
{
	return bSensedTarget;
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
		PawnSensingComp->OnHearNoise.AddDynamic(this, &AEnemyCharacter::OnHearPlayer);
	}

	this->OnActorHit.AddDynamic(this, &AEnemyCharacter::OnHit);

	DebugTextRender = this->FindComponentByClass<UTextRenderComponent>();
	AIController = Cast<AEnemyAIController>(GetController());
	AIController->SetShouldWander(true);
	AIController->SetWaypoint(nullptr);
	//AIController->SetEventLocation(FVector(-1140, -1960, 80));
	//AIController->SetTargetLocation(FVector(0));
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	//UE_LOG(LogTemp, Warning, TEXT("Start of Tick: %s"), *this->GetActorLocation().ToString());

	Super::Tick(DeltaTime);

	//UE_LOG(LogTemp, Warning, TEXT("New: %s    Old: %s"), *this->GetActorLocation().ToString(), *LastLocation.ToString());
	//UE_LOG(LogTemp, Warning, TEXT("Time since seen: %f   Time since heard: %f"), GetWorld()->TimeSeconds - LastSeenTime, GetWorld()->TimeSeconds - LastHeardTime);
	//UE_LOG(LogTemp, Warning, TEXT("Target Location: %s"), *AIController->GetTargetLocation().ToString());

	//AIController->SetTargetLocation(FVector(-1340.0f, -1290.0f, 30.0f));
	AIController = Cast<AEnemyAIController>(GetController());
	
	// Check for events
	if (AIController->IsEventActive() && AIController->IsTargetLocationSet())
	{
		//UE_LOG(LogTemp, Warning, TEXT("Event Active"));

		// Event sets target location in blackboard

		//UE_LOG(LogTemp, Warning, TEXT("Target Location: %s"), *AIController->GetTargetLocation().ToString());

		DebugTextRender->SetText(FText::FromString("Event"));
		UE_LOG(LogTemp, Warning, TEXT("My Loc: %s   Event Loc: %s"), *this->GetActorLocation().ToString(), *AIController->GetEventLocation().ToString());

		if (IsCloseToLocation(AIController->GetEventLocation()))
		{
		    UE_LOG(LogTemp, Warning, TEXT("At Event Target")); 
			//AIController->SetEventActive(false);			
		}

		return;	
	}

	FVector TargetLoc = AIController->GetTheTargetLocation();
	bIsCloseToTargetLocation = IsCloseToLocation(TargetLoc);

	// Check if enemy has seen or heard anything
	if (bSensedTarget)
	{
		//UE_LOG(LogTemp, Warning, TEXT("SENSED TARGET"));
		CheckIfStuck(this->GetActorLocation(), LastLocation);

		if (bIsCloseToTargetLocation && !bTargetTimerSet)
		{
			TimeArrivedAtTarget = GetWorld()->TimeSeconds;
			bTargetTimerSet = true;
			if (DebugAIText) { UE_LOG(LogTemp, Warning, TEXT("At Target Timer Started")); }
		}
		else if (!bIsCloseToTargetLocation)
		{
			bTargetTimerSet = false;
			TimeArrivedAtTarget = 0.0f;
		}
	}

	/* Check if the last time we sensed a player is beyond the time out value to prevent bot from endlessly following a player. */
	if (bIsCloseToTargetLocation && bSensedTarget && (GetWorld()->TimeSeconds - LastSeenTime) > SenseTimeOut
		&& (GetWorld()->TimeSeconds - LastHeardTime) > SenseTimeOut)// && IsCloseToTargetLocation())
//	if (bIsCloseToTargetLocation && bSensedTarget)
	{
		float Time = GetWorld()->TimeSeconds - TimeArrivedAtTarget;

		//UE_LOG(LogTemp, Warning, TEXT("Time at target: %f"), Time);

		if (GetWorld()->TimeSeconds - TimeArrivedAtTarget > TimeToWaitAtTargetLocation)
		{
			if (DebugAIText) { UE_LOG(LogTemp, Warning, TEXT("At Target Timer Finished")); }

			AIController = Cast<AEnemyAIController>(GetController());
			if (AIController)
			{
				bSensedTarget = false;
				if (DebugAIText) { UE_LOG(LogTemp, Warning, TEXT("Sensed target false")); }

				/* Reset */
				AIController->SetTargetEnemy(nullptr);

				if (DebugAIText) { UE_LOG(LogTemp, Warning, TEXT("Rest Target To Null")); }
			}
		}
	}
	
	// Patrolling
	if (bPatrolPointsSet && !bSensedTarget)
	{
		DebugTextRender->SetText(FText::FromString("Patrolling"));
		AIController = Cast<AEnemyAIController>(GetController());
		AIController->DrawDebugLineToTarget();
	}
	// Waiting
	else if(!bPatrolPointsSet && !bSensedTarget)
	{
		DebugTextRender->SetText(FText::FromString("Wait"));
	}
	//UE_LOG(LogTemp, Warning, TEXT("End of Tick: %s"), *this->GetActorLocation().ToString());

	LastLocation = this->GetActorLocation();
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

	//UE_LOG(LogTemp, Warning, TEXT("Player SEEN"));
	AIController = Cast<AEnemyAIController>(GetController());

	if (AIController->IsEventActive())
	{
		return;
	}

	/* Keep track of the time the player was last sensed in order to clear the target */
	LastSeenTime = GetWorld()->GetTimeSeconds();
	bSensedTarget = true;

	AFirstPersonCharacterController* SensedPawn = Cast<AFirstPersonCharacterController>(Pawn);

	if (AIController)// && SensedPawn->IsAlive())
	{
		AIController->SetTargetEnemy(SensedPawn);
		AIController->SetTargetLocation(SensedPawn->GetActorLocation());
		DebugTextRender->SetText(FText::FromString("Chasing-Seen"));
		if (DebugAIText) { UE_LOG(LogTemp, Error, TEXT("Set target location - seen")); }
	}
}

void AEnemyCharacter::OnHearPlayer(APawn * PawnInstigator, const FVector & Location, float Volume)
{
	//if (!IsAlive())
	//{
	//	return;
	//}

	//if (!bSensedTarget)
	//{
	//	BroadcastUpdateAudioLoop(true);
	//}

	AIController = Cast<AEnemyAIController>(GetController());

	if (AIController->IsEventActive())
	{
		return;
	}

	if (DebugAIText) { UE_LOG(LogTemp, Warning, TEXT("HEARD Noise")); }

	float DistanceToNoise = FVector::Dist(this->GetActorLocation(), PawnInstigator->GetActorLocation());
	//UE_LOG(LogTemp, Warning, TEXT("Distance to noise: %f"), DistanceToNoise);

	bSensedTarget = true;
	LastHeardTime = GetWorld()->GetTimeSeconds();

	

	//UE_LOG(LogTemp, Warning, TEXT("Sensed target set to true"));

	if (AIController)
	{
		AIController->SetTargetEnemy(PawnInstigator);
		AIController->SetTargetLocation(PawnInstigator->GetActorLocation());
		//AIController->SetTargetLocation(PawnInstigator->GetActorLocation()); //  change to some kind of delay??? Watch vids etc
		if (DebugAIText) { UE_LOG(LogTemp, Error, TEXT("Set target location - heard")); }
		DebugTextRender->SetText(FText::FromString("Chasing-Heard"));
	}

	else
	{
		UE_LOG(LogTemp, Error, TEXT("Hear player AI controller cast failed"));
	}
}

void AEnemyCharacter::OnHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	AFirstPersonCharacterController* SensedPawn = Cast<AFirstPersonCharacterController>(OtherActor);

	if (SensedPawn)
	{
		// UE_LOG(LogTemp, Warning, TEXT("Player Hit"));
		OnSeePlayer(SensedPawn);
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

bool AEnemyCharacter::IsCloseToLocation(FVector _location)
{
	// Check if enemy is close the target - as sometimes they can't reach the exact location

	AIController = Cast<AEnemyAIController>(GetController());
	FVector Loc = this->GetActorLocation();

	if (Loc.X - _location.X < TargetDistanceThreshold && Loc.Y - _location.Y < TargetDistanceThreshold)
	{
		//UE_LOG(LogTemp, Error, TEXT("At Target Location")); 
		//bIsCloseToTargetLocation = true;
		return true;
	}

	//bIsCloseToTargetLocation = false;
	//UE_LOG(LogTemp, Error, TEXT("NOT At Target Location!   Target = %f,%f    Me = %f,%f"), TargetLoc.X, TargetLoc.Y, Loc.X, Loc.Y);
	return false;
}

void AEnemyCharacter::CheckIfStuck(FVector CurrentPos, FVector LastPos)
{
	//UE_LOG(LogTemp, Warning, TEXT("New: %s    Old: %s"), *this->GetActorLocation().ToString(), *LastLocation.ToString());

	if (CurrentPos == LastPos)
	{
		if (!StuckTimerSet)
		{
			StuckTimer = GetWorld()->TimeSeconds;
			StuckTimerSet = true;
			//UE_LOG(LogTemp, Warning, TEXT("Stuck timer set"));
		}

		if (GetWorld()->TimeSeconds - StuckTimer > StuckThreshold && StuckTimerSet)
		{
			bSensedTarget = false;
			AIController->SetTargetEnemy(nullptr);
			StuckTimerSet = false;
			if (DebugAIText) { UE_LOG(LogTemp, Error, TEXT("Stuck TIMER REACHED MAX!")); }
		}

		//UE_LOG(LogTemp, Warning, TEXT("Timer: %F"), GetWorld()->TimeSeconds - StuckTimer);
	}

	else
	{
		StuckTimerSet = false;
	}
}


// Called to bind functionality to input
//void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
//{
//	Super::SetupPlayerInputComponent(PlayerInputComponent);
//
//}

