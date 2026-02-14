// Fill out your copyright notice in the Description page of Project Settings.


#include "Chunk.h"
#include "ProceduralMeshComponent.h"
#include "KismetProceduralMeshLibrary.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "NoiseParameters.h"

//UFastNoiseWrapper* fastNoiseWrapper = nullptr;
//class EFractalType;
class UHierarchicalInstancedStaticMeshComponent;

// Sets default values
AChunk::AChunk()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>("ProceduralMesh");
	ProceduralMesh->SetupAttachment(GetRootComponent());

}

// Called when the game starts or when spawned
void AChunk::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AChunk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AChunk::CreateVertices()
{
	Vertices.Empty();
	Triangles.Empty();
	Normals.Empty();
	UV0.Empty();
	Tangents.Empty();
	
	
	for (int X = 0; X <= Size; ++ X)
	{
		for (int Y = 0; Y <= Size; ++Y)
		{
			
			float NoiseValuesSum = 0.0f;
			for (int32 i = 0; i < NoiseParameters.Num(); ++i)
			{
				// 1. 对应蓝图中的第一个 GET (从 NoiseParameters 取结构体)
				//const FNoiseParameters& Params = NoiseParameters[i];

				// 2. 对应蓝图中的第二个 GET (根据同样的 Index 从 Noises 取 Target 对象)
				// 必须检查数组越界和指针是否为空，这是 C++ 的安全常识
				if (Noises.IsValidIndex(i) && Noises[i] != nullptr)
				{
					//TObjectPtr<UCurveFloat> HeightCurve = NoiseParameters[i].HeightCurve;
				}
				float Time = Noises[i]->GetNoise2D(X+0.1f+XOffset , Y+0.1f+YOffset);
				NoiseValuesSum += NoiseParameters[i].HeightCurve->GetFloatValue(Time);
			}
			
			
			
			
			
			float Z = NoiseValuesSum*ZMultiplier;
			Vertices.Add(FVector(CalculateCoordinates(VertexDistance,X,Y),Z ));
			UV0.Add(FVector2D(CalculateCoordinates(UVScale,X,Y)));
			
			//DrawDebugSphere(GetWorld(), FVector(X * Scale, Y * Scale, 0), 50, 5, FColor::Red, false,50);
		}
	}
}

float AChunk::CalculateZ(float X, float Y)
{
	float NoiseValuesSum = 0.0f;

	// 遍历所有噪声层
	for (int32 i = 0; i < NoiseParameters.Num(); ++i)
	{
		if (Noises.IsValidIndex(i) && Noises[i] != nullptr)
		{
			// 注意：这里不需要再乘 VertexDistance，因为传入的 X, Y 已经是计算好的坐标了
			float Time = Noises[i]->GetNoise2D(X, Y);

			if (NoiseParameters[i].HeightCurve)
			{
				NoiseValuesSum += NoiseParameters[i].HeightCurve->GetFloatValue(Time);
			}
			else
			{
				NoiseValuesSum += Time;
			}
		}
	}
	return NoiseValuesSum * ZMultiplier;
}







void AChunk::CreateTriangles()
{
	
	int VertexIndex = 0;
	for (int i = 0; i < Size; ++i)
	{
		for (int j = 0; j < Size; ++j)
		{
			Triangles.Add(VertexIndex);
			Triangles.Add(VertexIndex+1);
			Triangles.Add(VertexIndex +Size+ 1);
			Triangles.Add(VertexIndex+1);
			Triangles.Add(VertexIndex +Size+ 2);
			Triangles.Add(VertexIndex +Size+ 1);
			++VertexIndex;
		}
		++VertexIndex;
		
	}
	
}

