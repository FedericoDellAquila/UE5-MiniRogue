#include "TinyRogueEditor.h"
#include "LevelEditor.h"
#include "TinyRogueEditorSettings.h"
#include "HAL/FileManagerGeneric.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Utility/Log.h"

#define LOCTEXT_NAMESPACE "FTinyRogueEditorModule"

void FTinyRogueEditorModule::StartupModule()
{
	SetupEditorTabs();
}

void FTinyRogueEditorModule::ShutdownModule()
{}

void FTinyRogueEditorModule::SetupEditorTabs()
{
	const TSharedPtr<FExtender> MenuExtender {MakeShareable(new FExtender {})};

	MenuExtender->AddMenuBarExtension(
		TEXT("Help"),
		EExtensionHook::After,
		nullptr,
		FMenuBarExtensionDelegate::CreateLambda([](FMenuBarBuilder& MenuBarBuilder) -> void
		{
			MenuBarBuilder.AddPullDownMenu(
				FText::FromString(TEXT("Tiny Rogue")),
				FText::FromString(TEXT("Tiny Rogue editor functionalities.")),
				FNewMenuDelegate::CreateLambda([](FMenuBuilder& MenuBuilder) -> void
				{
					PackagingToolsSection(MenuBuilder);
					DevelopmentToolsSection(MenuBuilder);
				}),
				TEXT("TinyRogueTab")
			);
		})
	);

	FLevelEditorModule& LevelEditorModule {FModuleManager::LoadModuleChecked<FLevelEditorModule>(TEXT("LevelEditor"))};
	LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
}

void FTinyRogueEditorModule::LaunchPackagedBuildButton(FMenuBuilder& MenuBuilder)
{
	const FString ExePath {
		FPaths::ConvertRelativePathToFull(
			FPaths::SetExtension(
				FPaths::Combine(
					UTinyRogueEditorSettings::Get()->PackagedBuildDirectoryPath.Path,
					TEXT("Windows"),
					FApp::GetProjectName()),
				TEXT("exe")))
	};

	MenuBuilder.AddMenuEntry(
		FText::FromString(TEXT("Launch Packaged Build")),
		FText::FromString(FString::Printf(TEXT("Launch the executable in %s."), *FPaths::ConvertRelativePathToFull(UTinyRogueEditorSettings::Get()->PackagedBuildDirectoryPath.Path))),
		FSlateIcon {FSlateIcon(FAppStyle::GetAppStyleSetName(), TEXT("Icons.Launch"))},
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
					nullptr,
					true,
					false,
					false,
					nullptr,
					0,
					nullptr,
					nullptr
				);
			})
		},
		TEXT("LaunchPackagedBuild")
	);
}

void FTinyRogueEditorModule::ClearPackagedFolder(const FString& PackagedProjectPath)
{
	IFileManager& FileManager {FFileManagerGeneric::Get()};
	FileManager.DeleteDirectory(*PackagedProjectPath, false, true);
}

void FTinyRogueEditorModule::PackagedProject(const FString& Configuration, const FString& PackagedProjectPath, const FString& ExtraParameters)
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
			TEXT("-project=\"%s\" -target=%s -unrealexe=\"%s\" -platform=Win64 -installed -stage -archive -package -build -pak -iostore -compressed ")
			TEXT("-prereqs -archivedirectory=\"%s\" -clientconfig=%s %s -nocompile -nocompileuat\""),
			*RunUATPath,
			*ProjectPath,
			*ProjectPath,
			*ProjectPath,
			FApp::GetProjectName(),
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

