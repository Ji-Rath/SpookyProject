using UnrealBuildTool;
 
public class MoreMovement : ModuleRules
{
	public MoreMovement(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine"});
 
		PublicIncludePaths.AddRange(new string[] {"MoreMovement/Public"});
		PrivateIncludePaths.AddRange(new string[] {"MoreMovement/Private"});
	}
}