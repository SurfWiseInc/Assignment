// Copyright Epic Games, Inc. All Rights Reserved.

#include "AssignmentCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"


//////////////////////////////////////////////////////////////////////////
// AAssignmentCharacter

AAssignmentCharacter::AAssignmentCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 400.0f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	AmmoAmount = 10;

	FullHealth = 100.f;
	Health = FullHealth;
	HealthPercentage = 1;
	SetCanBeDamaged(true);

	WalkSpeed = 300.f;
	RunSpeed = 600.f;
	FullStamina = 100.f;
	Stamina = FullStamina;
	StaminaPercentage = 1;
	StaminaDropRate = 30;
	StaminaRegenerateRate= 25;
	bIsRunning = false;
}

void AAssignmentCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	//Bind to taking damage
	OnTakeAnyDamage.AddDynamic(this, &AAssignmentCharacter::TakeDamage);

}
//////////////////////////////////////////////////////////////////////////
////Tick
void AAssignmentCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

		UpdateStamina();
}

//////////////////////////////////////////////////////////////////////////
// Input

void AAssignmentCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAssignmentCharacter::Move);

		//Sprinting
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Started, this, &AAssignmentCharacter::StartRun);
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &AAssignmentCharacter::StopRun);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AAssignmentCharacter::Look);

		//Shooting
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &AAssignmentCharacter::Shoot);

		//Shooting
		EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Triggered, this, &AAssignmentCharacter::Pause);
	}

}

void AAssignmentCharacter::Pause(const FInputActionValue& Value)
{
	OnPauseGame.Broadcast();
}

void AAssignmentCharacter::Shoot(const FInputActionValue& Value)
{
	if (AmmoAmount > 0) 
	{ 	
		AmmoAmount--; 
	}
	UE_LOG(LogTemp, Warning, TEXT("Have %d Bullets"), AmmoAmount);
}

void AAssignmentCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AAssignmentCharacter::StartRun(const FInputActionValue& Value)
{
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	bIsRunning = true;
}

void AAssignmentCharacter::StopRun(const FInputActionValue& Value)
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	bIsRunning = false;

	bCanRegenerateStamina = false;
	GetWorldTimerManager().SetTimer(RegenerateStaminaTimerHandle, this, &AAssignmentCharacter::SetCanRegenerate, 1.0f, false);
}

void AAssignmentCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

float AAssignmentCharacter::GetHealth() const
{
	return HealthPercentage;
}

float AAssignmentCharacter::GetStamina() const
{
	return StaminaPercentage;
}

FText AAssignmentCharacter::GetHealthInText() const
{
	int32 HP = FMath::RoundHalfFromZero(HealthPercentage * 100);
	FString HPS = FString::FromInt(HP);
	FString HealthHUD = HPS + FString(TEXT("%"));
	FText HPTEXT = FText::FromString(HealthHUD);
	return HPTEXT;
}

FText AAssignmentCharacter::GetStaminaInText() const
{
	int32 HP = FMath::RoundHalfFromZero(StaminaPercentage * 100);
	FString SStamina = FString::FromInt(HP);
	FString FullStaminaS = FString::FromInt(FullStamina);
	FString StaminaHUD = SStamina + FString(TEXT(" / ")) + FullStaminaS;
	FText StaminaTEXT = FText::FromString(StaminaHUD);
	return StaminaTEXT;
}

FText AAssignmentCharacter::GetAmmoInText(/*int32 AmmoAmount*/) const
{
	FString SAmmoAmount = FString::FromInt(AmmoAmount);
	FText AmmoTEXT = FText::FromString(SAmmoAmount);
	return AmmoTEXT;
}


void AAssignmentCharacter::DamageTimer() 
{
	GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &AAssignmentCharacter::SetDamageState, 0.5f, false);
}

void AAssignmentCharacter::SetDamageState()
{
	SetCanBeDamaged(true);
}

void AAssignmentCharacter::UpdateHealth(float HealthChange)
{
	Health = FMath::Clamp(Health += HealthChange, 0.f, FullHealth);
	HealthPercentage = Health / FullHealth;
}

void AAssignmentCharacter::TakeDamage(AActor* DamagedActor, float DamageAmount, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	UE_LOG(LogTemp, Warning, TEXT("We deal damage"));
	SetCanBeDamaged(false);
	bRedFlash = true;
	UpdateHealth(-DamageAmount);

	if (FMath::IsNearlyZero(HealthPercentage, 0.001f))
	{
		OnPlayerDied.Broadcast();
		return;
	}

	DamageTimer();
}

void AAssignmentCharacter::RegenerateStamina(float StaminaRegenRate)
{
		Stamina = FMath::Clamp(Stamina + (StaminaRegenerateRate * GetWorld()->GetDeltaSeconds()), 0, FullStamina);
		StaminaPercentage = Stamina / FullStamina;
}

void AAssignmentCharacter::SetCanRegenerate()
{
	bCanRegenerateStamina = true;
	UE_LOG(LogTemp, Warning, TEXT("Setting Regenerate"));
}

void AAssignmentCharacter::UpdateStamina()
{
	if (!bIsRunning && Stamina != FullStamina && bCanRegenerateStamina) 
	{
		RegenerateStamina(StaminaRegenerateRate);
	}
	else if (bIsRunning) 
	{
		Stamina = FMath::Clamp(Stamina - (StaminaRegenerateRate * GetWorld()->GetDeltaSeconds()), 0, FullStamina);
		StaminaPercentage = Stamina / FullStamina;

		if (FMath::IsNearlyZero(Stamina, 0.001f))
		{ 
			bCanRegenerateStamina = false;
			bIsRunning = false;
			GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
			GetWorldTimerManager().SetTimer(RegenerateStaminaTimerHandle, this, &AAssignmentCharacter::SetCanRegenerate, 2.0f, false);
		}
	}
}

void AAssignmentCharacter::SetWalkSpeed()
{
	GetCharacterMovement()->MaxWalkSpeed = bIsRunning ? RunSpeed : WalkSpeed;
	/*UCharacterMovementComponent* MovementComponent = FindComponentByClass<UCharacterMovementComponent>();
	if (MovementComponent)
	{
		MovementComponent->MaxWalkSpeed = WalkSpeed;
		bIsRunning = false;
	}*/
}

bool AAssignmentCharacter::PlayFlash()
{
	if (bRedFlash)
	{
		bRedFlash = false;
			return true;
	}

	return false;
}




