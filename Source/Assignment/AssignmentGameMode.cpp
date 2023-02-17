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

	static ConstructorHelpers::FClassFinder<UUserWidget> HealthBar(TEXT("/Game/UI/HUD_Widget"));
	HUDWidgetClass = HealthBar.Class;

	// use our custom HUD class
	HUDClass = AHUD::StaticClass();

	// add Health Bar UI to viewport
	if (HUDWidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);

		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport();
		}
	}
}

void AAssignmentGameMode::BeginPlay() {

	Super::BeginPlay();

	SetCurrentState(EGamePlayState::EPlaying);

	MyCharacter = Cast<AAssignmentCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
}

void AAssignmentGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MyCharacter)
	{
		if (FMath::IsNearlyZero(MyCharacter->GetHealth(), 0.001f))
		{
			SetCurrentState(EGamePlayState::EGameOver);
		}

	}
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
		}
		break;

	case EGamePlayState::EGameOver:
		{
			UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
		}
		break;

	case EGamePlayState::EPaused:
		{
			UGameplayStatics::SetGamePaused(this, true);
		}

	default:
	case EGamePlayState::EUnknown:
		{
			//Do nothing
		}
		break;
	}
}

