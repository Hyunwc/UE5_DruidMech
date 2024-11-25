// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

// UMG

/**
 * 
 */
UCLASS()
class DRUIDMECH_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Widgets")
	TSubclassOf<UUserWidget> HUDOverlayAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UUserWidget* HUDOverlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<UUserWidget> EnemyHealthBarAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	UUserWidget* EnemyHealthBar;

	bool bEnemyHealthBarVisible;

	void DisplayEnemyHealthBar();
	void RemoveEnemyHealthBar();

	FVector EnemyLocation;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
