#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FTinyRogueEditorModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	static void SetupEditorTabs();
	static void LaunchPackagedBuildButton(FMenuBuilder& MenuBuilder);
	static void ClearPackagedFolder(const FString& PackagedProjectPath);
	static void PackagedProject(const FString& Configuration, const FString& PackagedProjectPath, const FString& ExtraParameters = TEXT(""));
	static void PackagedProjectDevelopment(FMenuBuilder& MenuBuilder);
	static void PackagedProjectShipping(FMenuBuilder& MenuBuilder);
	static void PackagingToolsSection(FMenuBuilder& MenuBuilder);
	static void DevelopmentToolsSection(FMenuBuilder& MenuBuilder);
	static void PackageProjectButton(FMenuBuilder& MenuBuilder);
	static void ShowToDosListButton(FMenuBuilder& MenuBuilder);
};
