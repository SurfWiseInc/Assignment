// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractInterface.h"
#include "Components/TimelineComponent.h"
#include "Door.generated.h"

UCLASS()
class ASSIGNMENT_API ADoor : public AActor, public IInteractInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADoor();

	// Variable to hold the Curve asset
	UPROPERTY(EditAnywhere)
		UCurveFloat* DoorTimelineFloatCurve;

	UFUNCTION()
		virtual void OnInteract();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		class UStaticMeshComponent* DoorFrame;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		class UStaticMeshComponent* Door;

	//TimelineComponent to animate Door meshes
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UTimelineComponent* DoorTimelineComp;

	//Float Track Signature to handle our update track event
	FOnTimelineFloat UpdateFunctionFloat;

	//Function which updates our Door's relative location with the timeline graph
	UFUNCTION()
		void UpdateTimelineComp(float Output);

};
