// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Weapon.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Kismet/KismetMathLibrary.h"
#include "Enemy.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true; //Tick�� �߻��ϵ��� true�� ����

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(this->RootComponent); //GetRootComponent()�� �־��
	CameraBoom->TargetArmLength = 600.0f;
	// Use Pawn Control Rotation -> ī�޶� ȸ���Ҷ� ���� ���� ȸ���� ������?
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	GetCapsuleComponent()->SetCapsuleSize(48.0f, 100.0f);

	/* 3��Ī ĳ���� ȸ���� �� �ؾ��� �۾��� */
	// ���콺 ������ �� ĳ���Ͱ� ȸ���ϴ� ���� �����ϱ� ����
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	// �����̴� ���⿡ ���� ĳ���Ͱ� ȸ��
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 650.0f;
	GetCharacterMovement()->AirControl = 0.2f;

	BaseTurnRate = 65.0f;
	BaseLookUpRate = 65.0f;

	MaxHealth = 100.0f;
	Health = 65.0f;
	MaxStamina = 150.0f;
	Stamina = 120.0f;
	Coins = 0;

	RunningSpeed = 450.0f;
	SprintingSpeed = 800.0f;

	bShiftKeyDown = false;
	bLMBDown = false;
	bAttacking = false;

	MovementStatus = EMovementStatus::EMS_Normal;
	StaminaStatus = EStaminaStatus::ESS_Normal;

	StaminaDrainRate = 25.0f;
	MinSprintStamina = 50.0f;

	InterpSpeed = 15.f;
	bInterpToEnemy = false;
}

void AMainCharacter::ShowPickupLocations()
{
	for (auto Location : PickupLocations)
	{
		UKismetSystemLibrary::DrawDebugSphere(this, Location, 25.0f, 8,
			FLinearColor::Green, 10.0f, 0.5f);
	}
}

void AMainCharacter::SetInterpToEnemy(bool Interp)
{
	bInterpToEnemy = Interp;
}

FRotator AMainCharacter::GetLookAtRotationYaw(FVector Target)
{
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target);
	FRotator LookAtRotationYaw(0.0f, LookAtRotation.Yaw, 0.0f);
	return LookAtRotationYaw;
}

void AMainCharacter::SetMovementStatus(EMovementStatus Status)
{
	MovementStatus = Status;

	if (MovementStatus == EMovementStatus::EMS_Sprinting)
	{
		// ĳ���� Movement ������Ʈ�� ������ (����ĳ������ �������Ʈ ����)
		GetCharacterMovement()->MaxWalkSpeed = SprintingSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = RunningSpeed;
	}
}

void AMainCharacter::ShiftKeyDown()
{
	bShiftKeyDown = true;
}

void AMainCharacter::ShiftKeyUp()
{
	bShiftKeyDown = false;
}

void AMainCharacter::DecrementHealth(float Amount)
{
	Health -= Amount;
	if (Health <= 0.f)
	{
		Die();
	}
}

void AMainCharacter::Die()
{
}

void AMainCharacter::IncrementCoins(int32 Amount)
{
	Coins += Amount;
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	// ���� ��ü, ���� �÷��̾��� ��ġ, ������, ���� ���������?, ����, ���� ���� ���� �� ������?
	/*UKismetSystemLibrary::DrawDebugSphere(this,
		GetActorLocation() + FVector(0, 0, 75.f),
		25.0f, 12, FLinearColor::Green,
		10.f, 0.5f);*/
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float DeltaStamina = StaminaDrainRate * DeltaTime; // �ʴ� 25 ����

	switch (StaminaStatus)
	{
	case EStaminaStatus::ESS_Normal:
		if (bShiftKeyDown)
		{
			Stamina -= DeltaStamina;
			if (Stamina <= MinSprintStamina)
			{
				SetStaminaStatus(EStaminaStatus::ESS_BelowMinimum);
			}

			SetMovementStatus(EMovementStatus::EMS_Sprinting);
		}
		else
		{
			Stamina += DeltaStamina;
			if (Stamina > MaxStamina)
			{
				Stamina = MaxStamina;
			}

			SetMovementStatus(EMovementStatus::EMS_Normal);
		}
		break;
	case EStaminaStatus::ESS_BelowMinimum:
		if (bShiftKeyDown)
		{
			Stamina -= DeltaStamina;
			if (Stamina <= 0.f)
			{
				SetStaminaStatus(EStaminaStatus::ESS_Exhausted);
				Stamina = 0;
				SetMovementStatus(EMovementStatus::EMS_Normal);
			}
			else
			{
				SetMovementStatus(EMovementStatus::EMS_Sprinting);
			}
		}
		else
		{
			Stamina += DeltaStamina;
			if (Stamina >= MinSprintStamina)
			{
				SetStaminaStatus(EStaminaStatus::ESS_Normal);
			}

			SetMovementStatus(EMovementStatus::EMS_Normal);
		}
		break;
	case EStaminaStatus::ESS_Exhausted:
		if (bShiftKeyDown)
		{
			Stamina = 0.0f;
		}
		else
		{
			SetStaminaStatus(EStaminaStatus::ESS_ExhaustedRecovering);
			Stamina += DeltaStamina;
		}
		SetMovementStatus(EMovementStatus::EMS_Normal);
		break;
	case EStaminaStatus::ESS_ExhaustedRecovering:
		Stamina += DeltaStamina;
		if (Stamina >= MinSprintStamina)
		{
			SetStaminaStatus(EStaminaStatus::ESS_Normal);
		}
		SetMovementStatus(EMovementStatus::EMS_Normal);
		break;
	default:
		;
	}

	if (bInterpToEnemy && CombatTarget)
	{
		FRotator LookAtYaw = GetLookAtRotationYaw(CombatTarget->GetActorLocation());
		FRotator InterpRotation = FMath::RInterpTo(GetActorRotation(), LookAtYaw, DeltaTime, InterpSpeed);

		SetActorRotation(InterpRotation);
	}
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AMainCharacter::MoveRight);

	PlayerInputComponent->BindAxis(TEXT("TurnRate"), this, &AMainCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis(TEXT("LookUpAtRate"), this, &AMainCharacter::LookUpAtRate);

	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AMainCharacter::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AMainCharacter::LookUp);

	// ������ Character�� ������ ����
	// IE_Pressed : ������ ��
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ACharacter::Jump);
	// IE_Released : ������ �ʾ��� ��
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Pressed, this, &AMainCharacter::ShiftKeyDown);
	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Released, this, &AMainCharacter::ShiftKeyUp);

	PlayerInputComponent->BindAction(TEXT("LMB"), IE_Pressed, this, &AMainCharacter::LMBDown);
	PlayerInputComponent->BindAction(TEXT("LMB"), IE_Released, this, &AMainCharacter::LMBUp);

}

