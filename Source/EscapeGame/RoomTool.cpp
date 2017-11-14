// Fill out your copyright notice in the Description page of Project Settings.

#include "RoomTool.h"
#include "Engine/StaticMesh.h"

// Sets default values
ARoomTool::ARoomTool()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(FName("RootComponent"));
	SetRootComponent(Root);

	//Initialise the InstanceStaticMeshComponent and attach to root
	WallInstances = CreateDefaultSubobject<UInstancedStaticMeshComponent>(FName("WallInstance"));
	WallInstances->AttachTo(GetRootComponent());

	CornerPieceInstances = CreateDefaultSubobject<UInstancedStaticMeshComponent>(FName("CornerPieceInstance"));
	CornerPieceInstances->AttachTo(GetRootComponent());

	DoorInstances = CreateDefaultSubobject<UInstancedStaticMeshComponent>(FName("DoorInstances"));
	DoorInstances->AttachTo(GetRootComponent());

	ExtentionWallInstances = CreateDefaultSubobject<UInstancedStaticMeshComponent>(FName("ExtentionWallInstances"));
	ExtentionWallInstances->AttachTo(GetRootComponent());

	ExtentionDoorInstances = CreateDefaultSubobject<UInstancedStaticMeshComponent>(FName("ExtentionDoorInstances"));
	ExtentionDoorInstances->AttachTo(GetRootComponent());


}

// Called when the game starts or when spawned
void ARoomTool::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARoomTool::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


FMeshInfo ARoomTool::GenerateMeshInfo(UStaticMesh* Mesh)
{
	if (Mesh == nullptr)
	{
		return FMeshInfo();
	}

	Mesh->CalculateExtendedBounds();
	FBox MeshBox = Mesh->GetBoundingBox();
	FVector Negative = FVector(MeshBox.Min.X, MeshBox.Min.Y, 0);
	FVector Positive = FVector(MeshBox.Max.X, MeshBox.Max.Y, 0);

	float NegativeSize = Negative.Size();
	float PositiveSize = Positive.Size();

	//UE_LOG(LogTemp, Warning, TEXT("PositiveBounds : %s "), *MeshBox.Min.ToString())
	//UE_LOG(LogTemp, Warning, TEXT("NegativeBounds : %s "), *MeshBox.Max.ToString())

	float X = 0;
	float Y = 0; 
	EMeshDisplacement displacement;
	float LengthOfLongestSide = 0.0f;

	//Comment : Working Correctly, Calculates whethere the objects longest axis points positively from its position or negatively or is equal and thus centered.
	if (FMath::IsNearlyEqual(PositiveSize, NegativeSize, 0.01f))
	{
		displacement = EMeshDisplacement::Centered;
		UE_LOG(LogTemp, Warning, TEXT("Object pivot is centered"))

		X = fabs(MeshBox.Max.X);
		Y = fabs(MeshBox.Max.Y);

	}
	else if (PositiveSize > NegativeSize)
	{
		displacement = EMeshDisplacement::PositiveDisplacement;
		UE_LOG(LogTemp, Warning, TEXT("Object pivot is Positive"))
		X = fabs(MeshBox.Max.X);
		Y = fabs(MeshBox.Max.Y);
		
	}
	else
	{
		displacement = EMeshDisplacement::NegativeDisplacement;
		UE_LOG(LogTemp, Warning, TEXT("Object pivot is Negative"))

		X = fabs(MeshBox.Min.X);
		Y = fabs(MeshBox.Min.Y);
	}

	EAxisDirection DirectionType;

	FVector Direction;
	if (X > Y)
	{
		Direction = FVector(MeshBox.Max.X, 0, 0);
		DirectionType = EAxisDirection::XAxis;
		LengthOfLongestSide = fabs(MeshBox.Max.X) + fabs(MeshBox.Min.X);

	}
	else
	{
		Direction = FVector(0, MeshBox.Max.Y, 0);
		DirectionType = EAxisDirection::YAxis;
		LengthOfLongestSide = fabs(MeshBox.Max.Y) + fabs(MeshBox.Min.Y);
	}

	FVector LengthVector;
	FVector WidthVector;

	if (DirectionType == EAxisDirection::XAxis)
	{
		WidthVector = Direction;
		LengthVector = Direction.RotateAngleAxis(90.0f, FVector(0, 0, 1.0f));


		UE_LOG(LogTemp, Warning, TEXT("Direction of Mesh is along XAxis"))
	}

	else if (DirectionType == EAxisDirection::YAxis)
	{
		LengthVector = Direction;
		WidthVector = Direction.RotateAngleAxis(90.0f, FVector(0, 0, 1.0f));

		UE_LOG(LogTemp, Warning, TEXT("Direction of Mesh is along YAxis"))
	}


	FMeshInfo MeshInfo;
	MeshInfo.Mesh = Mesh;
	MeshInfo.Axis = DirectionType;
	MeshInfo.MeshDisplacement = displacement;
	MeshInfo.WidthVector = WidthVector;
	MeshInfo.LengthVector = LengthVector;
	MeshInfo.HeightVector = FVector(0, 0, fabs(MeshBox.Min.Z) + fabs(MeshBox.Max.Z));

	return MeshInfo;
}

