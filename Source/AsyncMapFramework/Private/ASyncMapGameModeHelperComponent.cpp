// Copyright 2023 Gabriel Bustillo del Cuvillo

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
	LoadingLevel = UGameplayStatics::GetStreamingLevel(GetOwner(), LoadingLevelName);
	Super::BeginPlay();
}

// Uhhh, We are making a Component... Maybe We could bind this function to the event in the *ACTUAL* PlayerController
void UASyncMapGameModeHelperComponent::OnSwapPlayerControllers(APlayerController *oldPlayer, APlayerController *newPlayer)
{
	PlayerController = newPlayer;
}

void UASyncMapGameModeHelperComponent::LoadMap_Implementation(FName levelName, ELoadingMode loadingMode, bool IgnoreFade)
{
	if (!IgnoreFade){ StartCameraFade(); }

	LoadingLevel->LevelTransform = UGameplayStatics::GetStreamingLevel(GetOwner(), levelName)->LevelTransform;
	UGameplayStatics::LoadStreamLevel(GetOwner(), LoadingLevelName, true, false, FLatentActionInfo());
	LoadMap_Implementation_TimeElapsed(levelName, loadingMode, IgnoreFade);
}

void UASyncMapGameModeHelperComponent::LoadMap_Implementation_TimeElapsed(FName levelName, ELoadingMode loadingMode, bool IgnoreFade)
{
	if ( PlayerController == nullptr || (!IgnoreFade && TimeToFadding < FDateTime::Now()))
	{
		LoadingLevelDelegate = FTimerDelegate::CreateUObject(this, &UASyncMapGameModeHelperComponent::LoadMap_Implementation_TimeElapsed, levelName, loadingMode, IgnoreFade);
		GetOwner()->GetWorldTimerManager().SetTimer(FadingTimerHandle, LoadingLevelDelegate, HeldTimerDuration, false);
		return;
	}
	IAsyncPlayerControllerInterface::Execute_TeleportToPlayerStart(PlayerController, LoadingLevelName);
	IMapInterface::Execute_SetPrologueMode(GetLevelScriptBlueprint(LoadingLevel), loadingMode);
	UGameplayStatics::UnloadStreamLevel(GetOwner(), CurrentLevelName, FLatentActionInfo(), false);
	UGameplayStatics::LoadStreamLevel(GetOwner(), levelName, true, false, FLatentActionInfo());

	StopCameraFade();
	if(GetLevelScriptBlueprint(levelName)->Implements<UMapInterface>())
	{
		IMapInterface::Execute_SetPrologueMode(GetLevelScriptBlueprint(levelName), loadingMode);
	}
	CurrentLevelName = levelName;
	if (loadingMode != ELoadingMode::Prologue)
	{
		RemoveLoadingMap_Implementation(false /*??*/, loadingMode, false);
		return;
	};
	// We need add this as a callback
	IMapInterface::Execute_SetLoadingWidgetVisibility(LoadingLevel, false);
}

void UASyncMapGameModeHelperComponent::RemoveLoadingMap_Implementation(bool lazyLoad, ELoadingMode loadingMode, bool IgnoreFade)
{
	if (!IgnoreFade){StartCameraFade();}

	if (lazyLoad && !CurrentLevel->IsLevelVisible())
	{
		CurrentLevel = UGameplayStatics::GetStreamingLevel(GetOwner(), CurrentLevelName);
		CurrentLevel->SetShouldBeVisible(true);
		LoadingLevelDelegate = FTimerDelegate::CreateUObject(this, &UASyncMapGameModeHelperComponent::RemoveLoadingMap_Implementation, lazyLoad, loadingMode, IgnoreFade);
		GetOwner()->GetWorldTimerManager().SetTimer(LoadingLevelTimerHandle, LoadingLevelDelegate, HeldTimerDuration, false);
		return;
	}

	UGameplayStatics::UnloadStreamLevel(GetOwner(), CurrentLevelName, FLatentActionInfo(), false);

	if (IsCorrectThePawnMode(PlayerController, loadingMode, CurrentLevelName))
	{
		IAsyncPlayerControllerInterface::Execute_ReSpawn(PlayerController, loadingMode == ELoadingMode::MainMenu);
	}

	if (loadingMode == ELoadingMode::CustomLocation)
	{
		IAsyncPlayerControllerInterface::Execute_TeleportToLastTransform(PlayerController /*,... ?*/);
	}
	else
	{
		IAsyncPlayerControllerInterface::Execute_TeleportToPlayerStart(PlayerController, CurrentLevelName);
		IAsyncPlayerControllerInterface::Execute_SaveData(PlayerController, CurrentLevelName, loadingMode);
	}

	if (!IgnoreFade)
	{
		StopCameraFade();
	}

	return;
}

bool UASyncMapGameModeHelperComponent::IsCorrectThePawnMode(APlayerController *playerController, ELoadingMode NewParam, FName levelName)
{
	return ((IAsyncPlayerControllerInterface::Execute_IsMenuPawn(playerController) && levelName == "MainMenu") || levelName == LoadingLevelName);
}

void UASyncMapGameModeHelperComponent::StartCameraFade()
{
	TimeToFadding = GetTimeFadding();
	PlayerController->PlayerCameraManager->StartCameraFade(0.0f, 1.0f, FadeDuration, FadeColor, false, true);
}

void UASyncMapGameModeHelperComponent::StopCameraFade()
{
	TimeToFadding = GetTimeFadding();
	PlayerController->PlayerCameraManager->StartCameraFade(1.0f, 0.0f, FadeDuration, FadeColor, false, true);
}

FDateTime UASyncMapGameModeHelperComponent::GetTimeFadding()
{
	return FDateTime::Now() + FTimespan(FadeDuration);
};

UObject* UASyncMapGameModeHelperComponent::GetLevelScriptBlueprint(FName levelName)
{
	return GetLevelScriptBlueprint(UGameplayStatics::GetStreamingLevel(GetOwner(),levelName));
}

UObject* UASyncMapGameModeHelperComponent::GetLevelScriptBlueprint(TObjectPtr<ULevelStreaming> level)
{
	return reinterpret_cast<UObject*>(level->GetLoadedLevel()->GetLevelScriptBlueprint(false));
}
