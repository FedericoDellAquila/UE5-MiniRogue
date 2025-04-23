#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FMiniRogueEditorModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	static void LaunchPackagedBuild(FMenuBuilder& MenuBuilder);
	static void PackagingTools(FMenuBuilder& MenuBuilder);
};
