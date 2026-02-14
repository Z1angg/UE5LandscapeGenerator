// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FastNoiseWrapper.h"
#include "NoiseParameters.generated.h"

/**
 * 
 */
class WARGAME_API NoiseParameters
{
public:
	NoiseParameters();
	~NoiseParameters();
};

/** Please add a struct description */
USTRUCT(BlueprintType)
struct FNoiseParameters
{
	GENERATED_BODY()
public:
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="HeightCurve", MakeStructureDefaultValue="CurveFloat'/Game/Curves/Linear.Linear'"))
	TObjectPtr<UCurveFloat> HeightCurve;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="NoiseType", MakeStructureDefaultValue="Simplex"))
	EFastNoise_NoiseType NoiseType;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="Seed", MakeStructureDefaultValue="1337"))
	int32 Seed;

	/** 地形粗糙度 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="Frequency", MakeStructureDefaultValue="0.010000"))
	double Frequency;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="Interp", MakeStructureDefaultValue="Quintic"))
	EFastNoise_Interp Interp;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="Fractaltype", MakeStructureDefaultValue="FBM"))
	EFastNoise_FractalType FractalType;

	/** 噪声叠加层数 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="Octaves", MakeStructureDefaultValue="3"))
	int32 Octaves;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="Lacunarity", MakeStructureDefaultValue="2.000000"))
	double Lacunarity;

	/** 每层噪声和上一层的相对强度 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="Gain", MakeStructureDefaultValue="0.500000"))
	double Gain;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="CellularJitter", MakeStructureDefaultValue="0.450000"))
	double CellularJitter;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="CellularDistanceFunction", MakeStructureDefaultValue="Euclidean"))
	EFastNoise_CellularDistanceFunction CellularDistanceFunction;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(DisplayName="CellularReturnType", MakeStructureDefaultValue="CellValue"))
	EFastNoise_CellularReturnType CellularReturnType;
};
