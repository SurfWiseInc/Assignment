// Fill out your copyright notice in the Description page of Project Settings.


#include "MedKit.h"
#include "AssignmentCharacter.h"

// Sets default values
AMedKit::AMedKit()
{
	OnActorBeginOverlap.AddDynamic(this, &AMedKit::OnOverlap);

	HealAmount = 50;
}

void AMedKit::OnOverlap(AActor* MyOverlappedActor, AActor* OtherActor)
{
	if(OtherActor && OtherActor != this)
	{
		MyCharacter = Cast<AAssignmentCharacter>(OtherActor);

		if (MyCharacter)
		{
			MyCharacter->UpdateHealth(HealAmount);
			Destroy();
		}
	}
}


