#include "MiniRogueEditor.h"
#include "LevelEditor.h"
#include "MiniRogueEditorSettings.h"
#include "HAL/FileManagerGeneric.h"
#include "Utility/Logger.h"

#define LOCTEXT_NAMESPACE "FMiniRogueEditorModule"

void FMiniRogueEditorModule::StartupModule()
{
	SetupEditorTabs();
}

void FMiniRogueEditorModule::ShutdownModule()
{}

void FMiniRogueEditorModule::SetupEditorTabs()
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
					PackagingToolsSection(MenuBuilder);
				}),
				TEXT("MiniRogueTab") // Optional tag name
			);
		})
	);

	// Add the extender to the Level Editor's menu extensibility manager
	LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
}

void FMiniRogueEditorModule::LaunchPackagedBuildButton(FMenuBuilder& MenuBuilder)
{
	const FString ExePath {
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
		FSlateIcon {FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.Launch")},
		FUIAction
		{
			FExecuteAction::CreateLambda([ExePath]() -> void
			{
				if (FPaths::FileExists(ExePath) == false)
				{
					LOG_WARNING("Executable not found at path: {0}", *ExePath)
					return;
				}

				LOG("Launching executable: {0}", ExePath);
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
		TEXT("LaunchPackagedBuild") // Optional tag name
	);
}

void FMiniRogueEditorModule::ClearPackagedFolder(const FString& PackagedProjectPath)
{
	IFileManager& FileManager {FFileManagerGeneric::Get()};
	FileManager.DeleteDirectory(*PackagedProjectPath, false, true);
}

void FMiniRogueEditorModule::PackagedProject(const FString& Configuration, const FString& PackagedProjectPath, const FString& ExtraParameters)
{
	ClearPackagedFolder(PackagedProjectPath);

	const FString RunUATPath {
		FPaths::ConvertRelativePathToFull(FPaths::Combine(
			FPaths::EngineDir(),
			TEXT("Build/BatchFiles/RunUAT.bat")
		))
	};

	const FString ProjectPath {FPaths::ConvertRelativePathToFull(FPaths::GetProjectFilePath())};

	const FString UEExePath {
		FPaths::ConvertRelativePathToFull(FPaths::Combine(
			FPaths::EngineDir(),
			TEXT("Binaries/Win64/UnrealEditor-Win64-DebugGame-Cmd.exe")
		))
	};

	const FString Cmd {
		FString::Printf(
			TEXT("cmd.exe /c \"\"%s\" -ScriptsForProject=\"%s\" Turnkey -command=VerifySdk -platform=Win64 -UpdateIfNeeded -EditorIO -EditorIOPort=7973 ")
			TEXT("-project=\"%s\" BuildCookRun -nop4 -utf8output -nocompileeditor -skipbuildeditor -cook ")
			TEXT("-project=\"%s\" -target=MiniRogue -unrealexe=\"%s\" -platform=Win64 -installed -stage -archive -package -build -pak -iostore -compressed ")
			TEXT("-prereqs -archivedirectory=\"%s\" -clientconfig=%s %s -nocompile -nocompileuat\""),
			*RunUATPath,
			*ProjectPath,
			*ProjectPath,
			*ProjectPath,
			*UEExePath,
			*PackagedProjectPath,
			*Configuration,
			*ExtraParameters
		)
	};

	FPlatformProcess::CreateProc(
		TEXT("cmd.exe"),
		*FString::Printf(TEXT("/c %s"), *Cmd),
		true,
		false,
		false,
		nullptr,
		0,
		nullptr,
		nullptr
	);
}

void FMiniRogueEditorModule::PackagedProjectDebug(FMenuBuilder& MenuBuilder)
{
	const FString PackagedProjectPath {UMiniRogueEditorSettings::Get()->PackagedBuildDirectoryPath.Path};

	MenuBuilder.AddMenuEntry(
		FText::FromString("Debug"),
		FText::FromString(FString::Printf(TEXT("Build the project in Debug configuration at path %s."), *PackagedProjectPath)),
		FSlateIcon(FAppStyle::GetAppStyleSetName(), "Debug"),
		FUIAction(FExecuteAction::CreateLambda([PackagedProjectPath]() -> void
		{
			PackagedProject(TEXT("Development"), PackagedProjectPath);
		}))
	);
}

void FMiniRogueEditorModule::PackagedProjectRelease(FMenuBuilder& MenuBuilder)
{
	const FString PackagedProjectPath {UMiniRogueEditorSettings::Get()->PackagedBuildDirectoryPath.Path};

	MenuBuilder.AddMenuEntry(
		FText::FromString("Release"),
		FText::FromString(FString::Printf(TEXT("Build the project in Release configuration at path %s."), *PackagedProjectPath)),
		FSlateIcon(FAppStyle::GetAppStyleSetName(), "ClassIcon.PlayerController"),
		FUIAction(FExecuteAction::CreateLambda([PackagedProjectPath]() -> void
		{
			PackagedProject(TEXT("Shipping"), PackagedProjectPath, TEXT("-nodebuginfo"));
		}))
	);
}

void FMiniRogueEditorModule::PackagingToolsSection(FMenuBuilder& MenuBuilder)
{
	MenuBuilder.BeginSection(NAME_None, LOCTEXT("Packaged Build Tools", "Packaged Build Tools"));

	LaunchPackagedBuildButton(MenuBuilder);
	PackageProjectButton(MenuBuilder);

	MenuBuilder.EndSection();
}

void FMiniRogueEditorModule::PackageProjectButton(FMenuBuilder& MenuBuilder)
{
	// New: Add a SubMenu for Building
	MenuBuilder.AddSubMenu(
		FText::FromString("Package Project"),
		FText::FromString("Package the project with a specific configuration."),
		FNewMenuDelegate::CreateLambda([](FMenuBuilder& SubMenuBuilder) -> void
		{
			PackagedProjectDebug(SubMenuBuilder);
			PackagedProjectRelease(SubMenuBuilder);
		}),
		false, // bInOpenSubMenuOnClick
		FSlateIcon(FAppStyle::GetAppStyleSetName(), "MainFrame.PackageProject")
	);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FMiniRogueEditorModule, MiniRogueEditor)
