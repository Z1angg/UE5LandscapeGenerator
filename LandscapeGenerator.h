// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChunkPosition.h"
#include "NoiseParameters.h"
#include "Environment.h"
#include "LandscapeGenerator.generated.h"

struct FNoiseParameters;
struct FChunkPosition;
class AChunk;

UCLASS()
class WARGAME_API ALandscapeGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALandscapeGenerator();
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Scale = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32  Size = 10;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 RenderDistance = 10;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float UVScale = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ZMultiplier = 1000;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInterface* Material;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FNoiseParameters> NoiseParameters;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment")
	TArray<FEnvironmentLayer> EnvironmentLayers;

	// 全局种子
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 EnvironmentSeed = 12345;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	
private:
	int32 X;
	int32 Y;
	AChunk* Chunk;
	FChunkPosition CurrentPosition;
	FChunkPosition VisibleChunk;
	TSet<FChunkPosition> GeneratedChunks;
	

};
