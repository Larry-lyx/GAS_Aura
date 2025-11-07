// Copyright GAS Aura Course

using UnrealBuildTool;
using System.Collections.Generic;

public class GAS_AuraEditorTarget : TargetRules
{
	public GAS_AuraEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange( new string[] { "GAS_Aura" } );
	}
}
