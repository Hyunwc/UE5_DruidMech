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

	// 카테고리에 공백이 포함된다면 따옴표 "" 사용할 것.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Floor Switch")
	class UBoxComponent* TriggerBox; // 박스 컴포넌트는 헤더가 없기때문에 전방선언

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Floor Switch")
	UStaticMeshComponent* Switch;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Floor Switch")
	UStaticMeshComponent* Door;

	// 블루프린트에서만 수정가능
	// 문과 스위치의 초기 위치를 담을 변수
	UPROPERTY(BlueprintReadWrite, Category = "Floor Switch")
	FVector InitialDoorLocation;

	UPROPERTY(BlueprintReadWrite, Category = "Floor Switch")
	FVector InitialSwitchLocation;

	FTimerHandle SwitchHandle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floor Switch")
	float SwitchTime;

	bool bCharacterOnSwitch;

	void CloseDoor();

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

	// BlueprintImplementableEvent : 이 함수는 블루프린트에서 구현하겠다.
	UFUNCTION(BlueprintImplementableEvent, Category = "Floor Switch")
	void RaiseDoor();

	UFUNCTION(BlueprintImplementableEvent, Category = "Floor Switch")
	void LowerDoor();

	UFUNCTION(BlueprintImplementableEvent, Category = "Floor Switch")
	void RaiseSwitch();

	UFUNCTION(BlueprintImplementableEvent, Category = "Floor Switch")
	void LowerSwitch();

	// BlueprintCallable : 블루프린트에서 C++함수를 호출하겠다
	UFUNCTION(BlueprintCallable, Category = "Floor Switch")
	void UpdateDoorLocation(float Z);

	UFUNCTION(BlueprintCallable, Category = "Floor Switch")
	void UpdateSwitchLocation(float Z);
};
