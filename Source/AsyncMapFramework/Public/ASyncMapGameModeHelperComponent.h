// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/AsyncGameModeComponentInterface.h"
#include "ASyncMapGameModeHelperComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ASYNCMAPFRAMEWORK_API UASyncMapGameModeHelperComponent : public UActorComponent, public IAsyncGameModeInterface
{
	GENERATED_BODY()

private:
	ULevelStreaming* CurrentLevel;
	ULevelStreaming* LoadingLevel;
	FName CurrentLevelName;
	TObjectPtr<APlayerController> PlayerController;

public:	
	// Sets default values for this component's properties
	UASyncMapHelperComponent();

	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Default")
	TObjectPtr<USceneComponent> DefaultSceneRoot;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default", meta=(MultiLine="true"))
	bool DebugUI;


	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default", meta=(MultiLine="true", UIMin="", UIMax="2", ClampMin="", ClampMax=""))
	float FadeDuration = 0.1f;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default", meta=(MultiLine="true"))
	FLinearColor FadeColor;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default", meta=(MultiLine="true"))
	FDateTime TimeToFadding;

public:

	// Events

	UFUNCTION(BlueprintCallable)
	void LoadMap(FName LevelName, ELoadingMode loadingMode, bool IgnoreFade);
	virtual void LoadMap_Implementation(FName LevelName, ELoadingMode loadingMode, bool IgnoreFade) override;


	UFUNCTION(BlueprintCallable)
	void RemoveLoadingMap(bool LazyLoad, ELoadingMode loadingMode, bool IgnoreFade);
	void RemoveLoadingMap_Implementation(bool LazyLoad, ELoadingMode loadingMode, bool IgnoreFade);


	UFUNCTION(BlueprintCallable)
	void CheckPointSaveData(FName LastLoadedMap);

	// Functions

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void LoadFMODSettings();

	/** Please add a function description */
	UFUNCTION(BlueprintPure)
	bool IsCorrectThePawnMode(APlayerController* self2, TEnumAsByte<ELoadingMode> NewParam, FName NewParam1);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void StartCameraFade();

	/** Please add a function description */
	UFUNCTION(BlueprintPure)
	FDateTime GetTimeFadding();


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
