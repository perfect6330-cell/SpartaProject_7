#include "SpartaPawn.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"

ASpartaPawn::ASpartaPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	SetRootComponent(CapsuleComp);
	CapsuleComp->InitCapsuleSize(42.f, 96.f);
	CapsuleComp->SetSimulatePhysics(false);

	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(CapsuleComp);
	MeshComp->SetSimulatePhysics(false);
	MeshComp->SetRelativeLocation(FVector(0.f, 0.f, -96.f));

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(CapsuleComp);
	SpringArmComp->TargetArmLength = 300.f;
	SpringArmComp->bUsePawnControlRotation = false;
	SpringArmComp->bDoCollisionTest = true;
	SpringArmComp->SetRelativeRotation(FRotator(-10.f, 0.f, 0.f));

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false;

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

void ASpartaPawn::BeginPlay()
{
	Super::BeginPlay();

	GroundZ = GetActorLocation().Z;
}

void ASpartaPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	HandleMovement(DeltaTime);
	HandleJump(DeltaTime);
	UpdateAnimationState();
}

void ASpartaPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ASpartaPawn::Move(const FVector2D& InputValue)
{
	MoveInput = InputValue;
}

void ASpartaPawn::Look(const FVector2D& InputValue, float DeltaTime)
{
	if (InputValue.IsNearlyZero())
	{
		return;
	}

	const float DeltaYaw = InputValue.X * LookYawSpeed * DeltaTime;
	AddActorLocalRotation(FRotator(0.f, DeltaYaw, 0.f));

	FRotator SpringArmRot = SpringArmComp->GetRelativeRotation();

	SpringArmRot.Pitch -= InputValue.Y * LookPitchSpeed * DeltaTime;
	SpringArmRot.Pitch = FMath::Clamp(SpringArmRot.Pitch, MinPitch, MaxPitch);

	SpringArmComp->SetRelativeRotation(FRotator(SpringArmRot.Pitch, 0.f, 0.f));
}

void ASpartaPawn::StartJump()
{
	if (bIsJumping)
	{
		return;
	}

	bIsJumping = true;
	bIsGoingUp = true;
	bIsFallingLike = false;
	GroundZ = GetActorLocation().Z;
}

void ASpartaPawn::StopJump()
{
	if (bIsJumping && bIsGoingUp)
	{
		bIsGoingUp = false;
		bIsFallingLike = true;
	}
}

void ASpartaPawn::StartSprint()
{
	bSprintHeld = true;
}

void ASpartaPawn::StopSprint()
{
	bSprintHeld = false;
}

void ASpartaPawn::HandleMovement(float DeltaTime)
{
	FVector2D ClampedInput = MoveInput;

	if (ClampedInput.Length() > 1.f)
	{
		ClampedInput.Normalize();
	}

	const float MoveSpeed = bSprintHeld ? SprintMoveSpeed : NormalMoveSpeed;

	FVector Forward = GetActorForwardVector();
	FVector Right = GetActorRightVector();

	Forward.Z = 0.f;
	Right.Z = 0.f;

	Forward.Normalize();
	Right.Normalize();

	const FVector MoveDelta =
		(Forward * ClampedInput.Y + Right * ClampedInput.X) * MoveSpeed * DeltaTime;

	if (!MoveDelta.IsNearlyZero())
	{
		AddActorWorldOffset(MoveDelta, true);
	}
}

void ASpartaPawn::HandleJump(float DeltaTime)
{
	if (!bIsJumping)
	{
		return;
	}

	FVector NewLocation = GetActorLocation();

	if (bIsGoingUp)
	{
		NewLocation.Z += JumpUpSpeed * DeltaTime;

		if (NewLocation.Z >= GroundZ + JumpHeight)
		{
			NewLocation.Z = GroundZ + JumpHeight;
			bIsGoingUp = false;
			bIsFallingLike = true;
		}
	}
	else
	{
		NewLocation.Z -= JumpDownSpeed * DeltaTime;

		if (NewLocation.Z <= GroundZ)
		{
			NewLocation.Z = GroundZ;
			bIsJumping = false;
			bIsGoingUp = false;
			bIsFallingLike = false;
		}
	}

	SetActorLocation(NewLocation, true);
}

void ASpartaPawn::UpdateAnimationState()
{
	FVector2D ClampedInput = MoveInput;

	if (ClampedInput.Length() > 1.f)
	{
		ClampedInput.Normalize();
	}

	const float MoveSpeed = bSprintHeld ? SprintMoveSpeed : NormalMoveSpeed;

	CurrentSpeed = ClampedInput.Size() * MoveSpeed;
	bIsMoving = CurrentSpeed > 0.f;
}