FMeshInfo ARoomTool::GetWallInfo() const
{
	return WallInfo;
}

void ARoomTool::CreateBoundingBox()
{

}

FVector ARoomTool::GetMainRoomBoxExtents()
{
	FVector width = WallInfo.WidthVector * Width;
	FVector length = WallInfo.LengthVector * Length;
	FVector height = WallInfo.HeightVector;

	return (width + length + height) / 2.0f;
}

void ARoomTool::AddWall()
{
	WallInstances->ClearInstances();
	WallInstances->SetStaticMesh(WallInfo.Mesh);
	WallInstances->SetWorldLocation(GetRootComponent()->GetRelativeTransform().GetLocation());

	for (int i = 0; i < Width; i++)
	{
		FTransform transform = FTransform();
		transform.SetLocation(WallInfo.WidthVector * i);
		WallInstances->AddInstance(transform);
	}



	for (int i = 0; i < Length; i++)
	{
		FTransform transform = FTransform();
		FRotator rotation = FRotator(0.0f, 90.0f, 0.0f);
		transform.SetRotation(rotation.Quaternion());
		transform.SetLocation(WallInfo.LengthVector * i + WallInfo.WidthVector * Width);
		WallInstances->AddInstance(transform);
	}

	for (int i = Width - 1; i >= 0; i--)
	{
		FTransform transform = FTransform();
		transform.SetLocation(WallInfo.LengthVector * Length + WallInfo.WidthVector * i);
		WallInstances->AddInstance(transform);
	}

	for (int i = Length - 1; i >= 0; i--)
	{
		FTransform transform = FTransform();
		FRotator rotation = FRotator(0.0f, 90.0f, 0.0f);
		transform.SetRotation(rotation.Quaternion());
		transform.SetLocation(WallInfo.LengthVector * i);
		WallInstances->AddInstance(transform);
	}


}

