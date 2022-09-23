

#include "Weapon.h"

#include "Blaster/MainCharacter/CodeMain.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon Mesh"));
	RootComponent = WeaponMesh;
	WeaponMesh->SetCollisionResponseToChannels(ECR_Block);
	WeaponMesh->SetCollisionResponseToChannel(ECC_Pawn,ECR_Ignore);
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	AreaSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Are Sphere"));
	AreaSphere->SetupAttachment(GetRootComponent());
	AreaSphere->SetCollisionResponseToChannels(ECR_Ignore);
	AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	PickupWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Pickiup Widget"));
	PickupWidget->SetupAttachment(GetRootComponent());

	/** Properties */
	// Weapon State
	WeaponState = EWeaponState::EWS_Initialised;

}

void AWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AWeapon,WeaponState)
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	PickupWidget->SetVisibility(false);
	if(HasAuthority())
	{
		AreaSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		AreaSphere->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);
		AreaSphere->OnComponentBeginOverlap.AddDynamic(this,&AWeapon::OnAreaSphereBeginOverlap);
		AreaSphere->OnComponentEndOverlap.AddDynamic(this,&AWeapon::OnAreaSphereEndOverlap);
	}
	
}
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::ShowPickupWidget(bool bShow)
{
	if(PickupWidget)
	{
		PickupWidget->SetVisibility(bShow);
	}
}


void AWeapon::OnAreaSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor)
	{
		ACodeMain* CodeMainRef = Cast<ACodeMain>(OtherActor);
		if(CodeMainRef)
		{
			CodeMainRef->SetOverlappedWeapon(this);
		
		}
	}
}

void AWeapon::OnAreaSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if(OtherActor)
	{
		ACodeMain* CodeMainRef = Cast<ACodeMain>(OtherActor);
		if(CodeMainRef)
		{
			
			CodeMainRef->SetOverlappedWeapon(nullptr);
		}
	}
}
// Weapon State changing
void AWeapon::SetWeaponState(EWeaponState State)
{
	WeaponState = State;
	ShowPickupWidget(false);
	 AreaSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AWeapon::OnRep_ChangeWeaponState()
{
	switch (WeaponState)
	{
	case EWeaponState::EWS_Equipped:
		ShowPickupWidget(false);
		break;
	default:
		break;
	}
}