void AMainCharacter::MoveForward(float Value)
{
	// ��Ʈ�ѷ��� ��ȿ�ϰ� Value�� 0�� �ƴҶ� && �������� �ƴҶ�
	if (Controller != nullptr && Value != 0.0f && !bAttacking)
	{
		//���� ī�޶��� ������ ���´�(Pitch, Yaw, Roll) 
		const FRotator Rotation = Controller->GetControlRotation();
		//Yaw���� ����
		const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);
		//�չ���
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		AddMovementInput(Direction, Value);
	}
}

void AMainCharacter::MoveRight(float Value)
{
	// ��Ʈ�ѷ��� ��ȿ�ϰ� Value�� 0�� �ƴҶ�
	if (Controller != nullptr && Value != 0.0f && !bAttacking)
	{
		//���� ī�޶��� ������ ���´�(Pitch, Yaw, Roll) 
		const FRotator Rotation = Controller->GetControlRotation();
		//Yaw���� ����
		const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);
		//������
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(Direction, Value);
	}
}

void AMainCharacter::TurnAtRate(float Rate)
{
	if (Controller != nullptr && Rate != 0.0f)
	{
		AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
	}
}

void AMainCharacter::LookUpAtRate(float Rate)
{
	if (Controller != nullptr && Rate != 0.0f)
	{
		AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
	}
}

void AMainCharacter::Turn(float Value)
{
	if (Controller != nullptr && Value != 0.0f)
	{
		AddControllerYawInput(Value);
	}
}


void AMainCharacter::LookUp(float Value)
{
	if (Controller != nullptr && Value != 0.0f)
	{
		AddControllerPitchInput(Value);
	}
}

void AMainCharacter::LMBDown()
{
	bLMBDown = true;

	if (ActiveOverlappingItem)
	{
		AWeapon* Weapon = Cast<AWeapon>(ActiveOverlappingItem);
		if (Weapon)
		{
			Weapon->Equip(this);
			SetActiveOverlappingItem(nullptr);
		}
	}
	else if (EquippedWeapon) // ���⸦ �̹� �������̶�� ������ ����ǰ�
	{
		Attack();
	}
}

void AMainCharacter::LMBUp()
{
	bLMBDown = false;
}

// ���� ����
void AMainCharacter::SetEquippedWeapon(AWeapon* WeaponToSet)
{
	// �̹� ���⸦ �������� �� ��� �ִ� ���� �ı�
	if (EquippedWeapon)
	{
		EquippedWeapon->Destroy();
	}

	EquippedWeapon = WeaponToSet;
}

void AMainCharacter::Attack()
{
	if (bAttacking) return; // �̹� �������̶��?

	bAttacking = true;
	SetInterpToEnemy(true);

	UAnimInstance* AnimInstace = GetMesh()->GetAnimInstance();
	if (AnimInstace && CombatMontage)
	{
		int32 Section = FMath::RandRange(0, 1);
		switch (Section)
		{
		case 0:
			AnimInstace->Montage_Play(CombatMontage, 2.2f); // ��Ÿ�� ���� 2.2���
			AnimInstace->Montage_JumpToSection(FName("Attack_1"), CombatMontage);
			break;
		case 1:
			AnimInstace->Montage_Play(CombatMontage, 1.8f); // ��Ÿ�� ���� 2.2���
			AnimInstace->Montage_JumpToSection(FName("Attack_2"), CombatMontage);
			break;
		default:
			;
		}
	}
}

void AMainCharacter::AttackEnd()
{
	bAttacking = false;
	SetInterpToEnemy(false);

	if (bLMBDown)
	{
		Attack();
	}
}

void AMainCharacter::PlaySwingSound()
{
	if (EquippedWeapon->SwingSound)
	{
		UGameplayStatics::PlaySound2D(this, EquippedWeapon->SwingSound);
	}
}

