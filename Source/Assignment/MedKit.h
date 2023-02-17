// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MedKit.generated.h"

UCLASS()
class ASSIGNMENT_API AMedKit : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMedKit();

	UPROPERTY(EditAnywhere)
		float HealAmount;

	UFUNCTION()
		void OnOverlap(AActor* MyOverlappedActor, AActor* OtherActor);

	UPROPERTY()
		class AAssignmentCharacter* MyCharacter;

};
