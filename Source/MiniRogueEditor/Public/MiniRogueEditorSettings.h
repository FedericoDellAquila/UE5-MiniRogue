#pragma once
#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "MiniRogueEditorSettings.generated.h"

UCLASS(Config="EditorPerProjectUserSettings", meta=(DisplayName="Mini Rogue Editor Settings"))
class MINIROGUEEDITOR_API UMiniRogueEditorSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UMiniRogueEditorSettings();

	UFUNCTION(BlueprintCallable, DisplayName="Get Mini Rogue Editor Settings")
	static const UMiniRogueEditorSettings* Get();

	UPROPERTY(Config, BlueprintReadOnly, EditDefaultsOnly)
	FDirectoryPath PackagedBuildDirectoryPath;

	UPROPERTY(Config, EditDefaultsOnly)
	TArray<FString> ToDoList;
};
