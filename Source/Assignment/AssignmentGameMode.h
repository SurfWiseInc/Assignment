// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AssignmentGameMode.generated.h"

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

	UFUNCTION(BlueprintPure, Category = "Health")
		EGamePlayState GetCurrentState() const;

	void SetCurrentState(EGamePlayState NewState);

private:
	EGamePlayState CurrentState;

	void HandleNewState(EGamePlayState NewState);

};



