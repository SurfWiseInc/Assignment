// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AssignmentGameMode.generated.h"

//DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(FPlayerDiedSignature, AActor, OnBeginCursorOver, AActor*, TouchedActor);


UENUM()
enum class EGamePlayState
{
	EMenu,
	EPlaying,
	EGameOver,
	EPaused,
	EUnknown
};


UCLASS(minimalapi)
class AAssignmentGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AAssignmentGameMode();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	class AAssignmentCharacter * MyCharacter;

	UPROPERTY(EditAnywhere, Category = "Health")
		TSubclassOf<class UUserWidget> HUDWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Health")
		class UUserWidget* CurrentWidget;

	bool bIsGamePaused;

	UFUNCTION(BlueprintPure, Category = "Health")
		EGamePlayState GetCurrentState() const;

	void SetCurrentState(EGamePlayState NewState);

	UFUNCTION()
		void HandlePlayerDeath();

	UFUNCTION()
		void HandlePauseGame();

private:
	EGamePlayState CurrentState;

	void HandleNewState(EGamePlayState NewState);

};



