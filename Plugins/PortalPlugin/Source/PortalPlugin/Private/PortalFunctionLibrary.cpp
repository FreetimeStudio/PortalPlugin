// Released under MIT License
// Copyright (c) Pascal Krabbe 2017

#include "IPortalPlugin.h"
#include "PortalFunctionLibrary.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Runtime/HeadMountedDisplay/Public/IHeadMountedDisplay.h"

//Credit goes to AgentMilkshake1 https://answers.unrealengine.com/questions/234597/screenspace-portals-on-vr.html
float UPortalFunctionLibrary::GetFOVForCaptureComponents(const APlayerController* ForPlayerController)
{
	float ResultFOV = 90.0f;

	if (ForPlayerController != nullptr)
	{
		if (ForPlayerController->PlayerCameraManager != nullptr)
		{
			ResultFOV = ForPlayerController->PlayerCameraManager->GetFOVAngle();
		}
	}

	if(!GEngine)
	{
		return ResultFOV;
	}

	// FOV changes when we have a VR Headset enabled
	if (GEngine->HMDDevice.IsValid() && GEngine->IsStereoscopic3D())
	{
		float HFOV, VFOV;
		GEngine->HMDDevice->GetFieldOfView(HFOV, VFOV);
		if (VFOV > 0 && HFOV > 0)
		{
			ResultFOV = FMath::Max(HFOV, VFOV);
			// AspectRatio won't be used until bConstrainAspectRatio is set to true,
			// but it doesn't really matter since HMD calcs its own projection matrix.
			//OutViewInfo.AspectRatio = HFOV / VFOV;
			//OutViewInfo.bConstrainAspectRatio = true;
		}
	}

	return ResultFOV;
}

//Credit goes to AgentMilkshake1 https://answers.unrealengine.com/questions/234597/screenspace-portals-on-vr.html
void UPortalFunctionLibrary::UpdatePortalVPMParameters(USceneCaptureComponent2D* CaptureComponent, UMaterialInstanceDynamic* MaterialInstance, const FTransform& CameraTransform)
{
	if (!IsValid(CaptureComponent) || !IsValid(MaterialInstance))
	{
		return;
	}

	if(!IsValid(CaptureComponent->TextureTarget))
	{
		return;
	}

	float CaptureSizeX = CaptureComponent->TextureTarget->GetSurfaceWidth();
	float CaptureSizeY = CaptureComponent->TextureTarget->GetSurfaceHeight();
	const FTransform& Transform = CameraTransform;
	FMatrix ViewMatrix = Transform.ToInverseMatrixWithScale();
	FVector ViewLocation = Transform.GetTranslation();
	// swap axis st. x=z,y=x,z=y (unreal coord space) so that z is up
	ViewMatrix = ViewMatrix * FMatrix(
		FPlane(0, 0, 1, 0),
		FPlane(1, 0, 0, 0),
		FPlane(0, 1, 0, 0),
		FPlane(0, 0, 0, 1));
	const float FOV = CaptureComponent->FOVAngle * (float)PI / 360.0f;

	// Build projection matrix
	float XAxisMultiplier;
	float YAxisMultiplier;

	if (CaptureSizeX > CaptureSizeY)
	{
		// if the viewport is wider than it is tall
		XAxisMultiplier = 1.0f;
		YAxisMultiplier = CaptureSizeX / CaptureSizeY;
	}
	else
	{
		// if the viewport is taller than it is wide
		XAxisMultiplier = CaptureSizeY / CaptureSizeX;
		YAxisMultiplier = 1.0f;
	}

	FMatrix ProjectionMatrix = FReversedZPerspectiveMatrix(
		FOV,
		FOV,
		XAxisMultiplier,
		YAxisMultiplier,
		10,
		1000
		);

	const FMatrix ViewProjectionMatrix = ViewMatrix * ProjectionMatrix;

	FVector Xaxis = ViewProjectionMatrix.GetColumn(0);
	FVector Yaxis = ViewProjectionMatrix.GetColumn(1);
	FVector Zaxis = ViewProjectionMatrix.GetColumn(2);
	FVector Waxis = ViewProjectionMatrix.GetColumn(3);

	float XaxisW = ViewProjectionMatrix.M[3][0];
	float YaxisW = ViewProjectionMatrix.M[3][1];
	float ZaxisW = ViewProjectionMatrix.M[3][2];
	float WaxisW = ViewProjectionMatrix.M[3][3];

	MaterialInstance->SetVectorParameterValue("PortalVPM_Xaxis", FLinearColor(Xaxis.X, Xaxis.Y, Xaxis.Z, XaxisW));
	MaterialInstance->SetVectorParameterValue("PortalVPM_Yaxis", FLinearColor(Yaxis.X, Yaxis.Y, Yaxis.Z, YaxisW));
	MaterialInstance->SetVectorParameterValue("PortalVPM_Zaxis", FLinearColor(Zaxis.X, Zaxis.Y, Zaxis.Z, ZaxisW));
	MaterialInstance->SetVectorParameterValue("PortalVPM_Waxis", FLinearColor(Waxis.X, Waxis.Y, Waxis.Z, WaxisW));
}

void UPortalFunctionLibrary::UpdateCaptureComponentTransform(USceneComponent* CaptureComponent, const FTransform& CameraTransform, const USceneComponent* SourcePortal, const USceneComponent* TargetPortal)
{
	if(!CaptureComponent)
	{
		return;
	}

	auto TargetLocation = GetLocationMappedToTarget(CameraTransform.GetLocation(), SourcePortal, TargetPortal);
	auto TargetRotation = GetQuatMappedToTarget(CameraTransform.GetRotation(), SourcePortal, TargetPortal);

	CaptureComponent->SetWorldLocationAndRotation(TargetLocation, TargetRotation);
}

