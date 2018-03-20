// Fill out your copyright notice in the Description page of Project Settings.

#include "FirstPersonCharacterController.h"


// Sets default values
AFirstPersonCharacterController::AFirstPersonCharacterController()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFirstPersonCharacterController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFirstPersonCharacterController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Delta = DeltaTime;

}

// Called to bind functionality to input
void AFirstPersonCharacterController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

