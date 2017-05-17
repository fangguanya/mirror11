// Some copyright should be here...

using UnrealBuildTool;

public class ARigEditor : ModuleRules
{
	public ARigEditor(TargetInfo Target)
	{
		PublicIncludePaths.AddRange(
			new string[] 
            {
				"ARigEditor/Public",
			}
			);
        PrivateIncludePaths.AddRange(
			new string[] 
            {
				"ARigEditor/Private",
			}
			);
        PublicDependencyModuleNames.AddRange(
			new string[]
			{
                "PropertyEditor",
                "EditorStyle",
                "LevelSequence",
                "ARigRuntime",
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
