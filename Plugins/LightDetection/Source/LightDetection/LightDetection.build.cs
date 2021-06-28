using UnrealBuildTool;
 
public class LightDetection : ModuleRules
{
	public LightDetection(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine"});
 
		PublicIncludePaths.AddRange(new string[] {"LightDetection/Public"});
		PrivateIncludePaths.AddRange(new string[] {"LightDetection/Private"});
	}
}