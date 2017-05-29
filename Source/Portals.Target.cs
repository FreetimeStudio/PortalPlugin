// Released under MIT License
// Copyright (c) Pascal Krabbe 2017

using UnrealBuildTool;
using System.Collections.Generic;

public class PortalsTarget : TargetRules
{
	public PortalsTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;

		ExtraModuleNames.AddRange( new string[] { "Portals" } );
	}
}