void AChunk::Generate()
{
	if (Noises.Num() != NoiseParameters.Num())
	{
		Noises.Empty();
		for (int32 i = 0; i < NoiseParameters.Num(); ++i)
		{
			// 必须创建对象实例！
			UFastNoiseWrapper* NewNoise = NewObject<UFastNoiseWrapper>(this);
			Noises.Add(NewNoise);
		}
	}
	
	for (int32 NoiseIndex = 0; NoiseIndex < NoiseParameters.Num(); ++NoiseIndex)
	{
		// 1. 对应蓝图中的第一个 GET (从 NoiseParameters 取结构体)
		const FNoiseParameters& Parameters = NoiseParameters[NoiseIndex];

		// 2. 对应蓝图中的第二个 GET (根据同样的 Index 从 Noises 取 Target 对象)
		// 必须检查数组越界和指针是否为空，这是 C++ 的安全常识
		if (Noises.IsValidIndex(NoiseIndex) && Noises[NoiseIndex] != nullptr)
		{
			UFastNoiseWrapper* TargetNoise = Noises[NoiseIndex];

			// 3. 对应 Setup Fast Noise 节点
			// 直接通过指针调用该对象的函数，并将结构体成员作为参数传入
			TargetNoise->SetupFastNoise(
				Parameters.NoiseType,
				Parameters.Seed,
				Parameters.Frequency,
				Parameters.Interp,
				Parameters.FractalType,
				Parameters.Octaves,
				Parameters.Lacunarity,
				Parameters.Gain,
				Parameters.CellularJitter,
				Parameters.CellularDistanceFunction,
				Parameters.CellularReturnType
			);
		}
		
	}
	CreateVertices1();
	CreateTriangles();
	//UKismetProceduralMeshLibrary::CalculateTangentsForMesh(Vertices, Triangles, UV0, Normals, Tangents);
	ProceduralMesh->CreateMeshSection(0, Vertices, Triangles, Normals, UV0, TArray<FColor>(), Tangents, true);
	ProceduralMesh->SetMaterial(0, Material);
	//GenerateEnvironment();
}

FVector2D AChunk::CalculateCoordinates(float Distance , int XIndex, int YIndex)
{
	float X = (Size/-2.0f + XIndex) * Distance;
	float Y = (Size/-2.0f + YIndex) * Distance;
	return FVector2D(X,Y);
}


void AChunk::CreateVertices1()
{ 
	// 1. 清空旧数据
    Vertices.Empty();
    Triangles.Empty();
    Normals.Empty();
    UV0.Empty();
    Tangents.Empty();

    // 采样步长：用于计算坡度的微小距离，0.1f 通常精度足够且效果好
    float Epsilon = 0.1f; 

    // 2. 循环生成点 (注意是 <= Size，因为 10 个格子需要 11 个点)
    for (int X = 0; X <= Size; ++X)
    {
        for (int Y = 0; Y <= Size; ++Y)
        {
            
            
            // 计算用于噪声采样的“绝对坐标” (包含 Offset)
            float InputX = X * VertexDistance + XOffset;
            float InputY = Y * VertexDistance + YOffset;
        	

            // 获取当前点的确切高度
            float CenterZ = CalculateZ(InputX, InputY);
            
            // 获取当前点在 Chunk 内部的相对位置 (居中逻辑)
            FVector2D LocalPos = CalculateCoordinates(VertexDistance, X, Y);
            
            // 添加顶点
            Vertices.Add(FVector(LocalPos.X, LocalPos.Y, CenterZ));
            
            // 添加 UV
            UV0.Add(FVector2D(CalculateCoordinates(UVScale, X, Y)));


            // --- C. 手动计算法线 (Normal) ---
            // 解决光照接缝的核心：我们不依赖网格面，而是直接问噪声函数“这里的坡度是多少？”

            // 采样周围 4 个微小偏移点的高度
            float HeightLeft  = CalculateZ(InputX - Epsilon, InputY);
            float HeightRight = CalculateZ(InputX + Epsilon, InputY);
            float HeightDown  = CalculateZ(InputX, InputY - Epsilon);
            float HeightUp    = CalculateZ(InputX, InputY + Epsilon);

            // 计算切线向量
            // X方向切线：(距离差, 0, 高度差)
            FVector TangentX = FVector(2.0f * Epsilon, 0.0f, HeightRight - HeightLeft);
            // Y方向切线：(0, 距离差, 高度差)
            FVector TangentY = FVector(0.0f, 2.0f * Epsilon, HeightUp - HeightDown);

            // 归一化
            TangentX.Normalize();
            TangentY.Normalize();

            // 计算法线：切线的叉积 (Cross Product) 垂直于表面
            FVector NormalVector = FVector::CrossProduct(TangentX, TangentY); 
            NormalVector.Normalize();

            // 添加法线和切线数据
            Normals.Add(NormalVector);
            Tangents.Add(FProcMeshTangent(TangentX, true));
        }
    }
}