void ARoomTool::AddCornerPieces()
{
	CornerPieceInstances->ClearInstances();
	CornerPieceInstances->SetStaticMesh(CornerPieceInfo.Mesh);
	CornerPieceInstances->SetWorldLocation(GetRootComponent()->GetRelativeTransform().GetLocation());

	//FTransform transform;
	//int index = 0;
	//do
	//{
	//	transform.SetLocation(transform.GetLocation() + CornerPieceInfo.WidthVector / 2.0f);
	//	transform.SetRotation(transform.GetRotation());
	//	CornerPieceInstances->AddInstance(transform);
	//	index++;
	//} while (WallInstances->GetInstanceTransform(index, transform, true));


	for (int i = 0; i < Width; i++)
	{
		FTransform transform = FTransform();
		transform.SetLocation(WallInfo.WidthVector * i);
		CornerPieceInstances->AddInstance(transform);
	}



	for (int i = 0; i < Length; i++)
	{
		FTransform transform = FTransform();
		FRotator rotation = FRotator(0.0f, 90.0f, 0.0f);
		transform.SetRotation(rotation.Quaternion());
		transform.SetLocation(WallInfo.LengthVector * i + WallInfo.WidthVector * Width);
		CornerPieceInstances->AddInstance(transform);
	}

	for (int i = Width - 1; i >= 0; i--)
	{
		FTransform transform = FTransform();
		transform.SetLocation(WallInfo.LengthVector * Length + WallInfo.WidthVector * i);
		CornerPieceInstances->AddInstance(transform);
	}

	for (int i = Length - 1; i >= 0; i--)
	{
		FTransform transform = FTransform();
		FRotator rotation = FRotator(0.0f, 90.0f, 0.0f);
		transform.SetRotation(rotation.Quaternion());
		transform.SetLocation(WallInfo.LengthVector * i);
		CornerPieceInstances->AddInstance(transform);
	}

}

void ARoomTool::AddDoors()
{
	//UE_LOG(LogTemp, Warning, TEXT("%d instances"), WallInstances->GetInstanceCount());

	//for (int i = 0; i < WallInstances->GetInstanceCount(); i++)
	//{
	//	FTransform transform;
	//	WallInstances->GetInstanceTransform(i, transform);
	//	UE_LOG(LogTemp, Warning, TEXT("%d instance transform : %s"), i, *transform.ToString())
	//}

	DoorInstances->ClearInstances();
	DoorInstances->SetStaticMesh(DoorwayInfo.Mesh);
	DoorInstances->SetWorldLocation(GetRootComponent()->GetRelativeTransform().GetLocation());

	for (int i = 0; i < DoorLocations.Num(); i++)
	{
		int ind = DoorLocations[i];

		FTransform transform;
		if (WallInstances->GetInstanceTransform(ind, transform))
		{
			WallInstances->RemoveInstance(ind);
			DoorInstances->AddInstance(transform);
		}
	}
	
}

void ARoomTool::AddExtentions()
{
	//for (int i = 0; i < RoomExtentions.Num(); i++)
	//{

	//	FTransform S1;
	//	if (WallInstances->GetInstanceTransform(RoomExtentions[i].PlacementIndex, S1))
	//	{
	//		if (WallInstances->RemoveInstance(RoomExtentions[i].PlacementIndex))
	//		{

	//		}
	//	}
	//}

	ExtentionWallInstances->ClearInstances();
	ExtentionWallInstances->SetStaticMesh(WallInfo.Mesh);
	ExtentionWallInstances->SetWorldLocation(GetRootComponent()->GetRelativeTransform().GetLocation());


	for (int i = 0; i < RoomExtentions.Num(); i++)
	{
		int width = RoomExtentions[i].Width;
		int length = RoomExtentions[i].Length;

		FTransform startTransform;
		if (!WallInstances->GetInstanceTransform(RoomExtentions[i].PlacementIndex, startTransform))
		{
			return;
		}

		if (!WallInstances->RemoveInstance(RoomExtentions[i].PlacementIndex))
		{
			return;
		}

		FRotator rotator = startTransform.Rotator();
		rotator = FRotator(0, rotator.Yaw + 90.0f, 0);
		startTransform.SetRotation(rotator.Quaternion());
		ExtentionWallInstances->SetWorldLocation(GetRootComponent()->GetRelativeTransform().GetLocation() + startTransform.GetLocation());


		for (int i = 0; i < width; i++)
		{
			FTransform transform = FTransform();
			transform.AddToTranslation(WallInfo.WidthVector * i);
			ExtentionWallInstances->AddInstance(transform);
		}



		for (int i = 0; i < length; i++)
		{
			FTransform transform = FTransform();
			FRotator rotation = FRotator(0.0f, 90.0f, 0.0f);
			transform.SetRotation(rotation.Quaternion());
			transform.SetLocation(WallInfo.LengthVector * i + WallInfo.WidthVector * width);
			ExtentionWallInstances->AddInstance(transform);
		}

		for (int i = width - 1; i >= 0; i--)
		{
			FTransform transform = FTransform();
			transform.SetLocation(WallInfo.LengthVector * length + WallInfo.WidthVector * i);
			ExtentionWallInstances->AddInstance(transform);
		}

		for (int i = length - 1; i >= 0; i--)
		{
			FTransform transform = FTransform();
			FRotator rotation = FRotator(0.0f, 90.0f, 0.0f);
			transform.SetRotation(rotation.Quaternion());
			transform.SetLocation(WallInfo.LengthVector * i);
			ExtentionWallInstances->AddInstance(transform);
		}
	}
}

