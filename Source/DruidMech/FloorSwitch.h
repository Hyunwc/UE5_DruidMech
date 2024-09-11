// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloorSwitch.generated.h"

UCLASS()
class DRUIDMECH_API AFloorSwitch : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloorSwitch();

	// ī�װ��� ������ ���Եȴٸ� ����ǥ "" ����� ��.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Floor Switch")
	class UBoxComponent* TriggerBox; // �ڽ� ������Ʈ�� ����� ���⶧���� ���漱��

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Floor Switch")
	UStaticMeshComponent* Switch;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Floor Switch")
	UStaticMeshComponent* Door;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION()
	void OnOverlapEnd(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

};
