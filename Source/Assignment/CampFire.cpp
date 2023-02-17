// Fill out your copyright notice in the Description page of Project Settings.


#include "CampFire.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

// Sets default values
ACampFire::ACampFire()
{

	MyBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("My Box Component"));
	MyBoxComponent->InitBoxExtent(FVector(50.0f, 50.0f, 50.0f));
	RootComponent = MyBoxComponent;

	Fire = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("My Fire"));
	Fire->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	Fire->SetupAttachment(RootComponent);

	MyBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ACampFire::OnOverlapBegin);

	MyBoxComponent->OnComponentEndOverlap.AddDynamic(this, &ACampFire::OnOverlapEnd);

	bCanApplyDamage = true;
	DamageAmount = 10.f;
}

void ACampFire::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this && OtherComp) 
	{
		UE_LOG(LogTemp, Warning, TEXT("We overlapped with fire"));
		bCanApplyDamage = true;
		MyCharacter = Cast<AActor>(OtherActor);
		MyHit = SweepResult;
		GetWorldTimerManager().SetTimer(FireTimerHandle, this, &ACampFire::ApplyFireDamage, 2.2f, true, 0.0f);
	}
}

void ACampFire::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	bCanApplyDamage = false;
	GetWorldTimerManager().ClearTimer(FireTimerHandle);
}

void ACampFire::ApplyFireDamage()
{
	if (bCanApplyDamage)
	{
		UE_LOG(LogTemp, Warning, TEXT("We Apply Point Damage"));
		//UGameplayStatics::ApplyPointDamage(MyCharacter, DamageAmount, GetActorLocation(), MyHit, nullptr, this, FireDamageType);
		UGameplayStatics::ApplyDamage(MyCharacter, DamageAmount, nullptr, this, FireDamageType);
	}
}