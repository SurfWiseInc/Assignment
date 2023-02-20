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


	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* PickupMesh;

	//This is used as a template to create the pipckup when spawned 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Instanced)
		class UItem* ItemTemplate;

	//Component used for checking collisions
	UPROPERTY(EditAnywhere)
		class UBoxComponent* BoxComp;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//This item that will be added to the inventory when this pickup is taken
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		class UItem* Item;

	UFUNCTION()
		void OnTakePickup(class AAssignmentCharacter* Taker);

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
