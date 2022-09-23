// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"
UENUM(BlueprintType)
enum class EWeaponState: uint8
{
	EWS_Initialised,
	EWS_Equipped,
	EWS_Dropped
};
UCLASS()
class BLASTER_API AWeapon : public AActor
{
	GENERATED_BODY()
	/** Mesh for The weapon */
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = WeaponMesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* WeaponMesh;
	/** Trigger Sphere for Weapon */
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = AreaSphere, meta = (AllowPrivateAccess = "true"))
	USphereComponent* AreaSphere;
	/** Weapon State enums */
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,ReplicatedUsing=OnRep_ChangeWeaponState,Category = "Properties | State",meta = (AllowPrivateAccess = "true"))
	EWeaponState WeaponState;

	/** Pickup UI */
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "UI | Pickup",meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* PickupWidget;
	
	
public:	

	AWeapon();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	

protected:
	
	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;

	void ShowPickupWidget(bool bShow);

	void SetWeaponState(EWeaponState State) ;
	UFUNCTION()
	void OnRep_ChangeWeaponState();
	
	UFUNCTION()
	void OnAreaSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	void OnAreaSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	FORCEINLINE EWeaponState GetWeaponState() {return WeaponState;}
	 

};

