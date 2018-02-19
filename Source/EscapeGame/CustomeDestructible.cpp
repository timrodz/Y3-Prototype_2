// Fill out your copyright notice in the Description page of Project Settings.

#include "CustomeDestructible.h"
#include "Runtime/Engine/Classes/Components/DestructibleComponent.h"
//#include "../Plugins/Runtime/ApexDestruction/Source/ApexDestruction/Public/DestructibleComponent.h"

// Sets default values
ACustomeDestructible::ACustomeDestructible()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ACustomeDestructible::CustomHit()
{
	if (Broken)
		return;

	UE_LOG(LogTemp, Warning, TEXT("CustomHit"))


		Broken = true;

	UDestructibleComponent* Destructible = GetDestructibleComponent();

	TArray<FName> SocketNames = Destructible->GetAllSocketNames();

	for (int i = 0; i < SocketNames.Num(); i++)
	{
		OriginTransforms.Push(Destructible->GetSocketTransform(SocketNames[i]));
	}

	UWorld* World = GetWorld();
	if (CustomHitDelay.IsValid() == false)
	{
		World->GetTimerManager().SetTimer(CustomHitDelay, this, &ACustomeDestructible::BeginReconstruction, 3.0f);
	}
	else
	{
		if (World->GetTimerManager().IsTimerPaused(CustomHitDelay) == true)
		{
			World->GetTimerManager().UnPauseTimer(CustomHitDelay);
		}
		else
		{
			World->GetTimerManager().PauseTimer(CustomHitDelay);
		}
	}

	Destructible->ApplyRadiusDamage(100, Destructible->GetComponentLocation(), 100, 1000, true);
}


void ACustomeDestructible::BeginReconstruction()
{

	UE_LOG(LogTemp, Warning, TEXT("BeginReconstruction ~~ Called "))
		//Allow the tick function to begin reversing the deconstruction
		Reconstructable = true;
	UDestructibleComponent* Destructible = GetDestructibleComponent();
	//Disable gravity to allow forces to act freely
	GravityWasEnabled = Destructible->IsGravityEnabled();
	Destructible->SetEnableGravity(false);
	Destructible->SetSimulatePhysics(false);

	TArray<FName> SocketNames = Destructible->GetAllSocketNames();

	for (int i = 0; i < SocketNames.Num(); i++)
	{
		ReconstructTransforms.Push(Destructible->GetSocketTransform(SocketNames[i]));
	}
}


void ACustomeDestructible::TickReconstruction(float deltaTime)
{
	if (!Broken || !Reconstructable || bNeedsRespawning)
		return;

	TimeSinceHit += deltaTime;



	//Add delta forces to simulate reconstruction of mesh.
	if (TimeSinceHit <= AnimationLength)
	{

		if (FMath::IsNearlyEqual(FMath::Fmod(TimeSinceHit, 1.0f), 0.0f, 0.01f))
		{
			VaryingTolerance -= 20.0f;
			VaryingTolerance = FMath::Clamp(VaryingTolerance, 0.0f, 100.0f);

			if (!AlmostFinished)
			{
				if (TimeSinceHit >= 2.0f)
				{
					AlmostFinished = true;
					VaryingTolerance = 10.0f;
				}
			}
		}
		else
		{
			UDestructibleComponent* Destructible = GetDestructibleComponent();
			TArray<FName> Names = Destructible->GetAllSocketNames();

			for (int i = 0; i < Names.Num(); i++)
			{
				int32 boneInd = Destructible->GetBoneIndex(Names[i]);
				int32 chunkInd = Destructible->BoneIdxToChunkIdx(boneInd);

				//The translation we want;
				FQuat ORotation = OriginTransforms[i].GetRotation();
				FVector OPosition = OriginTransforms[i].GetLocation();

				//The current translations
				FTransform CurrentTransform = Destructible->GetSocketTransform(Names[i]);

				FQuat CRotation = ReconstructTransforms[i].GetRotation();
				FVector CPosition = ReconstructTransforms[i].GetLocation();

				//FQuat CRotation = Destructible->GetBoneQuaternion(Names[i]);
				//FVector CPosition = Destructible->GetBoneLocation(Names[i]);

				float TimeScale = TimeSinceHit / AnimationLength;

				//TimeScale *= 1.5f;
				TimeScale = FMath::Clamp(TimeScale, 0.0f, 1.01f);

				//The final translation lerp
				FQuat ChunkRotation = FMath::Lerp(CRotation, ORotation, TimeScale);
				FVector ChunkPosition = FMath::Lerp(CPosition, OPosition, TimeScale);

				Destructible->SetChunkWorldRT(chunkInd, ChunkRotation, ChunkPosition);
			}
		}
	}

	//Destroy actor, and spawn new BP of same class to finish "Reconstruction".
	else
	{
		//UWorld* World = GetWorld();
		//FActorSpawnParameters SpawnParams;
		//UDestructibleComponent* Destructible = GetDestructibleComponent();
		////Spawn a new CustomDestructible to finish the reconstruction.
		//ACustomeDestructible* Spawned = World->SpawnActor<ACustomeDestructible>(ACustomeDestructible::StaticClass(), GetActorTransform(), SpawnParams);
		//Spawned->GetDestructibleComponent()->SetEnableGravity(GravityWasEnabled);


		bNeedsRespawning = true;
		UE_LOG(LogTemp, Warning, TEXT("NeedsRespawning"))

	}
}


// Called when the game starts or when spawned
void ACustomeDestructible::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ACustomeDestructible::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TickReconstruction(DeltaTime);
}


void ACustomeDestructible::DelayedDestroy()
{
	UE_LOG(LogTemp, Warning, TEXT("DelayedDestroy"))

		Destroy();
}








