// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Environment.generated.h"

/**
 * 
 */
class WARGAME_API Environment
{
public:
	Environment();
	~Environment();
};

USTRUCT(BlueprintType)
struct FEnvironmentLayer
{
    GENERATED_BODY()

public:
    // 植被模型
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UStaticMesh* Mesh;

    // 每次生成时的尝试次数（密度）
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Density = 100;

    // 最小缩放
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MinScale = 0.8f;

    // 最大缩放
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxScale = 1.2f;
    
    // 允许生成的最小高度 (比如 -1000 以下是水，不长树)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MinHeight = -10000.0f;

    // 允许生成的最大高度
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxHeight = 10000.0f;
    
    // 唯一标识种子的偏移量，防止草和树长在完全同一个点
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 SeedOffset = 0;
};
