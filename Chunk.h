// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
//#include "NoiseParameters.h"
#include "Environment.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Chunk.generated.h"

struct FNoiseParameters;
class UFastNoiseWrapper;
class UProceduralMeshComponent;
class UMaterialInterface;


UCLASS()
class WARGAME_API AChunk : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChunk();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	int Size;
	float VertexDistance = 100.0f;
	float UVScale;
	float ZMultiplier;
	UMaterialInterface* Material;
	TArray<FNoiseParameters> NoiseParameters;
	TArray<FEnvironmentLayer> EnvironmentLayers;
	int32 EnvironmentSeed;
	float XOffset;
	float YOffset;
	void Generate();
	void GenerateEnvironment();


private:
	
	float Scale;
	
	
	float NoiseScale;
	
	UProceduralMeshComponent* ProceduralMesh;
    TArray<FVector> Vertices;
	TArray<int32> Triangles;
    TArray<FVector2D> UV0;
	TArray<FVector> Normals;
	TArray <struct FProcMeshTangent> Tangents;
	
	TArray<UFastNoiseWrapper*> Noises;
	
	TArray<UHierarchicalInstancedStaticMeshComponent *> EnvironmentComponents;
	
	void CreateVertices();
	void CreateVertices1();
	void CreateTriangles();
	float CalculateZ(float X, float Y);
	FVector GetNormalAt(float X, float Y);
	FVector2D CalculateCoordinates(float Distance , int XIndex, int YIndex);
};
