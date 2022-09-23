// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLASTER_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UCombatComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	friend class ACodeMain;
protected:
	
	virtual void BeginPlay() override;
private:
	UPROPERTY(VisibleAnywhere)
	ACodeMain* CodeMainRef;
	UPROPERTY(VisibleAnywhere,Replicated)
	class AWeapon* EquippedWeapon;
	UPROPERTY(VisibleAnywhere,Replicated)
	bool bIsAiming;
public:
	void EquipWeapon(AWeapon* WeaponToEquip);
	
	

		
};
