// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Components/TimelineComponent.h"
#include "AssignmentCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPlayerDiedSignature); //, AActor, OnPlayerDied
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPauseGameSignature);

UCLASS(config=Game)
class AAssignmentCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Shoot Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* ShootAction;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	/** Run Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* RunAction;

	/** Pause Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* PauseAction;

public:
	AAssignmentCharacter();

	UFUNCTION(BlueprintCallable, Category = "Items")
		void UseItem(class UItem* Item);
	
	/** Inventory */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UInventoryComponent* Inventory;

protected:

	void Shoot(const FInputActionValue& Value);

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	void StartRun(const FInputActionValue& Value);
	void StopRun(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Pausefor looking input */
	void Pause(const FInputActionValue& Value);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

	virtual void Tick(float DeltaTime) override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UPROPERTY(BlueprintAssignable, Category = "Game|Damage")
		FPlayerDiedSignature OnPlayerDied;

	UPROPERTY(BlueprintAssignable, Category = "Game|Pause")
		FPauseGameSignature OnPauseGame;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		float FullHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		float Health;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		float HealthPercentage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		bool bRedFlash;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
		float WalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
		float RunSpeed;

	float bIsRunning;
	bool bCanRegenerateStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
		float FullStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
		float Stamina;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
		float StaminaPercentage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
		float PreviousStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
		float StaminaDropRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
		float StaminaRegenerateRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
		float StaminaDrop;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
		int32 AmmoAmount;

	UPROPERTY(EditAnywhere, Category = "Stamina")
		FTimeline MyTimeline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
		FTimerHandle MemberTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
		FTimerHandle RegenerateStaminaTimerHandle;

	UFUNCTION()
		void SetCanRegenerate();

	float CurveFloatValue;
	float TimelineValue;

	UFUNCTION(BlueprintPure, Category = "Health")
		float GetHealth() const;

	UFUNCTION(BlueprintPure, Category = "Health")
		FText GetHealthInText() const;

	UFUNCTION(BlueprintPure, Category = "Stamina")
		float GetStamina() const;

	UFUNCTION(BlueprintPure, Category = "Stamina")
		FText GetStaminaInText() const;

	UFUNCTION(BlueprintPure, Category = "Stamina")
		FText GetAmmoInText() const;

	UFUNCTION()
		void SetWalkSpeed();

	UFUNCTION()
		void DamageTimer();

	UFUNCTION()
		void SetDamageState();

	UFUNCTION()
		void RegenerateStamina(float StaminaRegenRate);

	UFUNCTION()
		void UpdateStamina();

	UFUNCTION(BlueprintPure, Category = "Health")
		bool PlayFlash();

	UFUNCTION()
		void TakeDamage(AActor* DamagedActor, float DamageAmount, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION(BlueprintCallable, Category = "Health")
		void UpdateHealth(float HealthChange);
};

