// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "Components/InventoryComponent.h"


UItem::UItem()
{
	DisplayName = FText::FromString("Item");
	UseActionText = FText::FromString("Use");
}

void UItem::Use(class AAssignmentCharacter* Character)
{

}

void UItem::AddedToInventory(class UInventoryComponent* Inventory)
{

}
