// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Classes/Components/InstancedStaticMeshComponent.h"
#include "Classes/Components/BoxComponent.h"
#include "Engine/StaticMeshActor.h"
#include "RoomTool.generated.h"


UENUM(BlueprintType)
enum class EAxisDirection : uint8
{
	YAxis,
	XAxis
};

UENUM(BlueprintType)
enum class EMeshDisplacement : uint8
{
	PositiveDisplacement UMETA(DisplayName = "PositiveDisplacement"),
	NegativeDisplacement UMETA(DisplayName = "NegativeDisplacement") ,
	Centered UMETA(DisplayName = "Centered")
};

USTRUCT(BlueprintType)
struct FMeshInfo
{
	GENERATED_USTRUCT_BODY()

	FMeshInfo() : Mesh(nullptr), MeshInstance(nullptr)
	{

	}
	UInstancedStaticMeshComponent* MeshInstance;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "MeshInfo")
		UStaticMesh* Mesh;
	UPROPERTY(BlueprintReadOnly, Category = "MeshInfo")
		EMeshDisplacement MeshDisplacement;
	UPROPERTY(BlueprintReadOnly, Category = "MeshInfo")
		EAxisDirection Axis;
	UPROPERTY(BlueprintReadOnly, Category = "MeshInfo")
		FVector WidthVector;
	UPROPERTY(BlueprintReadOnly, Category = "MeshInfo")
		FVector LengthVector;
	UPROPERTY(BlueprintReadOnly, Category = "MeshInfo")
		FVector HeightVector;
};


USTRUCT(BlueprintType)
struct FRoomExtention
{
	GENERATED_USTRUCT_BODY()

	FRoomExtention() : WallInfo(), DoorInfo()
	{

	}

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "MeshInfo")
		FMeshInfo WallInfo;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "MeshInfo")
		FMeshInfo DoorInfo;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "MeshInfo")
		int32 PlacementIndex;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "MeshInfo")
		int32 Width;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "MeshInfo")
		int32 Length;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "MeshInfo")
		int32 AccessWidth;
};

UCLASS(Blueprintable)
class ESCAPEGAME_API ARoomTool : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARoomTool();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	USceneComponent* Root;
	UBoxComponent* BoxCollision;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Instances")
		UInstancedStaticMeshComponent* DoorInstances;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Instances")
		UInstancedStaticMeshComponent* WallInstances;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Instances", meta = (EditCondition = bUseCornerPieces))
		UInstancedStaticMeshComponent* CornerPieceInstances;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Instances", meta = (EditCondition = bUseCornerPieces))
		bool bUseCornerPieceToHideSeams;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Instances")
		bool bUseCornerPieces;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Instances")
		UInstancedStaticMeshComponent* ExtentionDoorInstances;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Instances")
		UInstancedStaticMeshComponent* ExtentionWallInstances;

	
	UFUNCTION(BlueprintCallable, Category = "Generation")
		FMeshInfo GenerateMeshInfo(UStaticMesh* mesh);

	UFUNCTION(BlueprintCallable, Category = "Generation")
	FMeshInfo GetWallInfo() const;

	UFUNCTION(BlueprintCallable, Category = "Generation")
		void CreateBoundingBox();

	UFUNCTION(BlueprintCallable, Category = "Generation")
		FVector GetMainRoomBoxExtents();

	UFUNCTION(BlueprintCallable, Category = "Generation")
		void AddWall();

	UFUNCTION(BlueprintCallable, Category = "Generation")
		void AddDoors();

	UFUNCTION(BlueprintCallable, Category = "Generation")
		void AddExtentions();

	UFUNCTION(BlueprintCallable, Category = "Generation")
		void AddCornerPieces();

	UFUNCTION(BlueprintCallable, Category = "Generation")
		void ConvertToStaticMeshActors();

	virtual void PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent);

	void PostEdit_DoorLocation();

public:

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Decorators")
		AActor* DoorBlueprint;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Settings|Extentions")
		TArray<FRoomExtention> RoomExtentions;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Settings|Meshes")
		FMeshInfo DoorwayInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Meshes")
		FMeshInfo WallInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Meshes")
		FMeshInfo CornerPieceInfo;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Settings|Size", meta = (ClampMin = 0))
		int Width;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Settings|Size", meta = (ClampMin = 0))
		int Length;


	//UPROPERTY(EditDefaultsOnly, Category = "Settings|Options")
	//	bool bCreateDoors;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Settings|Decorators")
		TArray<int32> DoorLocations;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Settings|Decorators")
		TArray<AStaticMeshActor*> ConvertedInstances;

	//UPROPERTY(EditDefaultsOnly, Category = "Settings|Size", meta = (ClampMin = 1))
	//	int Stories;

	//UPROPERTY(EditDefaultsOnly, Category = "Settings|Options")
	//	bool bCreateExtention;

	///* Add an extention to the generated room
	//* *Enabled in options "CreateExtention"
	//*/
	//UPROPERTY(EditDefaultsOnly, Category = "Settings|Size", meta = (ClampMin = 0), meta = (EditCondition = bCreateExtention))
	//	int ExtentionWidth;
	///* Add an extention to the generated room
	//* *Enabled in options "CreateExtention"
	//*/
	//UPROPERTY(EditDefaultsOnly, Category = "Settings|Size", meta = (ClampMin = 0), meta = (EditCondition = bCreateExtention))
	//	int ExtentionLength;

	//

private:

	
	
};
