// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ASyncMapHelperComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ASYNCMAPFRAMEWORK_API UASyncMapHelperComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UASyncMapHelperComponent();

	ULevelStreaming* CurrentLevel;

	ULevelStreaming* LoadingLevel;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Default")
	TObjectPtr<USceneComponent> DefaultSceneRoot;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default", meta=(MultiLine="true"))
	FName CurrentLevelName;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default", meta=(MultiLine="true"))
	bool DebugUI;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default", meta=(MultiLine="true"))
	TObjectPtr<ABP_PlayerController_C> PlayerController;

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

	UFUNCTION(BlueprintCallable)
	void RemoveLoadingMap(bool LazyLoad, ELoadingMode loadingMode, bool IgnoreFade);

	UFUNCTION(BlueprintCallable)
	void CheckPointSaveData(FName LastLoadedMap);

	// Functions

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void LoadFMODSettings();

	/** Please add a function description */
	UFUNCTION(BlueprintPure)
	bool IsCorrectThePawnMode(ABP_PlayerController_C* self2, TEnumAsByte<ELoadingMode> NewParam, FName NewParam1);

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
