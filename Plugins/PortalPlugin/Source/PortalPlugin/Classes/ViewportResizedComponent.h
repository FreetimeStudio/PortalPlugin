// Released under MIT License
// Copyright (c) Pascal Krabbe 2017

#pragma once

#include "Components/ActorComponent.h"
#include "UnrealClient.h"
#include "ViewportResizedComponent.generated.h"

UCLASS( ClassGroup=(Utility), meta=(BlueprintSpawnableComponent) )
class UViewportResizedComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnViewportResized);

	UViewportResizedComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintAssignable, Category="Viewport")
	FOnViewportResized OnViewportResized;

	void ViewportResizedWrapper(FViewport* Viewport, uint32 Num);
};
