// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true; //Tick이 발생하도록 true로 설정

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(this->RootComponent); //GetRootComponent()로 넣어도됨
	CameraBoom->TargetArmLength = 600.0f;
	// Use Pawn Control Rotation -> 카메라가 회전할때 폰이 같이 회전할 것인지?
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	GetCapsuleComponent()->SetCapsuleSize(48.0f, 100.0f);

	/* 3인칭 캐릭터 회전할 때 해야할 작업들 */
	// 마우스 돌렸을 때 캐릭터가 회전하는 것을 방지하기 위해
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	// 움직이는 방향에 따라 캐릭터가 회전
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 650.0f;
	GetCharacterMovement()->AirControl = 0.2f;

	BaseTurnRate = 65.0f;
	BaseLookUpRate = 65.0f;
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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

	// 점프는 Character가 가지고 있음
	// IE_Pressed : 눌렀을 때
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ACharacter::Jump);
	// IE_Released : 누르지 않았을 때
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Released, this, &ACharacter::StopJumping);

}

void AMainCharacter::MoveForward(float Value)
{
	// 컨트롤러가 유효하고 Value가 0이 아닐때
	if (Controller != nullptr && Value != 0.0f)
	{
		//현재 카메라의 방향을 얻어온다(Pitch, Yaw, Roll) 
		const FRotator Rotation = Controller->GetControlRotation();
		//Yaw방향 설정
		const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);
		//앞방향
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		AddMovementInput(Direction, Value);
	}
}

void AMainCharacter::MoveRight(float Value)
{
	// 컨트롤러가 유효하고 Value가 0이 아닐때
	if (Controller != nullptr && Value != 0.0f)
	{
		//현재 카메라의 방향을 얻어온다(Pitch, Yaw, Roll) 
		const FRotator Rotation = Controller->GetControlRotation();
		//Yaw방향 설정
		const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);
		//옆방향
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

