// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class SurvivalEditorTarget : TargetRules
{
	public SurvivalEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		bOverrideBuildEnvironment = true;

		ExtraModuleNames.AddRange( new string[] { "Survival" } );
		GlobalDefinitions.Add("PLATFORM_HAS_ASAN_INCLUDE=0");

		bUseUnityBuild = false;
		bUsePCHFiles = true;
	}
}
