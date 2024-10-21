// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "MainCharacter.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AWeapon::AWeapon()
{
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(GetRootComponent());

	bWeaponParticles = false;
	WeaponState = EWeaponState::EWS_Pickup;
}

void AWeapon::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if ((WeaponState == EWeaponState::EWS_Pickup) && OtherActor)
	{
		AMainCharacter* MainCharacter = Cast<AMainCharacter>(OtherActor);
		// 메인캐릭터가 맞다면 무기장착
		if (MainCharacter)
		{
			MainCharacter->SetActiveOverlappingItem(this);
		}
	}
}

void AWeapon::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

	if (OtherActor)
	{
		AMainCharacter* MainCharacter = Cast<AMainCharacter>(OtherActor);
		// 메인캐릭터가 맞다면 무기장착
		if (MainCharacter)
		{
			MainCharacter->SetActiveOverlappingItem(nullptr);
		}
	}
}

void AWeapon::Equip(AMainCharacter* Char)
{
	if (Char)
	{
		SkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera,
			ECollisionResponse::ECR_Ignore);
		SkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,
			ECollisionResponse::ECR_Ignore);

		SkeletalMesh->SetSimulatePhysics(false);

		const USkeletalMeshSocket* RightHandSocket =
			Char->GetMesh()->GetSocketByName(TEXT("RightHandSocket"));

		if (RightHandSocket)
		{
			RightHandSocket->AttachActor(this, Char->GetMesh());

			bRotate = false;

			Char->SetEquippedWeapon(this);
			SetWeaponState(EWeaponState::EWS_Equipped);

			if (OnEquipSound)
				UGameplayStatics::PlaySound2D(this, OnEquipSound);

			if (!bWeaponParticles)
			{
				IdleParticlesComponent->Deactivate();
			}
		}
	}
}


