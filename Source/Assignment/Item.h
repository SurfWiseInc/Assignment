// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Item.generated.h"

/**
 * 
 */
UCLASS(Abstract, BlueprintType, Blueprintable, EditInlineNew, DefaultToInstanced)
class ASSIGNMENT_API UItem : public UObject
{
	GENERATED_BODY()
	
protected:
	virtual class UWorld* GetWorld() const { return World; };

public:
	UItem();

	UPROPERTY(Transient)
		class UWorld* World;

	/**The mesh to display for this items pickup*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
		class UStaticMesh* PickupMesh;

	/**The thumbnail for this item*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
		class UTexture2D* Thumbnail;

	/**The display name for this item in the inventory*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
		FText DisplayName;

	/**An optional description for the item*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item", meta = (MultiLine = true))
		FText Description;

	/**The text for using the item. (Equip, Eat, etc)*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
		FText UseActionText;

	//The inventory that owns this item
	UPROPERTY()
		class UInventoryComponent* OwningInventory;


	UFUNCTION(BlueprintImplementableEvent)
		void OnUse(class AAssignmentCharacter* Character);

	virtual void Use(class AAssignmentCharacter* Character);
	
	virtual void AddedToInventory(class UInventoryComponent* Inventory);
};
