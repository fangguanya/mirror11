// Some copyright should be here...

using UnrealBuildTool;

public class VRIKEditor : ModuleRules
{
	public VRIKEditor(TargetInfo Target)
	{
		PublicIncludePaths.AddRange(
			new string[] 
            {
				"VRIKEditor/Public",
			}
			);
        PrivateIncludePaths.AddRange(
			new string[] 
            {
				"VRIKEditor/Private",
			}
			);
        PublicDependencyModuleNames.AddRange(
			new string[]
			{
                "PropertyEditor",
                "EditorStyle",
                "LevelSequence",
                "VRIKRuntime",
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
                "InputCore",
                "UnrealEd",
                "AnimGraph",
                "BlueprintGraph",
			}
			);
	}
}
