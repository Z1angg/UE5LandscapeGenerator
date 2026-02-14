// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChunkPosition.generated.h"

/**
 * 
 */
class WARGAME_API ChunkPosition
{
public:
	ChunkPosition();
	~ChunkPosition();
};

/** Please add a struct description */
USTRUCT(BlueprintType)
struct FChunkPosition
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 X;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Y;

	// 修复 1：比较 X 和 Y，而不是 ID
	bool operator==(const FChunkPosition& Other) const
	{
		return X == Other.X && Y == Other.Y;
	}

	// 修复 2：哈希值必须结合 X 和 Y
	friend uint32 GetTypeHash(const FChunkPosition& Thing)
	{
		return HashCombine(GetTypeHash(Thing.X), GetTypeHash(Thing.Y));
	}
public:
	
};
