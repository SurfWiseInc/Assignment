// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "Assignment/Item.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	Capacity = 20;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	for (auto& Item : DefaultItems)
	{
		AddItem(Item);
	}
}

bool UInventoryComponent::AddItem(class UItem* Item) 
{
	if(Items.Num() >= Capacity || !Item)
	{
		return false;
	}

	Item->OwningInventory = this;
	Item->World = GetWorld();
	Items.Add(Item);

	OnInventoryUpdate.Broadcast();

	return true;
}

bool UInventoryComponent::RemoveItem(class UItem* Item)
{
	if (Item)
	{
		Item->OwningInventory = nullptr;
		Item->World = nullptr;
		Items.RemoveSingle(Item);
		OnInventoryUpdate.Broadcast();
		return true;
	}
	return false;
}