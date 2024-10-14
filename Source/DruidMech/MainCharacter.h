// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"

UENUM(BlueprintType)
enum class EMovementStatus : uint8 // Ÿ���� ��ȣ�� ���� 1����Ʈ ������ ����ʹ�
{
	EMS_Normal UMETA(DisplayName = "Normal"),
	EMS_Sprinting UMETA(DisplayName = "Sprinting"),
	EMS_Dead UMETA(DisplayName = "Dead"),

	EMS_MAX UMETA(DisplayName = "DefaultMax")
};

UCLASS()
class DRUIDMECH_API AMainCharacter : public ACharacter //���� -> ���� ��� ����
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Enums)
	EMovementStatus MovementStatus; // ���� ����, Current ���̴°� ���ƺ���.
	void SetMovementStatus(EMovementStatus Status);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Running)
	float RunningSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Enums)
	float SprintingSpeed;

	bool bShiftKeyDown;

	void ShiftKeyDown();
	void ShiftKeyUp();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	class USpringArmComponent* CameraBoom; //���漱��

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
	// �� ���ϴ� �� �Լ��� ����� (�÷��̾ �����ϱ� ����)
	// ���� ��ӹ����� ������ �ʴ´�
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/* �÷��̾��� ��/�� ������ */
	void MoveForward(float Value);
	/* �÷��̾��� ��/�� ������ */
	void MoveRight(float Value);


	/* ȸ�� */
	void TurnAtRate(float Rate);

	void LookUpAtRate(float Rate);

	void Turn(float Value);
	void LookUp(float Value);
};
