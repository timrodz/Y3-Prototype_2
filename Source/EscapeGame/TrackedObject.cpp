// Fill out your copyright notice in the Description page of Project Settings.

#include "TrackedObject.h"

ATrackedObject::ATrackedObject()
{
	ActiveState = false;
}

void ATrackedObject::BeginPlay()
{

}

void ATrackedObject::Tick(float DeltaTime)
{

}

void ATrackedObject::SetActiveState(bool newState)
{
	ActiveState = newState;
}

bool ATrackedObject::GetActiveState()
{
	return ActiveState;
}
