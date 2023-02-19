// Copyright Epic Games, Inc. All Rights Reserved.

#include "AssignmentGameMode.h"
#include "AssignmentCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/HUD.h"
#include "Blueprint/UserWidget.h"


AAssignmentGameMode::AAssignmentGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> GameplayWidget(TEXT("/Game/UI/WBP_GameplayHUD"));
	HUDWidgetClass = GameplayWidget.Class;

	// use our custom HUD class
	HUDClass = AHUD::StaticClass();


}

void AAssignmentGameMode::BeginPlay()
{
	Super::BeginPlay();

	SetCurrentState(EGamePlayState::EPlaying);

	MyCharacter = Cast<AAssignmentCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));

	//Bind to OnPlayerDied
	MyCharacter->OnPlayerDied.AddDynamic(this, &AAssignmentGameMode::HandlePlayerDeath);
	MyCharacter->OnPauseGame.AddDynamic(this, &AAssignmentGameMode::HandlePauseGame);

	// Add Gameplay UI to viewport
	if (HUDWidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);
	
		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport();
		}
	}

}

void AAssignmentGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
EGamePlayState AAssignmentGameMode::GetCurrentState() const
{
	return CurrentState;
}

void AAssignmentGameMode::SetCurrentState(EGamePlayState NewState)
{
	CurrentState = NewState;
	HandleNewState(CurrentState);
}

void AAssignmentGameMode::HandleNewState(EGamePlayState NewState)
{
	switch (NewState)
	{
		case EGamePlayState::EPlaying:
			{
				//Do nothing
			//UGameplayStatics::SetGamePaused(this, false);
			//SetPause(GetPlayerController(),true);
			}
			break;

		case EGamePlayState::EGameOver:
			{
				UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
			}
			break;

		case EGamePlayState::EPaused:
			{
				//UGameplayStatics::SetGamePaused(this, true);
			}
			break;

		default:
		case EGamePlayState::EUnknown:
			{
				//Do nothing
			}
			break;
	}
}

void AAssignmentGameMode::HandlePlayerDeath()
{
	//We restert level when player dies
	SetCurrentState(EGamePlayState::EGameOver);
}
void AAssignmentGameMode::HandlePauseGame()
{
	if (UGameplayStatics::IsGamePaused(this))
	{
		SetCurrentState(EGamePlayState::EPlaying);
	}
	else if (!UGameplayStatics::IsGamePaused(this))
	{
		SetCurrentState(EGamePlayState::EPaused);
	}
}