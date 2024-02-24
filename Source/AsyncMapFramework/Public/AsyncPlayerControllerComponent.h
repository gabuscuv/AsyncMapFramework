// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/AsyncPlayerControllerInterface.h"
#include "AsyncPlayerControllerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ASYNCMAPFRAMEWORK_API UAsyncPlayerControllerComponent : public UActorComponent, public IAsyncPlayerControllerInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAsyncPlayerControllerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	TObjectPtr<class APlayerController> PlayerController;
	FDateTime GetTimeFadding();

	FVector GetPlayerCoords(FName name);
	FVector HMDTeleportAdjustment(AActor *targetActor);

protected:

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default", meta = (UIMin = "0", UIMax = "2", ClampMin = "0", ClampMax = "2"))
	float FadeDuration = 0.5f;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default")
	float HeldTimerDuration = 0.1f;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default")
	FLinearColor FadeColor;

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "AsyncMapHelper")
	FDateTime StartCameraFade();
	FDateTime StartCameraFade_Implementation();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "AsyncMapHelper")
	FDateTime StopCameraFade();
	FDateTime StopCameraFade_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "AsyncMapHelper")
	APlayerCameraManager* GetOwnerPlayerCameraManager();
	APlayerCameraManager* GetOwnerPlayerCameraManager_Implementation();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "AsyncMapHelper")
	void TeleportToPlayerStart(FName LevelName);
	void TeleportToPlayerStart_Implementation(FName LevelName);
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "AsyncMapHelper")
    void TeleportToLastTransform();
	void TeleportToLastTransform_Implementation();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "AsyncMapHelper")
	bool IsMenuPawn();
	bool IsMenuPawn_Implementation();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "AsyncMapHelper")
	bool ReSpawn(bool MenuPawn);
	bool ReSpawn_Implementation(bool MenuPawn);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "AsyncMapHelper")
	void LoadSaveData(FTransform LastLoadedMap, TMap<EEvents,bool> & EventsEnabled);
	void LoadSaveData_Implementation(FTransform LastLoadedMap, TMap<EEvents,bool> & EventsEnabled);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "AsyncMapHelper")
	void SaveData(FName LastLoadedMap, ELoadingMode LoadingMode);
	void SaveData_Implementation(FName LastLoadedMap, ELoadingMode LoadingMode);
		
};
