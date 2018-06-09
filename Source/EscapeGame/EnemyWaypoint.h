#pragma once

#include "Engine/TargetPoint.h"
#include "EnemyWaypoint.generated.h"

/**
* Waypoint helper for bots to generate waypoints during patrols. Object is placed in level to specify a potential patrol target location.
*/
UCLASS()
class ESCAPEGAME_API AEnemyWaypoint : public ATargetPoint
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
		bool RandomEventNavigationOnly;
};