// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class SurvivalTarget : TargetRules
{
	public SurvivalTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;

		ExtraModuleNames.AddRange( new string[] { "Survival" } );
		GlobalDefinitions.Add("PLATFORM_HAS_ASAN_INCLUDE=0");

		bUseUnityBuild = false;
		bUsePCHFiles = true;
	}
}
