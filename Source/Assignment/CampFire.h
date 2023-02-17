// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CampFire.generated.h"


UCLASS()
class ASSIGNMENT_API ACampFire : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACampFire();

	UPROPERTY(EditAnywhere)
		float DamageAmount;

	UPROPERTY(EditAnywhere)
	class UParticleSystemComponent* Fire;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* MyBoxComponent;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UDamageType> FireDamageType;

	UPROPERTY(EditAnywhere)
	AActor* MyCharacter;
	 
	UPROPERTY(EditAnywhere)
	FHitResult MyHit;

	bool bCanApplyDamage;
	FTimerHandle FireTimerHandle;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
						int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
						int32 OtherBodyIndex);
	UFUNCTION()
		void ApplyFireDamage();

};
