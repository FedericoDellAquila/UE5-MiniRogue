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
	const TSharedPtr<FExtender> MenuExtender {MakeShareable(new FExtender {})};

	MenuExtender->AddMenuBarExtension(
		TEXT("Help"),
		EExtensionHook::After,
		nullptr,
		FMenuBarExtensionDelegate::CreateLambda([](FMenuBarBuilder& MenuBarBuilder) -> void
		{
			MenuBarBuilder.AddPullDownMenu(
				FText::FromString(TEXT("Mini Rogue")),
				FText::FromString(TEXT("Mini Rogue editor functionalities.")),
				FNewMenuDelegate::CreateLambda([](FMenuBuilder& MenuBuilder) -> void
				{
					PackagingToolsSection(MenuBuilder);
					DevelopmentToolsSection(MenuBuilder);
				}),
				TEXT("MiniRogueTab")
			);
		})
	);

	FLevelEditorModule& LevelEditorModule {FModuleManager::LoadModuleChecked<FLevelEditorModule>(TEXT("LevelEditor"))};
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

	MenuBuilder.AddMenuEntry(
		FText::FromString(TEXT("Launch Packaged Build")),
		FText::FromString(FString::Printf(TEXT("Launch the executable in %s."), *UMiniRogueEditorSettings::Get()->PackagedBuildDirectoryPath.Path)),
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

void FMiniRogueEditorModule::PackagedProjectDevelopment(FMenuBuilder& MenuBuilder)
{
	const FString PackagedProjectPath {UMiniRogueEditorSettings::Get()->PackagedBuildDirectoryPath.Path};

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

void FMiniRogueEditorModule::PackagedProjectShipping(FMenuBuilder& MenuBuilder)
{
	const FString PackagedProjectPath {UMiniRogueEditorSettings::Get()->PackagedBuildDirectoryPath.Path};

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

void FMiniRogueEditorModule::PackagingToolsSection(FMenuBuilder& MenuBuilder)
{
	MenuBuilder.BeginSection(NAME_None, LOCTEXT("Packaged Build Tools", "Packaged Build Tools"));

	LaunchPackagedBuildButton(MenuBuilder);
	PackageProjectButton(MenuBuilder);

	MenuBuilder.EndSection();
}

void FMiniRogueEditorModule::DevelopmentToolsSection(FMenuBuilder& MenuBuilder)
{
	MenuBuilder.BeginSection(NAME_None, LOCTEXT("Development Tools", "Development Tools"));

	ShowToDosListButton(MenuBuilder);

	MenuBuilder.EndSection();
}

void FMiniRogueEditorModule::PackageProjectButton(FMenuBuilder& MenuBuilder)
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

void FMiniRogueEditorModule::ShowToDosListButton(FMenuBuilder& MenuBuilder)
{
	MenuBuilder.AddMenuEntry(
		FText::FromString(TEXT("TODO List")),
		FText::FromString(TEXT("Display a list of things to do.")),
		FSlateIcon(FAppStyle::GetAppStyleSetName(), TEXT("Level.SaveModifiedHighlightIcon16x")),
		FUIAction(FExecuteAction::CreateLambda([]() -> void
			{
				FString ToDoList {};

				if (UMiniRogueEditorSettings::Get()->ToDoList.IsEmpty() == false)
				{
					for (const FString& Todo : UMiniRogueEditorSettings::Get()->ToDoList)
					{
						ToDoList.Append(FString::Printf(TEXT("- %s\n"), *Todo));
					}
					ToDoList.RemoveAt(ToDoList.Len() - 1, 1);
				}
				else
				{
					ToDoList = TEXT("There are no TODOs to display.");
				}

				FMessageDialog::Open(EAppMsgCategory::Info, EAppMsgType::Ok, FText::FromString(ToDoList), FText::FromString({TEXT("TODO List")}));
			}
		)),
		TEXT("ShowToDosListButton"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FMiniRogueEditorModule, MiniRogueEditor)
