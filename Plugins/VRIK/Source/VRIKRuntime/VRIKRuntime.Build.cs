// Copyright 2015-2016 Alexander Shatalov, Inc. All Rights Reserved.

using UnrealBuildTool;

public class VRIKRuntime : ModuleRules
{
    public VRIKRuntime(TargetInfo Target)
    {
        PublicIncludePaths.AddRange(
            new string[] 
            {
                "VRIKRuntime/Public",
            }
            );
        PrivateIncludePaths.AddRange(
            new string[] 
            {
                "VRIKRuntime/Private",
            }
            );
        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "LevelSequence",
                "AnimGraphRuntime",
            }
            );
        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Slate",
                "SlateCore",
                "Engine",
                "MovieScene",
                "MovieSceneTracks",
            }
            );
        if (UEBuildConfiguration.bBuildEditor)
        {
            PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "UnrealEd",
                "EditorStyle",
            }
            );
        }
    }
}