FVector AChunk::GetNormalAt(float GlobalX, float GlobalY)
{
	// 采样周围很小距离的三个点来计算斜率，距离越小越精确
	float Epsilon = 10.0f; // 10cm 的采样间距
    
	float H_Center = CalculateZ(GlobalX, GlobalY);
	float H_Right  = CalculateZ(GlobalX + Epsilon, GlobalY);
	float H_Forward = CalculateZ(GlobalX, GlobalY + Epsilon);

	// 构建两个切线向量
	FVector TangentX(Epsilon, 0.0f, H_Right - H_Center);
	FVector TangentY(0.0f, Epsilon, H_Forward - H_Center);

	// 叉乘得到法线
	FVector Normal = FVector::CrossProduct(TangentY, TangentX);
	return Normal.GetSafeNormal();
}


void AChunk::GenerateEnvironment()
{
    // 1. 清理旧的植被组件 (如果是刷新 Chunk)
    for (auto Comp : EnvironmentComponents)
    {
        if (Comp) Comp->DestroyComponent();
    }
    EnvironmentComponents.Empty();

    // 2. 遍历每一层植被配置 (树、草、花...)
    for (const FEnvironmentLayer& Layer : EnvironmentLayers)
    {
        if (!Layer.Mesh) continue;

        // 创建 HISM 组件 (性能极高)
        UHierarchicalInstancedStaticMeshComponent* HISM = NewObject<UHierarchicalInstancedStaticMeshComponent>(this);
        HISM->RegisterComponent();
        HISM->SetStaticMesh(Layer.Mesh);
        HISM->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
        
        // 设置剔除距离 (优化性能)
        HISM->SetCullDistances(10000, 20000); // 示例：100米开始淡出，200米完全消失

        EnvironmentComponents.Add(HISM);

        // --- 核心：确定性随机流 ---
        // 使用 Chunk 的坐标 (XOffset, YOffset) 和 WorldSeed 混合生成唯一的种子
        // 这样无论哪个客户端，只要坐标相同，算出来的随机数就相同
        int32 ChunkUniqueSeed = HashCombine(GetTypeHash(XOffset), GetTypeHash(YOffset));
        ChunkUniqueSeed = HashCombine(ChunkUniqueSeed, EnvironmentSeed);
        ChunkUniqueSeed = HashCombine(ChunkUniqueSeed, Layer.SeedOffset); // 区分不同植被层

        FRandomStream Stream(ChunkUniqueSeed);

        // 开始尝试生成实例
        for (int i = 0; i < Layer.Density; i++)
        {
            // 在 Chunk 范围内随机取点 (0 到 Size * VertexDistance)
            // 注意：这里是在 Chunk 局部空间随机
            float RandomX = Stream.FRandRange(0.0f, Size * VertexDistance);
            float RandomY = Stream.FRandRange(0.0f, Size * VertexDistance);

            // 换算成全局坐标用于计算高度 (CalculateZ 需要全局坐标)
            float GlobalX = RandomX + XOffset;
            float GlobalY = RandomY + YOffset;

            // 获取高度 (直接复用你的噪声函数，无需射线检测)
            float Z = CalculateZ(GlobalX, GlobalY);

            // 高度过滤 (例如：不要生成在水下)
            if (Z < Layer.MinHeight || Z > Layer.MaxHeight) continue;

            // 随机缩放
            float EnvScale = Stream.FRandRange(Layer.MinScale, Layer.MaxScale);
            
            // 随机旋转 (只绕 Z 轴旋转)
            FRotator Rotation = FRotator(0.0f, Stream.FRandRange(0.0f, 360.0f), 0.0f);
            
            // 组合 Transform (Z 稍微往下一点点避免悬空)
            FTransform InstanceTransform;
            InstanceTransform.SetLocation(FVector(RandomX, RandomY, Z - 20.0f)); 
            InstanceTransform.SetRotation(Rotation.Quaternion());
            InstanceTransform.SetScale3D(FVector(EnvScale));

            // 添加实例
            HISM->AddInstance(InstanceTransform, true);
        }
    }
}