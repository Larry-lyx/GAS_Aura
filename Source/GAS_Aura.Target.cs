// Copyright GAS Aura Course

using UnrealBuildTool;
using System.Collections.Generic;

public class GAS_AuraTarget : TargetRules
{
	public GAS_AuraTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange( new string[] { "GAS_Aura" } );
	}
}
