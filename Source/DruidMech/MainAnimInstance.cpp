// Fill out your copyright notice in the Description page of Project Settings.


#include "MainAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h" 

void UMainAnimInstance::NativeInitializeAnimation()
{
	// ���� �ı��Ǿ��� ��� �ٽ� �Ҵ�
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
	}
}

void UMainAnimInstance::UpdateAnimationProperties()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
	}
	// ���� ��ȿ�� ���
	if (Pawn)
	{
		FVector Speed = Pawn->GetVelocity();
		FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0.0f);
		MovementSpeed = LateralSpeed.Size();

		// ĳ���Ͱ� �������� �ִ� ��������?
		bIsInAir = Pawn->GetMovementComponent()->IsFalling();
	}
}
