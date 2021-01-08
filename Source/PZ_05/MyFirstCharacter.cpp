// Fill out your copyright notice in the Description page of Project Settings.

#include "MyFirstCharacter.h"


// Sets default values
AMyFirstCharacter::AMyFirstCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TurnRate = 45.0f;
	LookUpRate = 45.0f;
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);
	PlayAnimGunEquipt = false;
	OnFire = false;
	GunEquipt = false;
	PlayAnimChangeAmmo = false;
	IsMyGunVisible = false;

	this->bUseControllerRotationPitch = false;
	this->bUseControllerRotationRoll = false;
	this->bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
	GetCharacterMovement()->JumpZVelocity = 550.f;
	GetCharacterMovement()->AirControl = 0.2f;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 300.f;
	SpringArm->bUsePawnControlRotation = true;

	CameraFlow = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraFlow->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	CameraFlow->bUsePawnControlRotation = false;

	SpawnProjectilePoint = CreateDefaultSubobject<USceneComponent>("SpawnPoint");
	SpawnProjectilePoint->SetupAttachment(RootComponent);

	MyGun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MyGun"));
	MyGun->SetupAttachment(GetMesh());

}


// Called to bind functionality to input
void AMyFirstCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("ChangeAmmo", IE_Pressed, this, &AMyFirstCharacter::ChangeAmoo);
	PlayerInputComponent->BindAction("GetGun", IE_Pressed, this, &AMyFirstCharacter::GetGun);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AMyFirstCharacter::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AMyFirstCharacter::StopFire);

	PlayerInputComponent->BindAxis("MoveForvard", this, &AMyFirstCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyFirstCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AMyFirstCharacter::TurneAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMyFirstCharacter::LookUpAtRate);

}

void AMyFirstCharacter::SetMyGunVisibility()
{
	IsMyGunVisible = !IsMyGunVisible;
	MyGun->SetVisibility(IsMyGunVisible);

}

// Called when the game starts or when spawned
void AMyFirstCharacter::BeginPlay()
{
	Super::BeginPlay();
	MyGun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("hand_rSocket"));
	MyGun->SetVisibility(false);
	if (AmmosForGun.Num() > 0)
	{
		CurrentAmmoIndex = 0;
		CurrentAmmo = AmmosForGun[CurrentAmmoIndex];
	}

	SpawnProjectilePoint->AttachToComponent(MyGun, FAttachmentTransformRules::KeepRelativeTransform, TEXT("Muzzle"));
}

// Called every frame
void AMyFirstCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



void AMyFirstCharacter::MoveForward(float AxisValue)
{
	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, AxisValue);
	}
}

void AMyFirstCharacter::MoveRight(float AxisValue)
{
	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, AxisValue);
	}
}

void AMyFirstCharacter::TurneAtRate(float Rate)
{
	AddControllerYawInput(Rate * TurnRate * GetWorld()->GetDeltaSeconds());
}

void AMyFirstCharacter::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * LookUpRate * GetWorld()->GetDeltaSeconds());
}

void AMyFirstCharacter::ChangeAmoo()
{
	if (GunEquipt && !PlayAnimChangeAmmo && !OnFire)
	{
		PlayAnimChangeAmmo = !PlayAnimChangeAmmo;
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Yellow, TEXT("Play Anim Change Ammo"));

		if (AmmosForGun.Num() > 0)
		{
			if (AmmosForGun.Num() - 1 == CurrentAmmoIndex)
			{
				CurrentAmmoIndex = 0;
				CurrentAmmo = AmmosForGun[CurrentAmmoIndex];
				GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Purple, TEXT("Ammo: " + FString::FromInt(CurrentAmmoIndex)));
			}
			else
			{
				++CurrentAmmoIndex;
				CurrentAmmo = AmmosForGun[CurrentAmmoIndex];
				GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Purple, TEXT("Ammo: " + FString::FromInt(CurrentAmmoIndex)));
			}
		}
	}
}

void AMyFirstCharacter::GetGun()
{
	//Reset playing equip animation
	if (!PlayAnimGunEquipt && !OnFire)
	{
		PlayAnimGunEquipt = true;
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Blue, TEXT("Play Anim Equipt"));
	}

	//UnEquipt Gun
	if (GunEquipt && !OnFire)
	{
		GunEquipt = !GunEquipt;
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Blue, TEXT("UnEquipt"));
		FTimerHandle Timer;
		GetWorldTimerManager().SetTimer(Timer, this, &AMyFirstCharacter::SetMyGunVisibility, 1, false);
	}
	//Equip Gun
	else if (!GunEquipt && !OnFire)
	{
		GunEquipt = !GunEquipt;
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Blue, TEXT("Equipt"));
		FTimerHandle Timer;
		GetWorldTimerManager().SetTimer(Timer, this, &AMyFirstCharacter::SetMyGunVisibility, 1, false);
	}
}

void AMyFirstCharacter::Fire()
{
	if (OnFire)
	{
		FTransform SpawnTransform = SpawnProjectilePoint->GetComponentTransform();
		auto SpawnPrt = GetWorld()->SpawnActor(CurrentAmmo, &SpawnTransform);

		//SpawnedProjectile = (AProjectile*)GetWorld()->SpawnActor(CurrentAmmo, SpawnTransform);

		FTimerHandle Timer;
		GetWorldTimerManager().SetTimer(Timer, this, &AMyFirstCharacter::Fire, 1.35, false);
	}
}



void AMyFirstCharacter::StartFire()
{
	if (GunEquipt)
	{
		OnFire = true;
		Fire();
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("Start Fire"));
	}
}


void AMyFirstCharacter::StopFire()
{
	if (GunEquipt && OnFire)
	{
		OnFire = false;
		Fire();
		GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("Stop Fire"));
	}
}