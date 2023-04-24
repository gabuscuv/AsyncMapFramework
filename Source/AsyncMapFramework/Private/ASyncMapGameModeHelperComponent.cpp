// Fill out your copyright notice in the Description page of Project Settings.


#include "Kismet/GameplayStatics.h"
#include "Interface/AsyncPlayerControllerInterface.h"
#include "Interface/MapInterface.h"
#include "Interface/ASyncPlayerControllerInterface.h"
#include "ASyncMapGameModeHelperComponent.h"

// Sets default values for this component's properties
UASyncMapHelperComponent::UASyncMapHelperComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}



// Called when the game starts
void UASyncMapHelperComponent::BeginPlay()
{
	Super::BeginPlay();
	LoadingLevel = GetStreamingLevel("Loading");

}

// Uhhh, We are making a Component... Maybe We could bind this function to the event in the *ACTUAL* PlayerController
void UASyncMapHelperComponent::OnSwapPlayerControllers(APlayerController * oldPlayer, APlayerController * newPlayer)
{
	PlayerController = newPlayer;

}

void UASyncMapHelperComponent::LoadMap_Implementation(FName LevelName, ELoadingMode loadingMode, bool IgnoreFade)
{
	if(!IgnoreFade){StartCameraFade();}
	auto nextToLoadMap = GetStreamingLevel(LevelName);
	LoadingLevel->LevelTransform = nextToLoadMap->LevelTransform;
	GameplayStatics::LoadStreamLevel(GetParent(), LoadingLevel, true, false);
	// (Macro) IsFaded();
	IAsyncPlayerControllerInterface::Execute_TeleportToPlayerStart(PlayerController,"Loading");
	IMapInterface::Execute_SetPrologueMode(LoadingLevel,LoadingMode);
	GameplayStatics::UnloadStreamLevel(GetParent(), CurrentLevelName);
	// (Macro) RemoveCameraFade()
	IMapInterface::Execute_SetPrologueMode(nextToLoadMap,LoadingMode);

	CurrentLevelName = LevelName;
	if (loadingMode != ELoadingMode::Prologue)
	{
		RemoveLoadingLevel(CurrentLevelName,loadingMode,false); return;
	};

	IMapInterface::Execute_SetLoadingWidgetLevel(LoadingLevel,false);
}

void UASyncMapHelperComponent::RemoveLoadingLevel_Implementation(bool LazyLoad, ELoadingMode loadingMode, bool IgnoreFade)
{
	if(!IgnoreFade){StartCameraFade();}
	if(LazyLoad && !CurrentLevel->IsVisible())
	{
		auto CurrentLevel = GetStreamingLevel(CurrentLevelName);
		CurrentLevel->bShouldBeVisible = true;
		FTimerDelegate RespawnDelegate = FTimerDelegate::CreateUObject( this, &UASyncMapHelperComponent::removeLoadingLevel_TimerElapsed, LazyLoad, loadingMode, IgnoreFade );
		GetOwner()->GetWorldTimerManager().SetTimer(HeldTimerHandle, this, RespawnDelegate, HeldTimerDuration, false);
		return;
	}

	GameplayStatics::UnloadStreamLevel(GetParent(), CurrentLevelName);

	if (IsCorrectThePawnMode(PlayerController,loadingMode,CurrentLevelName))
	{
		// ReSpawn(bool bMenuPawn)
	}

	if(loadingMode == ELoadingMode::CustomLocation)
	{
		IASyncPlayerControllerInterface::Execute_TeleportToLastTransform(PlayerController /*,... ?*/);
	}
	else
	{
		IASyncPlayerControllerInterface::Execute_TeleportToPlayerStart(PlayerController,CurrentLevelName);
		// PlayerController (Interface)->SaveData(); 
		IASyncPlayerControllerInterface::Execute_SaveData(PlayerController/*,... ?*/);
	}

	if(!IgnoreFade)
	{
		// (Macro) Remove CameraFade
	}

	return;
}

bool UASyncMapHelperComponent::IsCorrectThePawnMode(APlayerController *playerController, TEnumAsByte<ELoadingMode> NewParam, FName LevelName)
{
    return (playerController->GetIsPlayerPawn() && LevelName == "MainMenu") || LevelName == "Loading";
}