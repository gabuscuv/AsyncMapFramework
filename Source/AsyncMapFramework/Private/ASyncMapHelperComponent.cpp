// Fill out your copyright notice in the Description page of Project Settings.


#include "Kismet/GameplayStatics.h"
#include "ASyncMapHelperComponent.h"

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

void UASyncMapHelperComponent::OnSwapPlayerControllers(APlayerController * oldPlayer, APlayerController * newPlayer)
{
	PlayerController = newPlayer;

}

void UASyncMapHelperComponent::LoadMap(FName LevelName, ELoadingMode loadingMode, bool IgnoreFade)
{
	if(!IgnoreFade){StartCameraFade();}
	auto nextToLoadMap = GetStreamingLevel(LevelName);
	LoadingLevel->LevelTransform = nextToLoadMap->LevelTransform;
	GameplayStatics::LoadStreamLevel(GetParent(), LoadingLevel, true, false);
	// (Macro) IsFaded();
	// PlayerController-> (Interface) TeleportToPlayerStart("Loading");
	// LoadingLevel-> (Interface) SetPrologueMode(loadingMode);
	GameplayStatics::UnloadStreamLevel(GetParent(), CurrentLevelName);
	// (Macro) RemoveCameraFade()
	// nextToLoadMap-> (Interface) SetPrologueMode(loadingMode);
	CurrentLevelName = LevelName;
	if (loadingMode != ELoadingMode::Prologue)
	{
		RemoveLoadingLevel(CurrentLevelName,loadingMode,false); return;
	}	 
	// LoadingLevel-> (Interface) SetLoadingWidgetLevel(false);
}

void UASyncMapHelperComponent::RemoveLoadingLevel(bool LazyLoad, ELoadingMode loadingMode, bool IgnoreFade)
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

	}
}

bool UASyncMapHelperComponent::IsCorrectThePawnMode(ABP_PlayerController_C *playerController, TEnumAsByte<ELoadingMode> NewParam, FName LevelName)
{
    return (playerController->GetIsPlayerPawn() && LevelName == "MainMenu") || LevelName == "Loading";
}