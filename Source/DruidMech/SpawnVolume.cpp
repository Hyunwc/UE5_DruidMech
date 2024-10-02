// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnVolume.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h" // ���� ���� ��ƿ��Ƽ �Լ����� ���Ե� �������

// Sets default values
ASpawnVolume::ASpawnVolume()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawningBox"));
}

// Called when the game starts or when spawned
void ASpawnVolume::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpawnVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector ASpawnVolume::GetSpawnPoint()
{
	FVector Extent = SpawningBox->GetScaledBoxExtent(); // �ڽ��� ũ��
	FVector Origin = SpawningBox->GetComponentLocation(); // �ڽ��� ��ġ

	FVector Point = UKismetMathLibrary::RandomPointInBoundingBox(Origin, Extent);
	return Point;
}

void ASpawnVolume::SpawnOurActor_Implementation(UClass* ToSpawn, const FVector& Location)
{
	if (ToSpawn)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			AActor* Actor = World->SpawnActor<AActor>(ToSpawn, Location, FRotator(0.f));
		}
	}
}