void FTinyRogueEditorModule::PackagedProjectDevelopment(FMenuBuilder& MenuBuilder)
{
	const FString PackagedProjectPath {FPaths::ConvertRelativePathToFull(UTinyRogueEditorSettings::Get()->PackagedBuildDirectoryPath.Path)};

	MenuBuilder.AddMenuEntry(
		FText::FromString(TEXT("Development")),
		FText::FromString(FString::Printf(TEXT("Build the project in Development configuration at path %s."), *PackagedProjectPath)),
		FSlateIcon(FAppStyle::GetAppStyleSetName(), TEXT("Debug")),
		FUIAction(FExecuteAction::CreateLambda([PackagedProjectPath]() -> void
		{
			PackagedProject(TEXT("Development"), PackagedProjectPath);
		})),
		TEXT("PackagedProjectDevelopmentButton"));
}

void FTinyRogueEditorModule::PackagedProjectShipping(FMenuBuilder& MenuBuilder)
{
	const FString PackagedProjectPath {FPaths::ConvertRelativePathToFull(UTinyRogueEditorSettings::Get()->PackagedBuildDirectoryPath.Path)};

	MenuBuilder.AddMenuEntry(
		FText::FromString(TEXT("Shipping")),
		FText::FromString(FString::Printf(TEXT("Build the project in Shipping configuration at path %s."), *PackagedProjectPath)),
		FSlateIcon(FAppStyle::GetAppStyleSetName(), TEXT("ClassIcon.PlayerController")),
		FUIAction(FExecuteAction::CreateLambda([PackagedProjectPath]() -> void
		{
			PackagedProject(TEXT("Shipping"), PackagedProjectPath, TEXT("-nodebuginfo"));
		})),
		TEXT("PackagedProjectShippingButton"));
}

void FTinyRogueEditorModule::PackagingToolsSection(FMenuBuilder& MenuBuilder)
{
	MenuBuilder.BeginSection(NAME_None, LOCTEXT("Packaged Build Tools", "Packaged Build Tools"));

	LaunchPackagedBuildButton(MenuBuilder);
	PackageProjectButton(MenuBuilder);

	MenuBuilder.EndSection();
}

void FTinyRogueEditorModule::DevelopmentToolsSection(FMenuBuilder& MenuBuilder)
{
	MenuBuilder.BeginSection(NAME_None, LOCTEXT("Development Tools", "Development Tools"));

	OpenTrelloButton(MenuBuilder);

	MenuBuilder.EndSection();
}

void FTinyRogueEditorModule::PackageProjectButton(FMenuBuilder& MenuBuilder)
{
	MenuBuilder.AddSubMenu(
		FText::FromString(TEXT("Package Project")),
		FText::FromString(TEXT("Package the project with a specific configuration.")),
		FNewMenuDelegate::CreateLambda([](FMenuBuilder& SubMenuBuilder) -> void
		{
			PackagedProjectDevelopment(SubMenuBuilder);
			PackagedProjectShipping(SubMenuBuilder);
		}),
		false,
		FSlateIcon(FAppStyle::GetAppStyleSetName(), TEXT("MainFrame.PackageProject"))
	);
}

void FTinyRogueEditorModule::OpenTrelloButton(FMenuBuilder& MenuBuilder)
{
	const FString TrelloUrl {UTinyRogueEditorSettings::Get()->TrelloUrl};
	if (TrelloUrl.IsEmpty())
	{
		LOG_ERROR("Trello URL is empty.");
		return;
	}
	
	MenuBuilder.AddMenuEntry(
		FText::FromString(TEXT("Trello")),
		FText::FromString(TEXT("Open the Trello Board.")),
		FSlateIcon(FAppStyle::GetAppStyleSetName(), TEXT("Level.SaveModifiedHighlightIcon16x")),
		FUIAction(FExecuteAction::CreateLambda([TrelloUrl]() -> void
			{
				if (UKismetSystemLibrary::CanLaunchURL(TrelloUrl) == false)
				{
					LOG_ERROR("Can't open Trello board at {0}", TrelloUrl)
					return;
				}
				UKismetSystemLibrary::LaunchURL(TrelloUrl);
			}
		)),
		TEXT("ShowToDosListButton"));
	
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FTinyRogueEditorModule, TinyRogueEditor)
