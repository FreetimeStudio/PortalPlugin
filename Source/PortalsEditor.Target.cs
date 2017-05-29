// Released under MIT License
// Copyright (c) Pascal Krabbe 2017

using UnrealBuildTool;
using System.Collections.Generic;

public class PortalsEditorTarget : TargetRules
{
	public PortalsEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;

		ExtraModuleNames.AddRange( new string[] { "Portals" } );
	}
}
