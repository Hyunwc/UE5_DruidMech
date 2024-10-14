// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"

UENUM(BlueprintType)
enum class EMovementStatus : uint8 // 타입을 부호가 없는 1바이트 정수로 쓰고싶다
{
	EMS_Normal UMETA(DisplayName = "Normal"),
	EMS_Sprinting UMETA(DisplayName = "Sprinting"),
	EMS_Dead UMETA(DisplayName = "Dead"),

	EMS_MAX UMETA(DisplayName = "DefaultMax")
};

UCLASS()
class DRUIDMECH_API AMainCharacter : public ACharacter //액터 -> 폰을 상속 받음
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Enums)
	EMovementStatus MovementStatus; // 현재 상태, Current 붙이는게 좋아보임.
	void SetMovementStatus(EMovementStatus Status);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Running)
	float RunningSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Enums)
	float SprintingSpeed;

	bool bShiftKeyDown;

	void ShiftKeyDown();
	void ShiftKeyUp();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class USpringArmComponent* CameraBoom; //전방선언

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

	/**
	/* Player Stats
	*/
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Stats")
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Stats")
	float MaxStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float Stamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	int32 Coins;

	void DecrementHealth(float Amount);
	void Die();

	void IncrementCoins(int32 Amount);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	// 폰 이하는 이 함수가 생긴다 (플레이어를 제어하기 때문)
	// 액터 상속받으면 생기지 않는다
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/* 플레이어의 앞/뒤 움직임 */
	void MoveForward(float Value);
	/* 플레이어의 좌/우 움직임 */
	void MoveRight(float Value);


	/* 회전 */
	void TurnAtRate(float Rate);

	void LookUpAtRate(float Rate);

	void Turn(float Value);
	void LookUp(float Value);
};
