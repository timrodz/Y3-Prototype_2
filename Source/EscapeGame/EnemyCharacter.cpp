// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyCharacter.h"
#include "EnemyAIController.h"
#include "FirstPersonCharacterController.h"
#include "Zone.h"
#include "Perception/PawnSensingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
AEnemyCharacter::AEnemyCharacter(const class FObjectInitializer& ObjectInitializer)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

	StuckTimer = 0.0f;
	TimeSinceLastSeen = 0.0f;
}

bool AEnemyCharacter::HasSensedTarget()
{
	if (AIController)
		return AIController->CurrentMode == EEnemyAIMode::SENSED;
	else
		return false;
}

void AEnemyCharacter::SetSensedTargetTrue()
{
	CharMovement->MaxWalkSpeed = WalkSpeedSensedTarget;

	AIController->CurrentMode = EEnemyAIMode::SENSED;
}

bool AEnemyCharacter::IsEnemyPatrolling()
{
	return AIController->CurrentMode == EEnemyAIMode::WANDER;
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
	//this->OnCollisionHit.AddDynamic(this, &AInteractableObject::OnCreateNoise);

	DebugTextRender = this->FindComponentByClass<UTextRenderComponent>();
	AIController = Cast<AEnemyAIController>(GetController());
	AIController->SetShouldWander(true);
	AIController->SetWaypoint(nullptr);
	LastLocation = this->GetActorLocation();
	StuckTimer = GetWorld()->TimeSeconds;
	CharMovement = this->FindComponentByClass<UCharacterMovementComponent>();

	CharMovement->MaxWalkSpeed = WalkSpeedDefault;
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float WorldTime = GetWorld()->TimeSeconds;


	if (WorldTime - StuckTimer > StuckThreshold)
	{
		CheckIfStuck();
	}
	
	// Check for events
	if (AIController->IsEventActive() && AIController->IsTargetLocationSet())
	{
		//bIsPatrolling = false;

		DebugTextRender->SetText(FText::FromString("Event"));

		if (IsCloseToLocation(AIController->GetEventLocation()))
		{
			//AIController->SetEventActive(false);			
		}

		return;	
	}

	FVector TargetLoc = AIController->GetTheTargetLocation();
	bIsCloseToTargetLocation = IsCloseToLocation(TargetLoc);


	// Check if enemy has seen or heard anything
	if (AIController->CurrentMode == EEnemyAIMode::SENSED)
	{
		TimeSinceLastSeen = WorldTime - LastSeenTime;

		//bIsPatrolling = false;

		//if (bIsCloseToTargetLocation && !bTargetTimerSet)
		//{
		//	TimeArrivedAtTarget = WorldTime;
		//	bTargetTimerSet = true;
		//	if (DebugAIText) { UE_LOG(LogTemp, Warning, TEXT("At Target Timer Started")); }
		//}

		//else if (!bIsCloseToTargetLocation)
		//{
		//	bTargetTimerSet = false;
		//	TimeArrivedAtTarget = 0.0f;
		//}

		if (TimeSinceLastSeen >= UnseenChaseTime)
		{
			//bSensedTarget = false;
			CharMovement->MaxWalkSpeed = WalkSpeedDefault;
			AIController->SetTargetLocation(AIController->GetTargetEnemy()->GetActorLocation());
			AIController->SetTargetEnemy(nullptr);

			AIController->CurrentMode = EEnemyAIMode::LAST_KNOWN_LOCATION;
		}
	}

	////Check time spent near target location, abort if longer than TimeToWait
	//if (bIsCloseToTargetLocation)
	//{
	//	float Time = WorldTime - TimeArrivedAtTarget;

	//	if (WorldTime - TimeArrivedAtTarget > TimeToWaitAtTargetLocation)
	//	{
	//		if (DebugAIText) { UE_LOG(LogTemp, Warning, TEXT("At Target Timer Finished")); }

	//		AIController = Cast<AEnemyAIController>(GetController());
	//		if (AIController)
	//		{
	//			bSensedTarget = false;
	//			CharMovement->MaxWalkSpeed = WalkSpeedDefault;
	//			if (DebugAIText) { UE_LOG(LogTemp, Warning, TEXT("Sensed target false")); }

	//			/* Reset */
	//			AIController->SetTargetEnemy(nullptr);

	//			if (DebugAIText) { UE_LOG(LogTemp, Warning, TEXT("Rest Target To Null")); }
	//		}
	//	}
	//}


	//// Patrolling
	//if (bPatrolPointsSet && !bSensedTarget)
	//{
	//	bIsPatrolling = true;
	//	DebugTextRender->SetText(FText::FromString("Patrolling"));
	//	AIController = Cast<AEnemyAIController>(GetController());

	//	if (DebugAIText)
	//	{
	//		AIController->DrawDebugLineToTarget();
	//	}	
	//}

	//else if (!bPatrolPointsSet && !bSensedTarget)
	//{

	//}

	LastLocation = this->GetActorLocation();
}

