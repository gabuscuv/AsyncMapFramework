// Copyright 2023 Gabriel Bustillo del Cuvillo

#include "ASyncMapGameModeHelperComponent.h"

#include "TimerManager.h"
#include "Misc/Guid.h"
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
	LoadingLevel->OnLevelLoaded.AddDynamic(this, &UASyncMapGameModeHelperComponent::LoadingMapLoaded);

	Super::BeginPlay();
}

// Uhhh, We are making a Component... Maybe We could bind this function to the event in the *ACTUAL* PlayerController
void UASyncMapGameModeHelperComponent::OnSwapPlayerControllers(APlayerController *oldPlayer, APlayerController *newPlayer)
{
	PlayerController = newPlayer;
}

void UASyncMapGameModeHelperComponent::LoadMap_Implementation(FName levelName, ELoadingMode loadingMode, bool IgnoreFade)
{
	if (!IgnoreFade)
	{
		StartCameraFade();
	}
	// Starts Loading the Loading Map
	LoadingLevel->LevelTransform = UGameplayStatics::GetStreamingLevel(GetOwner(), levelName)->LevelTransform;

	MapToLoadInformation = FMapRequestInformation(levelName, loadingMode, IgnoreFade);
	UGameplayStatics::LoadStreamLevel(GetOwner(), LoadingLevelName, true, false, FLatentActionInfo());
}

void UASyncMapGameModeHelperComponent::LoadMap_Implementation_TimeElapsed()
{
		UE_LOG(LogTemp,Warning,TEXT("HELLO2"));

	// Checking If Fadding Animation is ended
	if (PlayerController == nullptr || (!MapToLoadInformation.IgnoreFade && TimeToFadding < FDateTime::Now()))
	{
		LoadingLevelDelegate = FTimerDelegate::CreateUObject(this, &UASyncMapGameModeHelperComponent::LoadMap_Implementation_TimeElapsed);
		GetOwner()->GetWorldTimerManager().SetTimer(FadingTimerHandle, LoadingLevelDelegate, HeldTimerDuration, false);
		return;
	}

	UGameplayStatics::UnloadStreamLevel(GetOwner(), CurrentLevelName, FLatentActionInfo(), false);
	UGameplayStatics::LoadStreamLevel(GetOwner(), MapToLoadInformation.levelName, true, false,
									  FLatentActionInfo(0 /*No Idea*/, FCString::Atoi(*FGuid::NewGuid().ToString(EGuidFormats::Digits)), *FString("NextMapLoaded"), this));

	StopCameraFade();
}

void UASyncMapGameModeHelperComponent::LoadingMapLoaded()
{
	UE_LOG(LogTemp,Warning,TEXT("HELLO"));
	// Teleport the Player
	IAsyncPlayerControllerInterface::Execute_TeleportToPlayerStart(PlayerController, LoadingLevelName);
	StopCameraFade();

	LoadMap_Implementation_TimeElapsed();
}

void UASyncMapGameModeHelperComponent::NextMapLoaded()
{
	if (MapToLoadInformation.loadingMode == ELoadingMode::Prologue)
	{
		IMapInterface::Execute_SetLoadingWidgetVisibility(LoadingLevel, false);
		return;
	};

	CurrentLevelName = MapToLoadInformation.levelName;

	RemoveLoadingMap_Implementation(false /*??*/, MapToLoadInformation.loadingMode, false);
}

void UASyncMapGameModeHelperComponent::RemoveLoadingMap_Implementation(bool lazyLoad, ELoadingMode loadingMode, bool ignoreFade)
{
	if (!ignoreFade)
	{
		StartCameraFade();
	}

	if (lazyLoad && !CurrentLevel->IsLevelVisible())
	{
		CurrentLevel = UGameplayStatics::GetStreamingLevel(GetOwner(), CurrentLevelName);
		CurrentLevel->SetShouldBeVisible(true);
		LoadingLevelDelegate = FTimerDelegate::CreateUObject(this, &UASyncMapGameModeHelperComponent::RemoveLoadingMap_Implementation, lazyLoad, loadingMode, ignoreFade);
		GetOwner()->GetWorldTimerManager().SetTimer(LoadingLevelTimerHandle, LoadingLevelDelegate, HeldTimerDuration, false);
		return;
	}

	if (GetLevelScriptBlueprint(CurrentLevelName)->Implements<UMapInterface>())
	{
		IMapInterface::Execute_SetPrologueMode(GetLevelScriptBlueprint(CurrentLevelName), MapToLoadInformation.loadingMode);
	}

	UGameplayStatics::UnloadStreamLevel(GetOwner(), CurrentLevelName, FLatentActionInfo(), false);

	if (IsCorrectThePawnMode(PlayerController, loadingMode, CurrentLevelName))
	{
		IAsyncPlayerControllerInterface::Execute_ReSpawn(PlayerController, MapToLoadInformation.loadingMode == ELoadingMode::MainMenu);
	}

	if (MapToLoadInformation.loadingMode == ELoadingMode::CustomLocation)
	{
		IAsyncPlayerControllerInterface::Execute_TeleportToLastTransform(PlayerController /*,... ?*/);
	}
	else
	{
		IAsyncPlayerControllerInterface::Execute_TeleportToPlayerStart(PlayerController, CurrentLevelName);
		IAsyncPlayerControllerInterface::Execute_SaveData(PlayerController, CurrentLevelName, MapToLoadInformation.loadingMode);
	}

	if (!ignoreFade)
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

UObject *UASyncMapGameModeHelperComponent::GetLevelScriptBlueprint(FName levelName)
{
	return GetLevelScriptBlueprint(UGameplayStatics::GetStreamingLevel(GetOwner(), levelName));
}

UObject *UASyncMapGameModeHelperComponent::GetLevelScriptBlueprint(TObjectPtr<ULevelStreaming> level)
{
	return reinterpret_cast<UObject *>(level->GetLoadedLevel()->GetLevelScriptBlueprint(false));
}
