// Released under MIT License
// Copyright (c) Pascal Krabbe 2017

#include "IPortalPlugin.h"
#include "ViewportResizedComponent.h"

//Thanks to https://answers.unrealengine.com/questions/94342/how-to-get-current-screen-sizeresolution.html

// Sets default values for this component's properties
UViewportResizedComponent::UViewportResizedComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UViewportResizedComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!GEngine)
	{
		return;
	}
	
	auto GViewport = GEngine->GameViewport;
	if(!GViewport)
	{
		return;
	}


	auto Viewport = GViewport->Viewport;
	if (!Viewport)
	{
		return;
	}
	
	Viewport->ViewportResizedEvent.AddUObject(this, &UViewportResizedComponent::ViewportResizedWrapper);
}

void UViewportResizedComponent::ViewportResizedWrapper(FViewport* Viewport, uint32 Num)
{
	if(OnViewportResized.IsBound())
	{
		OnViewportResized.Broadcast();
	}
}