void UPortalFunctionLibrary::UpdateCaptureComponentWithTransforms(USceneComponent* CaptureComponent, const FTransform& CameraTransform, const FTransform& SourcePortal, const FTransform& TargetPortal)
{
	if (!CaptureComponent)
	{
		return;
	}

	auto TargetLocation = GetLocationMappedToTargetTransform(CameraTransform.GetLocation(), SourcePortal, TargetPortal);
	auto TargetRotation = GetQuatMappedToTargetTransform(CameraTransform.GetRotation(), SourcePortal, TargetPortal);

	CaptureComponent->SetWorldLocationAndRotation(TargetLocation, TargetRotation);
}

void UPortalFunctionLibrary::UpdateCaptureComponentClipplane(USceneCaptureComponent2D* CaptureComponent, const USceneComponent* TargetPortal, float ClipOffset)
{
	if(!CaptureComponent)
	{
		return;
	}

	auto TargetComponent = TargetPortal ? TargetPortal : CaptureComponent;
	//CaptureComponent->ClipPlaneNormal = TargetComponent->GetForwardVector();
	//CaptureComponent->ClipPlaneBase = TargetComponent->GetComponentLocation() + TargetComponent->GetForwardVector()*ClipOffset;

	UpdateCaptureComponentClipPlaneTransform(CaptureComponent, TargetComponent->GetComponentTransform(), ClipOffset);
}

void UPortalFunctionLibrary::UpdateCaptureComponentClipPlaneTransform(USceneCaptureComponent2D* CaptureComponent, const FTransform& TargetPortal, float ClipOffset)
{
	if (!CaptureComponent)
	{
		return;
	}
	
	CaptureComponent->ClipPlaneNormal = TargetPortal.TransformVector(FVector::ForwardVector);
	CaptureComponent->ClipPlaneBase = TargetPortal.GetLocation() + CaptureComponent->ClipPlaneNormal *ClipOffset;
}

void UPortalFunctionLibrary::UpdateFullPortalState(USceneCaptureComponent2D* CaptureComponent, UMaterialInstanceDynamic* MaterialInstance, const FTransform& CameraTransform, const USceneComponent* SourcePortal, const USceneComponent* TargetPortal, float ClipOffset)
{
	if(!CaptureComponent)
	{
		return;
	}

	UpdateCaptureComponentTransform(CaptureComponent, CameraTransform, SourcePortal, TargetPortal);
	if(CaptureComponent->bEnableClipPlane)
	{
		UpdateCaptureComponentClipplane(CaptureComponent, TargetPortal, ClipOffset);
	}
	UpdatePortalVPMParameters(CaptureComponent, MaterialInstance, CameraTransform);
}

FVector UPortalFunctionLibrary::GetLocationMappedToTarget(const FVector& Location, const USceneComponent* Source, const USceneComponent* Target)
{
	if(!Source || !Target)
	{
		return Location;
	}

	const FTransform& SourceTransform = Source->GetComponentTransform();
	const FTransform& TargetTransform = Target->GetComponentTransform();

	return GetLocationMappedToTargetTransform(Location,SourceTransform,TargetTransform);
}

FVector UPortalFunctionLibrary::GetLocationMappedToTargetTransform(const FVector& Location, const FTransform& Source, const FTransform& Target)
{
	return Target.TransformPosition(
		Source.InverseTransformPosition(Location));
}

FVector UPortalFunctionLibrary::GetDirectionMappedToTarget(const FVector& Direction, const USceneComponent* Source, const USceneComponent* Target)
{
	if (!Source || !Target)
	{
		return Direction;
	}

	const FTransform& SourceTransform = Source->GetComponentTransform();
	const FTransform& TargetTransform = Target->GetComponentTransform();

	return GetDirectionMappedToTargetTransform(Direction,SourceTransform,TargetTransform);
}

FVector UPortalFunctionLibrary::GetDirectionMappedToTargetTransform(const FVector& Direction, const FTransform& Source, const FTransform& Target)
{
	return Target.TransformVector(
		Source.InverseTransformVector(Direction));
}

FRotator UPortalFunctionLibrary::GetRotationMappedToTarget(const FRotator& Rotation, const USceneComponent* Source, const USceneComponent* Target)
{
	return GetQuatMappedToTarget(Rotation.Quaternion(), Source, Target).Rotator();
}

FRotator UPortalFunctionLibrary::GetRotationMappedToTargetTransform(const FRotator& Rotation, const FTransform& Source, const FTransform& Target)
{
	return GetQuatMappedToTargetTransform(Rotation.Quaternion(), Source, Target).Rotator();
}

FQuat UPortalFunctionLibrary::GetQuatMappedToTarget(const FQuat& Rotation, const USceneComponent* Source, const USceneComponent* Target)
{
	if (!Source || !Target)
	{
		return Rotation;
	}

	const FTransform& SourceTransform = Source->GetComponentTransform();
	const FTransform& TargetTransform = Target->GetComponentTransform();

	return GetQuatMappedToTargetTransform(Rotation,SourceTransform,TargetTransform);
}

FQuat UPortalFunctionLibrary::GetQuatMappedToTargetTransform(const FQuat& Rotation, const FTransform& Source, const FTransform& Target)
{
	return Target.GetRotation() *
		(Source.GetRotation().Inverse() * Rotation);
}
