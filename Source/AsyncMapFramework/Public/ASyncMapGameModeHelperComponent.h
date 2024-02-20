// Copyright 2023 Gabriel Bustillo del Cuvillo

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/AsyncGameModeComponentInterface.h"
#include "Misc/DateTime.h"
#include "Math/Color.h"
#include "Struct/MapRequestInformation.h"
#include "ASyncMapGameModeHelperComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ASYNCMAPFRAMEWORK_API UASyncMapGameModeHelperComponent : public UActorComponent, public IAsyncGameModeComponentInterface
{
	GENERATED_BODY()

private:
	TObjectPtr<ULevelStreaming> CurrentLevel;
	TObjectPtr<ULevelStreaming> LoadingLevel;
	FName CurrentLevelName;
	FMapRequestInformation MapToLoadInformation;
	FTimerHandle LoadingLevelTimerHandle;
	FTimerDelegate LoadingLevelDelegate;
	FTimerHandle FadingTimerHandle;

	bool isLoading = false;

public:
	// Sets default values for this component's properties
	UASyncMapGameModeHelperComponent();

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	bool DebugUI;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default")
	FName LoadingLevelName = "Loading";

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default", meta = (UIMin = "0", UIMax = "2", ClampMin = "0", ClampMax = "2"))
	float FadeDuration = 0.5f;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default")
	float HeldTimerDuration = 0.1f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	TObjectPtr<APlayerController> PlayerController;


	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default")
	FLinearColor FadeColor;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default")
	FDateTime TimeToFadding;

public:
	// Events

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "AsyncMapHelper")
	void LoadMap(FName LevelName, ELoadingMode loadingMode, bool IgnoreFade);
	virtual void LoadMap_Implementation(FName LevelName, ELoadingMode loadingMode, bool IgnoreFade) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "AsyncMapHelper")
	void RemoveLoadingMap(bool LazyLoad, ELoadingMode loadingMode, bool IgnoreFade);
	virtual void RemoveLoadingMap_Implementation(bool LazyLoad, ELoadingMode loadingMode, bool IgnoreFade) override;

	//	UFUNCTION(BlueprintCallable)
	//	void CheckPointSaveData(FName LastLoadedMap);

	// Functions

	/** Please add a function description */
	//	UFUNCTION(BlueprintCallable)
	//	void LoadFMODSettings();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "AsyncMapHelper")
	void OnSwapPlayerControllers(APlayerController *oldPlayer, APlayerController *newPlayer);
	virtual void OnSwapPlayerControllers_Implementation(APlayerController *oldPlayer, APlayerController *newPlayer) override;

private:
	bool IsCorrectThePawnMode(APlayerController *self2, ELoadingMode NewParam, FName NewParam1);

	void StartCameraFade();

	void StopCameraFade();

	void LoadMap_Implementation_TimeElapsed();

	FDateTime GetTimeFadding();
	
	UFUNCTION()
	void LoadingMapLoaded();
	UFUNCTION()
	void NextMapLoaded();

	UObject* GetLevelScriptBlueprint(FName LevelName);
	UObject* GetLevelScriptBlueprint(TObjectPtr<ULevelStreaming> LevelName);


protected:
	// Called when the game starts
	virtual void BeginPlay() override;
};
