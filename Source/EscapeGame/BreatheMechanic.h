// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BreatheMechanic.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPEGAME_API UBreatheMechanic : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBreatheMechanic();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintReadWrite, Category = "Variables")
		float CurrentAirIntake = 0.0f;

	UPROPERTY(BlueprintReadWrite, Category = "Variables")
		float MaxAirThreshold = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
		float BreathRechargeMultiplier = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
		float BreathDecreaseMultiplier = 35.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
		float BreathResetDelay = 5.0f;

	UPROPERTY(BlueprintReadWrite, Category = "Variables")
		bool IsHoldingBreath = false;

	UPROPERTY(BlueprintReadWrite, Category = "Variables")
		bool CanHoldBreath = true;
	
	//NOT NEEDED, CAN'T CALL FROM OUTSIDE BP FOR SOME REASON?
	//UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Hold Breath")
	//	void ToggleBreathState();

	//UFUNCTION(BlueprintImplementableEvent, Category = "Hold Breath")
	//	void ResetBreathState();
};