void ARoomTool::ConvertToStaticMeshActors()
{
	for (int i = 0; i < ConvertedInstances.Num(); i++)
	{

		ConvertedInstances[i]->Destroy();
	}

	ConvertedInstances.Empty();

	for (int i = 0; i < WallInstances->GetInstanceCount(); i++)
	{
		FTransform transform;
		if (WallInstances->GetInstanceTransform(i, transform))
		{
			transform.SetLocation(transform.GetLocation());
		}
	}

}

#if WITH_EDITOR

void ARoomTool::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	//Get the name of the property that was changed  
	FName PropertyName = (PropertyChangedEvent.Property != nullptr) ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	// We test using GET_MEMBER_NAME_CHECKED so that if someone changes the property name  
	// in the future this will fail to compile and we can update it.  
	if ((PropertyName == GET_MEMBER_NAME_CHECKED(ARoomTool, DoorLocations)))
	{
		UE_LOG(LogTemp, Warning, TEXT("PostEdit_DoorLocations()"));
		PostEdit_DoorLocation();
	}

	// Call the base class version  
	Super::PostEditChangeProperty(PropertyChangedEvent);
}

#endif




void ARoomTool::PostEdit_DoorLocation()
{

	WallInfo = GenerateMeshInfo(WallInfo.Mesh);
	DoorwayInfo = GenerateMeshInfo(DoorwayInfo.Mesh);
	AddWall();
	AddExtentions();
	AddDoors();

	return;

	//TArray<FTransform> DoorTransforms;
	//if (DoorInstances->GetInstanceCount() > 0)
	//{
	//	for (int i = 0; i < DoorInstances->GetInstanceCount(); i++)
	//	{
	//		FTransform instanceTransform;
	//		if (DoorInstances->GetInstanceTransform(i, instanceTransform))
	//		{
	//			DoorTransforms.Add(instanceTransform);
	//		}
	//	}
	//}

	//TArray<FTransform> WallTransforms;
	//if (WallInstances->GetInstanceCount() > 0)
	//{
	//	for (int i = 0; i < WallInstances->GetInstanceCount(); i++)
	//	{
	//		FTransform instanceTransform;
	//		if (WallInstances->GetInstanceTransform(i, instanceTransform))
	//		{
	//			WallTransforms.Add(instanceTransform);
	//		}
	//	}
	//}

	//WallInstances->ClearInstances();
	//WallInstances->SetStaticMesh(DoorwayInfo.Mesh);
	//WallInstances->SetWorldLocation(GetRootComponent()->GetRelativeTransform().GetLocation());

	//for (int i = 0; i < WallTransforms.Num(); i++)
	//{

	//}

	//DoorInstances->ClearInstances();
	//DoorInstances->SetStaticMesh(DoorwayInfo.Mesh);
	//DoorInstances->SetWorldLocation(GetRootComponent()->GetRelativeTransform().GetLocation());

	//for (int i = 0; i < DoorLocations.Num(); i++)
	//{
	//	int ind = DoorLocations[i];

	//	FTransform transform;
	//	if (WallInstances->GetInstanceTransform(ind, transform))
	//	{
	//		WallInstances->RemoveInstance(ind);
	//		DoorInstances->AddInstance(transform);
	//	}
	//}
}