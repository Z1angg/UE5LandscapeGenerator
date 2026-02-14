// Fill out your copyright notice in the Description page of Project Settings.


#include "LandscapeGenerator.h"
#include "Chunk.h"
#include "GameFramework/PlayerController.h"
#include "ChunkPosition.h"
#include "GameFramework/GameSession.h"
#include "Kismet/GameplayStatics.h"

//class APC_Demo_Playercntroller : ;
struct FChunkPosition;

// Sets default values
ALandscapeGenerator::ALandscapeGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Size = 10;
	Scale = 100.0f;
	RenderDistance = 10;

}

// Called when the game starts or when spawned
void ALandscapeGenerator::BeginPlay()
{
	Super::BeginPlay();
	GeneratedChunks.Empty();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, "Hello");
	
}

// Called every frame
void ALandscapeGenerator::Tick(float DeltaTime)
{
	
	Super::Tick(DeltaTime);
	/*
	AActor* FoundActor =UGameplayStatics::GetActorOfClass(GetWorld(), APlayerController::StaticClass());
	APlayerController* MyPC = Cast<APlayerController>(FoundActor);
	
	if (MyPC)
	{
		// 3. 逻辑完全一样，因为 PlayerCameraManager 在基类里就有
		APlayerCameraManager* CameraManager = MyPC->PlayerCameraManager;
		if (CameraManager)
		{
			FVector CameraLocation = CameraManager->GetCameraLocation();
			// 使用 Loc.X, Loc.Y, Loc.Z
			
		}
	}
	*/
	APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);

	if (CameraManager)
	{
		FVector CameraLocation = CameraManager->GetCameraLocation();
		
		CurrentPosition.X =	FMath::TruncToInt32(CameraLocation.X/Scale);
		CurrentPosition.Y =	FMath::TruncToInt32(CameraLocation.Y/Scale);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("X: %d, Y: %d"), CurrentPosition.X, CurrentPosition.Y));
	}
	//APlayerCameraManager* CameraManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
	//FVector CameraLocation = CameraManager->GetCameraLocation();
	//FVector CameraLocation = MyPC->PlayerCameraManager->GetCameraLocation();
	
	
	for (int i = -RenderDistance;i <= RenderDistance; ++i)
	{
		X = i;
		for (int j = -RenderDistance;j <= RenderDistance; ++j)
		{ 
			Y = j;
			VisibleChunk.X = CurrentPosition.X + X;
			VisibleChunk.Y = CurrentPosition.Y + Y;
			if (!GeneratedChunks.Contains(VisibleChunk))
			{
				Chunk = GetWorld()->SpawnActor<AChunk>(FVector(VisibleChunk.X*Scale,VisibleChunk.Y*Scale,0.0f), FRotator(0,0,0));
				Chunk->Size = Size;
				Chunk->VertexDistance = Scale/Size;
				Chunk->UVScale = UVScale;
				Chunk->ZMultiplier = ZMultiplier;
				Chunk->Material = Material;
				Chunk->NoiseParameters = NoiseParameters;
				Chunk->XOffset = Scale*VisibleChunk.X;
				Chunk->YOffset = Scale*VisibleChunk.Y;
				Chunk->EnvironmentLayers = EnvironmentLayers;
				Chunk->EnvironmentSeed = EnvironmentSeed;
				Chunk->Generate();
				
				GeneratedChunks.Add(VisibleChunk);
				
				
			}
			Chunk->GenerateEnvironment();
		}
	}
	
	
	

}

