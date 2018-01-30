// Released under MIT License
// Copyright (c) Pascal Krabbe 2017

#include "IPortalPlugin.h"
#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"



class FPortalPlugin : public IPortalPlugin
{
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

IMPLEMENT_MODULE( FPortalPlugin, PortalPlugin )



void FPortalPlugin::StartupModule()
{
	// This code will execute after your module is loaded into memory (but after global variables are initialized, of course.)
}


void FPortalPlugin::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}



