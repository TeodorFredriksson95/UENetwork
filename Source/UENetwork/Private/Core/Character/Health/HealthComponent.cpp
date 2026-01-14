// Fill out your copyright notice in the Description page of Project Settings.


#include "UENetwork/Public/Core/Character/Health/HealthComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "Projectile/ProjectileBase.h"


// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
	
	// Initialize player health
	MaxHealth = 100.0f;
	CurrentHealth = MaxHealth;
	
	//Initialize projectile class (SHOULD NOT LIVE IN HEALTH COMP)
	ProjectileClass = AProjectileBase ::StaticClass();
	FireRate = 0.25f;
	bIsFiringWeapon = false;
}

// FIRING LOGIC SHOULD NOT LIVE HERE
void UHealthComponent::HandleFire_Implementation()
{
	AActor* Owner = GetOwner();
	FVector spawnLocation = Owner->GetActorLocation() + ( Owner->GetActorRotation().Vector()  * 100.0f ) + (Owner->GetActorUpVector() * 50.0f);
	FRotator spawnRotation = Owner->GetActorRotation();
 
	FActorSpawnParameters spawnParameters;
	spawnParameters.Instigator = Owner->GetInstigator();
	spawnParameters.Owner = Owner;
 
	AProjectileBase* spawnedProjectile = GetWorld()->SpawnActor<AProjectileBase>(spawnLocation, spawnRotation, spawnParameters);
}

void UHealthComponent::StartFire()
{
	if (!bIsFiringWeapon)
	{
		bIsFiringWeapon = true;
		UWorld* World = GetWorld();
		World->GetTimerManager().SetTimer(FiringTimer, this, &UHealthComponent::StopFire, FireRate, false);
		HandleFire();
	}
}

void UHealthComponent::StopFire()
{
	bIsFiringWeapon = false;
}
// END OF FIRE BLOCK

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	// Replicate current health
	DOREPLIFETIME(UHealthComponent, CurrentHealth);
}


void UHealthComponent::SetCurrentHealth(float HealthValue)
{
	AActor* Owner = GetOwner();
	if (Owner->GetLocalRole() == ROLE_Authority)
	{
		CurrentHealth = FMath::Clamp(HealthValue, 0.0f, MaxHealth);
		OnHealthUpdate();
	}	
	
}

float UHealthComponent::TakeDamage(float DamageTaken, struct FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	float DamageApplied = CurrentHealth - DamageTaken;
	SetCurrentHealth(DamageApplied);
	return DamageApplied;
}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UHealthComponent::OnHealthUpdate()
{
	AActor* Owner = GetOwner();
	
	if (!Owner)
	{
		return;
	}
	
	// Client-specific functionality
	APawn* OwningPawn = Cast<APawn>(Owner);
	if (OwningPawn && OwningPawn->IsLocallyControlled())
	{
		FString HealthMessage = FString::Printf(TEXT("You now have %f health remaining."), CurrentHealth);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, HealthMessage);
		
		if (CurrentHealth <= 0)
		{
			FString DeathMessage = FString::Printf(TEXT("You have have been killed."));
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, DeathMessage);
		}
	}
	
	// Server-specific functionality
	if (OwningPawn->GetLocalRole() == ROLE_Authority)
	{
		FString HealthMessage = FString::Printf(TEXT("%s now has %f health remaining."), *GetFName().ToString(), CurrentHealth);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, HealthMessage);
	}
	
}


void UHealthComponent::OnRep_CurrentHealth()
{
	OnHealthUpdate();
}




// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

