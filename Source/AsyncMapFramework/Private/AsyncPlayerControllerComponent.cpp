// Fill out your copyright notice in the Description page of Project Settings.


#include "AsyncPlayerControllerComponent.h"

#include "Structs/HMDOffset.h"

#include "Kismet/GameplayStatics.h" 
#include "GameFramework/PlayerStart.h" 

// Sets default values for this component's properties
UAsyncPlayerControllerComponent::UAsyncPlayerControllerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAsyncPlayerControllerComponent::BeginPlay()
{
	Super::BeginPlay();

	// TODO: FIX THIS
	PlayerController->PlayerCameraManager->SetManualCameraFade(1, FadeColor, false);	
}


FDateTime UAsyncPlayerControllerComponent::StartCameraFade_Implementation()
{
	PlayerController->PlayerCameraManager->StartCameraFade(0.0f, 1.0f, FadeDuration, FadeColor, false, true);
	return GetTimeFadding();
};

FDateTime UAsyncPlayerControllerComponent::StopCameraFade_Implementation()
{
	PlayerController->PlayerCameraManager->StartCameraFade(1.0f, 0.0f, FadeDuration, FadeColor, false, true);
	return GetTimeFadding();
};

FDateTime UAsyncPlayerControllerComponent::GetTimeFadding()
{
	return FDateTime::Now() + FTimespan(FadeDuration);
};

void UAsyncPlayerControllerComponent::TeleportToPlayerStart_Implementation(FName LevelName)
{

}

FVector UAsyncPlayerControllerComponent::GetPlayerCoords(FName name)
{
	#ifdef VRExpansion
	// Get All Actors with Tag VRPlayerStart
	#endif
	TArray<AActor *> outActors;

	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), APlayerStart::StaticClass(), name, outActors);

	if (outActors.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("F"));
		return FVector(0.0f, 0.0f, 0.0f);
	}

	//outActors.Get(0);

	return FVector();
};


FVector UAsyncPlayerControllerComponent::HMDTeleportAdjustment(AActor* targetActor)
{
	return FVector();
}

void UAsyncPlayerControllerComponent::TeleportToLastTransform_Implementation()
{
	
}

bool UAsyncPlayerControllerComponent::IsMenuPawn_Implementation()
{
	return false;
}

bool UAsyncPlayerControllerComponent::ReSpawn_Implementation(bool MenuPawn)
{
	return false;
}

void UAsyncPlayerControllerComponent::LoadSaveData_Implementation(FTransform LastLoadedMap, TMap<EEvents,bool> & EventsEnabled)
{
	// It's not planned an implementation
}

void UAsyncPlayerControllerComponent::SaveData_Implementation(FName LastLoadedMap, ELoadingMode LoadingMode)
{
	// It's not Planned a implementation
}

// Probably a Dumbass this
APlayerCameraManager* UAsyncPlayerControllerComponent::GetOwnerPlayerCameraManager_Implementation()
{
	return nullptr;
}