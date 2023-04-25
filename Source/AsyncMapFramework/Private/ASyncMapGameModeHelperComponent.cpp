// Fill out your copyright notice in the Description page of Project Settings.

#include "ASyncMapGameModeHelperComponent.h"

#include "TimerManager.h" 
#include "Kismet/GameplayStatics.h"
#include "Engine/LevelStreaming.h" 
#include "Engine/LatentActionManager.h" 

#include "Interface/MapInterface.h"
#include "Interface/AsyncPlayerControllerInterface.h"

// Sets default values for this component's properties
UASyncMapGameModeHelperComponent::UASyncMapGameModeHelperComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}



// Called when the game starts
void UASyncMapGameModeHelperComponent::BeginPlay()
{
	Super::BeginPlay();
	LoadingLevel = UGameplayStatics::GetStreamingLevel(GetOwner(), LoadingLevelName);

}

// Uhhh, We are making a Component... Maybe We could bind this function to the event in the *ACTUAL* PlayerController
void UASyncMapGameModeHelperComponent::OnSwapPlayerControllers(APlayerController * oldPlayer, APlayerController * newPlayer)
{
	PlayerController = newPlayer;

}

void UASyncMapGameModeHelperComponent::LoadMap_Implementation(FName LevelName, ELoadingMode loadingMode, bool IgnoreFade)
{
	if(!IgnoreFade){StartCameraFade();}
	auto nextToLoadMap = UGameplayStatics::GetStreamingLevel(GetOwner(),LevelName);
	LoadingLevel->LevelTransform = nextToLoadMap->LevelTransform;
	UGameplayStatics::LoadStreamLevel(GetOwner(), LoadingLevelName, true, false, FLatentActionInfo());
	// (Macro) IsFaded();
	IAsyncPlayerControllerInterface::Execute_TeleportToPlayerStart(PlayerController, LoadingLevelName);
	IMapInterface::Execute_SetPrologueMode(LoadingLevel,loadingMode);
	UGameplayStatics::UnloadStreamLevel(GetOwner(), CurrentLevelName, FLatentActionInfo(), false);
	// (Macro) RemoveCameraFade()
	IMapInterface::Execute_SetPrologueMode(nextToLoadMap,loadingMode);

	CurrentLevelName = LevelName;
	if (loadingMode != ELoadingMode::Prologue)
	{
		RemoveLoadingMap_Implementation(false/*??*/,loadingMode,false); return;
	};

	IMapInterface::Execute_SetLoadingWidgetVisibility(LoadingLevel,false);
}

void UASyncMapGameModeHelperComponent::RemoveLoadingMap_Implementation(bool LazyLoad, ELoadingMode loadingMode, bool IgnoreFade)
{
	if(!IgnoreFade){StartCameraFade();}
	if(LazyLoad && !CurrentLevel->IsLevelVisible())
	{
		CurrentLevel = UGameplayStatics::GetStreamingLevel(GetOwner(),CurrentLevelName);
		CurrentLevel->SetShouldBeVisible(true);
		FTimerDelegate RespawnDelegate = FTimerDelegate::CreateUObject( this, &UASyncMapGameModeHelperComponent::RemoveLoadingMap_Implementation, LazyLoad, loadingMode, IgnoreFade );
		GetOwner()->GetWorldTimerManager().SetTimer(HeldTimerHandle, RespawnDelegate, HeldTimerDuration, false);
		return;
	}

	UGameplayStatics::UnloadStreamLevel(GetOwner(), CurrentLevelName, FLatentActionInfo(), false);

	if (IsCorrectThePawnMode(PlayerController,loadingMode,CurrentLevelName))
	{
		// ReSpawn(bool bMenuPawn)
	}

	if(loadingMode == ELoadingMode::CustomLocation)
	{
		IAsyncPlayerControllerInterface::Execute_TeleportToLastTransform(PlayerController /*,... ?*/);
	}
	else
	{
		IAsyncPlayerControllerInterface::Execute_TeleportToPlayerStart(PlayerController,CurrentLevelName);
		// PlayerController (Interface)->SaveData(); 
		IAsyncPlayerControllerInterface::Execute_SaveData(PlayerController/*,... ?*/);
	}

	if(!IgnoreFade)
	{
		// (Macro) Remove CameraFade
	}

	return;
}

bool UASyncMapGameModeHelperComponent::IsCorrectThePawnMode(APlayerController *playerController, ELoadingMode NewParam, FName LevelName)
{
    return ((IAsyncPlayerControllerInterface::Execute_IsMenuPawn(playerController) && LevelName == "MainMenu") || LevelName == LoadingLevelName);
}

void UASyncMapGameModeHelperComponent::StartCameraFade()
{
	// TODO
}
