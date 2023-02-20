// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup.h"
#include "AssignmentCharacter.h"
#include "Item.h"
#include "Components/InventoryComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
APickup::APickup()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	RootComponent = BoxComp;

	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pickup Mesh"));
	PickupMesh->AttachToComponent(BoxComp, FAttachmentTransformRules::KeepRelativeTransform);

	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &APickup::OnOverlapBegin);


}

void APickup::InitializePickup(const TSubclassOf<class UItem> ItemClass)
{
	if (ItemClass)
	{
		Item = NewObject<UItem>(this, ItemClass);
		PickupMesh->SetStaticMesh(Item->PickupMesh);
	}
}

// Called when the game starts or when spawned
void APickup::BeginPlay()
{
	Super::BeginPlay();

	if (ItemTemplate)
	{
		InitializePickup(ItemTemplate->GetClass());
	}
}

void APickup::OnTakePickup(AAssignmentCharacter* Taker)
{
	if (Taker)
	{
		Taker->Inventory->AddItem(Item);
	}
}

void APickup::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this && OtherComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("We overlapped with pickup"));

		OverlappedComp->SetActive(false);
		OverlappedComp->GetOwner()->SetActorHiddenInGame(true);

		auto MyChar = Cast<AAssignmentCharacter>(OtherActor);

		OnTakePickup(MyChar);

	}
}

