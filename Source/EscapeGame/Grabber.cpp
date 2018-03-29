// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"


#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandleComponent();
	SetupInputComponent();
}

/// Look for attached Physics Handle
void UGrabber::FindPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing physics handle component"), *GetOwner()->GetName())
	}
}

/// Look for attached Input Component (only appears at run time)
void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		InputComponent->BindAction("Interact", IE_Pressed, this, &UGrabber::Grab);
		//InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing input component"), *GetOwner()->GetName())
	}
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PhysicsHandle) { return; }
	// if the physics handle is attached
	if (PhysicsHandle->GrabbedComponent)
	{
		// move the object that we're holding
		PhysicsHandle->SetTargetLocation(GetReachLineEnd());
	}
}

void UGrabber::Grab() {

	//if (!PhysicsHandle) { // OLD
	if (PhysicsHandle->GrabbedComponent != nullptr) { // <- NEW
													  //UE_LOG(LogTemp, Warning, TEXT("But there was no Physics Handle"))
		Release();
		return;
	}
	/// LINE TRACE and see if we reach any actors with physics body collision channel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent(); // gets the mesh in our case
	auto ActorHit = HitResult.GetActor();

	/// If we hit something then attach a physics handle
	if (ActorHit)
	{
		//UE_LOG(LogTemp, Warning, TEXT("An actor was hit by the Grabber::GetFirstPhysicsBodyInReach()"))

	

		//UE_LOG(LogTemp, Warning, TEXT("And the component has been grabbed"))
		PhysicsHandle->GrabComponent(
			ComponentToGrab,
			NAME_None, // no bones needed
			ComponentToGrab->GetComponentLocation(),
			true // allow rotation
		);

		OnGrabComponent(PhysicsHandle->GrabbedComponent);
	}
}

void UGrabber::Release()
{
	//UE_LOG(LogTemp, Warning, TEXT("Release has been called"))

	if (!PhysicsHandle) { 
		//UE_LOG(LogTemp, Warning, TEXT("but there was no Physics Handle"))
			return;
	}

	//UE_LOG(LogTemp, Warning, TEXT("and the component has been released"))

	OnReleaseComponent(PhysicsHandle->GrabbedComponent);
	PhysicsHandle->ReleaseComponent();
}

void UGrabber::Throw(FVector direction)
{
	if (!PhysicsHandle)
	{
		return;
	}

	UPrimitiveComponent* comp = PhysicsHandle->GrabbedComponent;

	if (comp == nullptr)
	{
		return;
	}

	comp->AddForce(direction * ThrowStrength, NAME_None, true);
	OnReleaseComponent(PhysicsHandle->GrabbedComponent);
	PhysicsHandle->ReleaseComponent();
	
	//comp->AddForce(direction * ThrowStrength);
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	/// Line-trace (AKA ray-cast) out to reach distance
	FHitResult HitResult;
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(
		OUT HitResult,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);
	return HitResult;
}

FVector UGrabber::GetReachLineStart()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);
	return PlayerViewPointLocation;
}

FVector UGrabber::GetReachLineEnd()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);
	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
}

void UGrabber::OnReleaseComponent_Implementation(UPrimitiveComponent* Component)
{

}

void UGrabber::OnGrabComponent_Implementation(UPrimitiveComponent* Component)
{

}