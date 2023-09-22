// Copyright 2023 Gabriel Bustillo del Cuvillo

#include "ASyncMapGameModeHelperComponent.h"

#include "CompatibilityLayer/Macros.h"

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
	//	LoadingLevel->OnLevelLoaded.AddDynamic(this, &UASyncMapGameModeHelperComponent::LoadingMapLoaded);

	Super::BeginPlay();
}

// Uhhh, We are making a Component... Maybe We could bind this function to the event in the *ACTUAL* PlayerController
void UASyncMapGameModeHelperComponent::OnSwapPlayerControllers_Implementation(APlayerController *oldPlayer, APlayerController *newPlayer)
{
	PlayerController = newPlayer;
}

void UASyncMapGameModeHelperComponent::LoadMap_Implementation(FName levelName, ELoadingMode loadingMode, bool IgnoreFade)
{
#ifdef WITH_EDITOR
	if (!(PlayerController->Implements<UAsyncPlayerControllerInterface>() ||
		  GetLevelScriptBlueprint(LoadingLevel)->Implements<UMapInterface>()))
	{
		UE_LOG(LogTemp, Error, TEXT("[%f][%s][%s]: PlayerController and/or Loading Map REQUIRES have their respective interfaces, Check their classes"),
			   GetWorld()->TimeSeconds, *GetName(), *FString(__PRETTY_FUNCTION__), *levelName.ToString(), *MapToLoadInformation.levelName.ToString());
	return;
	}
#endif

	if (isLoading)
	{
		UE_LOG(LogTemp, Warning, TEXT("[%f][%s][%s]: It was requested load a Map while It was loading other\nAttempt To Load: %s\tLoading: %s"),
			   GetWorld()->TimeSeconds, *GetName(), *FString(__PRETTY_FUNCTION__), *levelName.ToString(), *MapToLoadInformation.levelName.ToString());

		return;
	}
	if (!IgnoreFade)
	{
		StartCameraFade();
	}

	isLoading = true;
	// Starts Loading the Loading Map
	LoadingLevel->LevelTransform = UGameplayStatics::GetStreamingLevel(GetOwner(), levelName)->LevelTransform;

	MapToLoadInformation = FMapRequestInformation(levelName, loadingMode, IgnoreFade);
	UGameplayStatics::LoadStreamLevel(GetOwner(), LoadingLevelName, true, false,
									  FLatentActionInfo(0 /*No Idea*/, FCString::Atoi(*FGuid::NewGuid().ToString(EGuidFormats::Digits)), *FString("LoadingMapLoaded"), this));
}

void UASyncMapGameModeHelperComponent::LoadMap_Implementation_TimeElapsed()
{

	// Checking If Fadding Animation is ended
	if (PlayerController == nullptr || (!MapToLoadInformation.IgnoreFade && TimeToFadding < FDateTime::Now()))
	{
		LoadingLevelDelegate = FTimerDelegate::CreateUObject(this, &UASyncMapGameModeHelperComponent::LoadMap_Implementation_TimeElapsed);
		GetOwner()->GetWorldTimerManager().SetTimer(FadingTimerHandle, LoadingLevelDelegate, HeldTimerDuration, false);
		return;
	}

	if (!(CurrentLevelName == FName("None") || CurrentLevelName == LoadingLevelName))
	{
		UGameplayStatics::UnloadStreamLevel(GetOwner(), CurrentLevelName, FLatentActionInfo(), false);
	}

	UGameplayStatics::LoadStreamLevel(GetOwner(), MapToLoadInformation.levelName, ! (MapToLoadInformation.loadingMode == ELoadingMode::Prologue), false, FLatentActionInfo(0 /*No Idea*/, FCString::Atoi(*FGuid::NewGuid().ToString(EGuidFormats::Digits)), *FString("NextMapLoaded"), this));

	StopCameraFade();
}

void UASyncMapGameModeHelperComponent::LoadingMapLoaded()
{
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
	CurrentLevel = UGameplayStatics::GetStreamingLevel(GetOwner(), CurrentLevelName);

	RemoveLoadingMap_Implementation(false /*??*/, MapToLoadInformation.loadingMode, MapToLoadInformation.IgnoreFade);
}

void UASyncMapGameModeHelperComponent::RemoveLoadingMap_Implementation(bool lazyLoad, ELoadingMode loadingMode, bool ignoreFade)
{
	if (!ignoreFade)
	{
		StartCameraFade();
	}

	if (lazyLoad && !CurrentLevel->IsLevelVisible())
	{
		CurrentLevel->SetShouldBeVisible(true);
		LoadingLevelDelegate = FTimerDelegate::CreateUObject(this, &UASyncMapGameModeHelperComponent::RemoveLoadingMap_Implementation, lazyLoad, loadingMode, ignoreFade);
		GetOwner()->GetWorldTimerManager().SetTimer(LoadingLevelTimerHandle, LoadingLevelDelegate, HeldTimerDuration, false);
		return;
	}

	if (GetLevelScriptBlueprint(CurrentLevelName)->Implements<UMapInterface>())
	{
		IMapInterface::Execute_SetPrologueMode(GetLevelScriptBlueprint(CurrentLevelName), MapToLoadInformation.loadingMode);
	}

	UGameplayStatics::UnloadStreamLevel(GetOwner(), LoadingLevelName, FLatentActionInfo(), false);

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
	isLoading = false;
	return;
}

bool UASyncMapGameModeHelperComponent::IsCorrectThePawnMode(APlayerController *playerController, ELoadingMode NewParam, FName levelName)
{
	return ( ! (IAsyncPlayerControllerInterface::Execute_IsMenuPawn(playerController) && levelName == "MainMenu") || levelName == LoadingLevelName);
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
	return reinterpret_cast<UObject *>(level->GetLevelScriptActor());
}
