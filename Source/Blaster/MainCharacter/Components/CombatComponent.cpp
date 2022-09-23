// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"

#include "Blaster/MainCharacter/CodeMain.h"
#include "Blaster/Weapons/Weapon.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Net/UnrealNetwork.h"


UCombatComponent::UCombatComponent()
{
	
	PrimaryComponentTick.bCanEverTick = false;


}



void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();


}



void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


}

void UCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UCombatComponent,EquippedWeapon);
	DOREPLIFETIME(UCombatComponent,bIsAiming);
}

// Weapon Equipping
void UCombatComponent::EquipWeapon(AWeapon* WeaponToEquip)
{
	if(CodeMainRef && WeaponToEquip)
	{
		EquippedWeapon = WeaponToEquip;
		EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped);
		
		const USkeletalMeshSocket* WeaponSocket = CodeMainRef->GetMesh()->GetSocketByName(FName("RightHandWeapon"));
		if(WeaponSocket)
		{
			WeaponSocket->AttachActor(EquippedWeapon,CodeMainRef->GetMesh());
			
			
		}
	}
}

