#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SpartaPawn.generated.h"

struct FInputActionValue;

class UCapsuleComponent;
class USkeletalMeshComponent;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class SPARTAPROJECT_API ASpartaPawn : public APawn
{
	GENERATED_BODY()

public:
	ASpartaPawn();

	virtual void Tick(float DeltaTime) override;

	// PlayerController가 호출할 함수들
	void Move(const FVector2D& InputValue);
	void Look(const FVector2D& InputValue, float DeltaTime);
	void StartJump();
	void StopJump();
	void StartSprint();
	void StopSprint();

protected:
	virtual void BeginPlay() override;

private:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void HandleMovement(float DeltaTime);
	void HandleJump(float DeltaTime);
	void UpdateAnimationState();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pawn|Components", meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* CapsuleComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pawn|Components", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pawn|Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pawn|Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn|Movement", meta = (AllowPrivateAccess = "true"))
	float NormalMoveSpeed = 350.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn|Movement", meta = (AllowPrivateAccess = "true"))
	float SprintMoveSpeed = 700.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn|Movement", meta = (AllowPrivateAccess = "true"))
	float LookYawSpeed = 120.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn|Movement", meta = (AllowPrivateAccess = "true"))
	float LookPitchSpeed = 120.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn|Movement", meta = (AllowPrivateAccess = "true"))
	float MinPitch = -60.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn|Movement", meta = (AllowPrivateAccess = "true"))
	float MaxPitch = 45.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn|Jump", meta = (AllowPrivateAccess = "true"))
	float JumpHeight = 150.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn|Jump", meta = (AllowPrivateAccess = "true"))
	float JumpUpSpeed = 350.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn|Jump", meta = (AllowPrivateAccess = "true"))
	float JumpDownSpeed = 350.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pawn|State", meta = (AllowPrivateAccess = "true"))
	FVector2D MoveInput = FVector2D::ZeroVector;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pawn|State", meta = (AllowPrivateAccess = "true"))
	bool bSprintHeld = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pawn|State", meta = (AllowPrivateAccess = "true"))
	bool bIsJumping = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pawn|State", meta = (AllowPrivateAccess = "true"))
	bool bIsGoingUp = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pawn|State", meta = (AllowPrivateAccess = "true"))
	bool bIsFallingLike = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pawn|State", meta = (AllowPrivateAccess = "true"))
	float GroundZ = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pawn|Animation", meta = (AllowPrivateAccess = "true"))
	float CurrentSpeed = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pawn|Animation", meta = (AllowPrivateAccess = "true"))
	bool bIsMoving = false;
};