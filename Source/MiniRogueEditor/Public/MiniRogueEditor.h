#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FMiniRogueEditorModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	static void SetupEditorTabs();
	static void LaunchPackagedBuildButton(FMenuBuilder& MenuBuilder);
	static void ClearPackagedFolder(const FString& PackagedProjectPath);
	static void PackagedProject(const FString& Configuration, const FString& PackagedProjectPath, const FString& ExtraParameters = TEXT(""));
	static void PackagedProjectDebug(FMenuBuilder& MenuBuilder);
	static void PackagedProjectRelease(FMenuBuilder& MenuBuilder);
	static void PackagingToolsSection(FMenuBuilder& MenuBuilder);
	static void PackageProjectButton(FMenuBuilder& MenuBuilder);
};
