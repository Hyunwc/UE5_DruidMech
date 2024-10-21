// Fill out your copyright notice in the Description page of Project Settings.


#include "Explosive.h"
#include "MainCharacter.h"

AExplosive::AExplosive()
{
	Damage = 15.0f;
}

void AExplosive::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	

	//UE_LOG(LogTemp, Warning, TEXT("Explosive::OnOverlapBegin()"));

	if (OtherActor)
	{
		AMainCharacter* MainCharacter = Cast<AMainCharacter>(OtherActor);
		if (MainCharacter)
		{
			Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

			MainCharacter->DecrementHealth(Damage);

			Destroy();
		}
	}
}

void AExplosive::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

	//UE_LOG(LogTemp, Warning, TEXT("Explosive::OnOverlapEnd()"));
}
