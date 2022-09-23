// Fill out your copyright notice in the Description page of Project Settings.


#include "CodeMainAnimInstance.h"

#include "GameFramework/CharacterMovementComponent.h"

void UCodeMainAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	CodeMainRef = Cast<ACodeMain>(TryGetPawnOwner());
}

void UCodeMainAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if(CodeMainRef == nullptr)
	{
		CodeMainRef = Cast<ACodeMain>(TryGetPawnOwner());
	}
	if(CodeMainRef == nullptr) return;
	bIsFalling = CodeMainRef->GetCharacterMovement()->IsFalling();

	FVector Vel = CodeMainRef->GetVelocity();
	Vel.Z = 0;
	MainCharacterSpeed = Vel.Size();

	bIsAccelerating = CodeMainRef->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0 ? true : false;
	bWeaponEquipped = CodeMainRef->IsWeaponEquipped();
	bCrouched = CodeMainRef->bIsCrouched;
}
