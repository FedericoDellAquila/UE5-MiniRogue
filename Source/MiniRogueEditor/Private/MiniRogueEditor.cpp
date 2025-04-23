#include "MiniRogueEditor.h"

#include "LevelEditor.h"
#include "MiniRogueEditorSettings.h"
#include "Utility/Logger.h"

#define LOCTEXT_NAMESPACE "FMiniRogueEditorModule"

void FMiniRogueEditorModule::StartupModule()
{
	// Get Level Editor module
	FLevelEditorModule& LevelEditorModule {FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor")};

	// Create an extender for the menu
	const TSharedPtr<FExtender> MenuExtender {MakeShareable(new FExtender {})};

	// Add your custom menu entry
	MenuExtender->AddMenuBarExtension(
		TEXT("Help"), // Add after "Help"
		EExtensionHook::After,
		nullptr,
		FMenuBarExtensionDelegate::CreateLambda([](FMenuBarBuilder& MenuBarBuilder) -> void
		{
			MenuBarBuilder.AddPullDownMenu(
				FText::FromString("Mini Rogue"), // Menu Tab name
				FText::FromString("Mini Rogue editor functionalities."), // Tooltip description
				FNewMenuDelegate::CreateLambda([](FMenuBuilder& MenuBuilder) -> void
				{
					PackagingTools(MenuBuilder);
				}),
				TEXT("AxonParkTab") // Optional tag name
			);
		})
	);

	// Add the extender to the Level Editor's menu extensibility manager
	LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
}

void FMiniRogueEditorModule::ShutdownModule()
{}

void FMiniRogueEditorModule::LaunchPackagedBuild(FMenuBuilder& MenuBuilder)
{
	FString ExePath {
		FPaths::ConvertRelativePathToFull(
			FPaths::SetExtension(
				FPaths::Combine(
					UMiniRogueEditorSettings::Get()->PackagedBuildDirectoryPath.Path,
					TEXT("Windows"),
					TEXT("MiniRogue")),
				TEXT("exe")))
	};

	// Define the items in the custom pull-down menu here
	MenuBuilder.AddMenuEntry(
		FText::FromString("Launch Packaged Build"),
		FText::FromString(FString::Printf(TEXT("Launch the executable in %s."), *UMiniRogueEditorSettings::Get()->PackagedBuildDirectoryPath.Path)),
		FSlateIcon {FSlateIcon(FAppStyle::GetAppStyleSetName(), "MainFrame.PackageProject")},
		FUIAction
		{
			FExecuteAction::CreateLambda([ExePath]() -> void
			{
				if (FPaths::FileExists(ExePath) == false)
				{
					LOG_WARNING("Executable not found at path: %s", *ExePath)
				}

				UE_LOG(LogTemp, Log, TEXT("Launching executable: %s"), *ExePath);
				FPlatformProcess::CreateProc(
					*ExePath,
					nullptr, // Optional command line arguments
					true, // bLaunchDetached
					false, // bLaunchHidden
					false, // bLaunchReallyHidden
					nullptr, // Out process ID
					0, // Priority
					nullptr, // Optional working directory
					nullptr // Pipe handles
				);
			})
		},
		TEXT("UpdateDevAuthenticationToken") // Optional tag name
	);
}

void FMiniRogueEditorModule::PackagingTools(FMenuBuilder& MenuBuilder)
{
	MenuBuilder.BeginSection(NAME_None, LOCTEXT("Learning Framework", "Learning Framework"));

	LaunchPackagedBuild(MenuBuilder);

	MenuBuilder.EndSection();
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FMiniRogueEditorModule, MiniRogueEditor)
