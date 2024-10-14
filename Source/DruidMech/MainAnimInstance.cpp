// Fill out your copyright notice in the Description page of Project Settings.


#include "MainAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h" 
#include "MainCharacter.h"

void UMainAnimInstance::NativeInitializeAnimation() // BeginPlay()와 같음
{
	// 폰이 파괴되었을 경우 다시 할당
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
		if (Pawn)
		{
			MainCharacter = Cast<AMainCharacter>(Pawn);
		}
	}
}

void UMainAnimInstance::UpdateAnimationProperties()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
		if (Pawn)
		{
			MainCharacter = Cast<AMainCharacter>(Pawn);
		}
	}
	// 폰이 유효할 경우
	if (Pawn)
	{
		FVector Speed = Pawn->GetVelocity();
		FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0.0f);
		MovementSpeed = LateralSpeed.Size();

		// 캐릭터가 떨어지고 있는 상태인지?
		bIsInAir = Pawn->GetMovementComponent()->IsFalling();

		if (MainCharacter == nullptr)
		{
			MainCharacter = Cast<AMainCharacter>(Pawn);
		}
	}
}
