// Released under MIT License
// Copyright (c) Pascal Krabbe 2017

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "PortalFunctionLibrary.generated.h"

/**
 * Documentation: Enable global clip plane for capture actors
 * 
 */
UCLASS()
class UPortalFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Portal")
	static float GetFOVForCaptureComponents(const APlayerController* ForPlayerController = nullptr);

	UFUNCTION(BlueprintCallable, Category = "Portal")
	static void UpdatePortalVPMParameters(USceneCaptureComponent2D* CaptureComponent, UMaterialInstanceDynamic* MaterialInstance, const FTransform& CameraTransform);

	UFUNCTION(BlueprintCallable, Category = "Portal")
	static void UpdateCaptureComponentTransform(USceneComponent* CaptureComponent, const FTransform& CameraTransform, const USceneComponent* SourcePortal, const USceneComponent* TargetPortal);

	UFUNCTION(BlueprintCallable, Category = "Portal")
	static void UpdateCaptureComponentWithTransforms(USceneComponent* CaptureComponent, const FTransform& CameraTransform, const FTransform& SourcePortal, const FTransform& TargetPortal);

	UFUNCTION(BlueprintCallable, Category = "Portal")
	static void UpdateCaptureComponentClipplane(USceneCaptureComponent2D* CaptureComponent, const USceneComponent* TargetPortal, float ClipOffset = -5.0f);

	UFUNCTION(BlueprintCallable, Category = "Portal")
	static void UpdateCaptureComponentClipPlaneTransform(USceneCaptureComponent2D* CaptureComponent, const FTransform& TargetPortal, float ClipOffset = -5.0f);

	UFUNCTION(BlueprintCallable, Category = "Portal")
	static void UpdateFullPortalState(USceneCaptureComponent2D* CaptureComponent, UMaterialInstanceDynamic* MaterialInstance, const FTransform& CameraTransform, const USceneComponent* SourcePortal, const USceneComponent* TargetPortal, float ClipOffset = -5.0f);


	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Portal")
	static FVector GetLocationMappedToTarget(const FVector& Location, const USceneComponent* Source, const USceneComponent* Target);

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Portal")
	static FVector GetLocationMappedToTargetTransform(const FVector& Location, const FTransform& Source, const FTransform& Target);

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Portal")
	static FVector GetDirectionMappedToTarget(const FVector& Direction, const USceneComponent* Source, const USceneComponent* Target);

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Portal")
	static FVector GetDirectionMappedToTargetTransform(const FVector& Direction, const FTransform& Source, const FTransform& Target);

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Portal")
	static FRotator GetRotationMappedToTarget(const FRotator& Rotation, const USceneComponent* Source, const USceneComponent* Target);

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Portal")
	static FRotator GetRotationMappedToTargetTransform(const FRotator& Rotation, const FTransform& Source, const FTransform& Target);

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Portal")
	static FQuat GetQuatMappedToTarget(const FQuat& Rotation, const USceneComponent* Source, const USceneComponent* Target);

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Portal")
	static FQuat GetQuatMappedToTargetTransform(const FQuat& Rotation, const FTransform& Source, const FTransform& Target);
};