void AEnemyCharacter::OnSeePlayer(APawn * Pawn)
{

	//if (!bSensedTarget)
	//{
	//	//BroadcastUpdateAudioLoop(true);
	//}

	AIController = Cast<AEnemyAIController>(GetController());

	if (AIController->IsEventActive())
	{
		return;
	}


	AFirstPersonCharacterController* SensedPawn = Cast<AFirstPersonCharacterController>(Pawn);
	if (SensedPawn)
	{
		if (AIController)// && SensedPawn->IsAlive())
		{
			AIController->SetTargetLocation(SensedPawn->GetActorLocation());
			AIController->SetTargetEnemy(SensedPawn);

			SetSensedTargetTrue();

			/* Keep track of the time the player was last sensed in order to clear the target */
			LastSeenTime = GetWorld()->GetTimeSeconds();

		}
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

	SetSensedTargetTrue();

	AIController = Cast<AEnemyAIController>(GetController());

	if (AIController->IsEventActive())
	{
		return;
	}

	if (DebugAIText) { UE_LOG(LogTemp, Warning, TEXT("HEARD Noise")); }

	float DistanceToNoise = FVector::Dist(this->GetActorLocation(), PawnInstigator->GetActorLocation());
	//UE_LOG(LogTemp, Warning, TEXT("Distance to noise: %f"), DistanceToNoise);

	LastHeardTime = GetWorld()->GetTimeSeconds();

	

	//UE_LOG(LogTemp, Warning, TEXT("Sensed target set to true"));

	if (AIController)
	{
		AIController->SetTargetLocation(PawnInstigator->GetActorLocation());
		AIController->SetTargetEnemy(PawnInstigator);
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

void AEnemyCharacter::CheckIfStuck()
{
	//UE_LOG(LogTemp, Warning, TEXT("New: %s    Old: %s"), *this->GetActorLocation().ToString(), *LastLocation.ToString());
	AIController = Cast<AEnemyAIController>(GetController());

	StuckTimer = GetWorld()->TimeSeconds;

	if (this->GetActorLocation() == LastLocation)
	{
		CharMovement->MaxWalkSpeed = WalkSpeedDefault;

		AIController->CurrentMode = EEnemyAIMode::WANDER;

		AIController->SetTargetEnemy(nullptr);
		if (DebugAIText) { UE_LOG(LogTemp, Error, TEXT("STUCK!!!")); }

		if (!AIController->IsEventActive())
		{
			AIController->SetWaypoint(nullptr);
			//AIController->FindWaypoint();
		}
		else // Event
		{
			AIController->SetEventActive(false);
		}
	}

	LastLocation = this->GetActorLocation();
}

void AEnemyCharacter::MoveToLocation(FVector newLocation)
{
	this->SetActorLocation(newLocation);
}

void AEnemyCharacter::CheckForActiveZoneEvents()
{
	if ((CurrentZone) && CurrentZone->GetHasItemToCheck())
	{
		SetZoneEventActive();
	}
	else
	{
		SetZoneEventComplete();
	}
}

void AEnemyCharacter::SetZoneEventActive()
{
	AIController = Cast<AEnemyAIController>(GetController());
	AIController->SetEventActive(true);
	AIController->SetEventLocation(CurrentZone->GetItemLocation());
	UE_LOG(LogTemp, Warning, TEXT("Event Active in Current Zone"));

}

void AEnemyCharacter::SetZoneEventComplete()
{
	AIController = Cast<AEnemyAIController>(GetController());
	AIController->SetEventActive(false);

	if ((CurrentZone) && CurrentZone->GetHasItemToCheck())
	{
		CurrentZone->SetHasItemToCheck(false);

		if (CurrentZone->GetCurrentTarget())
		{
			CurrentZone->SetCurrentTargetToInactive();
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Zone Event Complete"));
}
// Called to bind functionality to input
//void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
//{
//	Super::SetupPlayerInputComponent(PlayerInputComponent);
//
//}

