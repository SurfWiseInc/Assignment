// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

UCLASS()
class ASSIGNMENT_API APickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickup();

	//Takes the item to represent and creates the pickup from it. Done on BeginPlay and when a player drops an item on the ground
	void InitializePickup(const TSubclassOf<class UItem> ItemClass);



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	UFUNCTION()
		void OnTakePickup(class AAssignmentCharacter* Taker);

};
