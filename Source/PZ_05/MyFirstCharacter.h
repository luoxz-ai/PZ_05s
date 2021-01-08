// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "Projectile.h"
#include "MyFirstCharacter.generated.h"

UCLASS()
class PZ_05_API AMyFirstCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyFirstCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DefaultMyChar")
	TArray < TSubclassOf<AProjectile> > AmmosForGun;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCameraComponent* CameraFlow;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USkeletalMeshComponent* MyGun;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TurnRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LookUpRate;

	UFUNCTION()
	void StartFire();
	UFUNCTION()
	void StopFire();

	UPROPERTY(BlueprintReadWrite)
	bool PlayAnimGunEquipt;

	UPROPERTY(BlueprintReadWrite)
	bool OnFire;

	UPROPERTY(BlueprintReadWrite)
	bool PlayAnimChangeAmmo;

	UFUNCTION()
	void SetMyGunVisibility();

	UPROPERTY()
	AProjectile* SpawnedProjectile;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool IsMyGunVisible;

	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);

	void TurneAtRate(float Rate);
	void LookUpAtRate(float Rate);

	void ChangeAmoo();
	void GetGun();

	void Fire();

	UPROPERTY()
	bool GunEquipt;

	UPROPERTY()
	USceneComponent* SpawnProjectilePoint;

	UPROPERTY()
	TSubclassOf<AProjectile> CurrentAmmo;

	UPROPERTY()
	int32 CurrentAmmoIndex;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	
};
