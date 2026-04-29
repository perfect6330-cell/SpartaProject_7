#include "SpartaPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "SpartaPawn.h"

ASpartaPlayerController::ASpartaPlayerController()
{
	bShowMouseCursor = false;
}

void ASpartaPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (DefaultMappingContext)
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}
}

void ASpartaPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent);
	if (!EnhancedInput)
	{
		return;
	}

	if (MoveAction)
	{
		EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASpartaPlayerController::Move);
		EnhancedInput->BindAction(MoveAction, ETriggerEvent::Completed, this, &ASpartaPlayerController::Move);
	}

	if (LookAction)
	{
		EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASpartaPlayerController::Look);
	}

	if (JumpAction)
	{
		EnhancedInput->BindAction(JumpAction, ETriggerEvent::Started, this, &ASpartaPlayerController::StartJump);
		EnhancedInput->BindAction(JumpAction, ETriggerEvent::Completed, this, &ASpartaPlayerController::StopJump);
	}

	if (SprintAction)
	{
		EnhancedInput->BindAction(SprintAction, ETriggerEvent::Started, this, &ASpartaPlayerController::StartSprint);
		EnhancedInput->BindAction(SprintAction, ETriggerEvent::Completed, this, &ASpartaPlayerController::StopSprint);
	}
}

void ASpartaPlayerController::Move(const FInputActionValue& Value)
{
	ASpartaPawn* SpartaPawn = Cast<ASpartaPawn>(GetPawn());
	if (!SpartaPawn)
	{
		return;
	}

	const FVector2D MoveValue = Value.Get<FVector2D>();
	SpartaPawn->Move(MoveValue);
}

void ASpartaPlayerController::Look(const FInputActionValue& Value)
{
	ASpartaPawn* SpartaPawn = Cast<ASpartaPawn>(GetPawn());
	if (!SpartaPawn)
	{
		return;
	}

	const FVector2D LookValue = Value.Get<FVector2D>();
	SpartaPawn->Look(LookValue, GetWorld()->GetDeltaSeconds());
}

void ASpartaPlayerController::StartJump(const FInputActionValue& Value)
{
	ASpartaPawn* SpartaPawn = Cast<ASpartaPawn>(GetPawn());
	if (!SpartaPawn)
	{
		return;
	}

	SpartaPawn->StartJump();
}

void ASpartaPlayerController::StopJump(const FInputActionValue& Value)
{
	ASpartaPawn* SpartaPawn = Cast<ASpartaPawn>(GetPawn());
	if (!SpartaPawn)
	{
		return;
	}

	SpartaPawn->StopJump();
}

void ASpartaPlayerController::StartSprint(const FInputActionValue& Value)
{
	ASpartaPawn* SpartaPawn = Cast<ASpartaPawn>(GetPawn());
	if (!SpartaPawn)
	{
		return;
	}

	SpartaPawn->StartSprint();
}

void ASpartaPlayerController::StopSprint(const FInputActionValue& Value)
{
	ASpartaPawn* SpartaPawn = Cast<ASpartaPawn>(GetPawn());
	if (!SpartaPawn)
	{
		return;
	}

	SpartaPawn->StopSprint();
}