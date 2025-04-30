#pragma once
#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "TinyRogueEditorSettings.generated.h"

UCLASS(Config="EditorPerProjectUserSettings", meta=(DisplayName="Tiny Rogue Editor Settings"))
class TINYROGUEEDITOR_API UTinyRogueEditorSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UTinyRogueEditorSettings();

	UFUNCTION(BlueprintCallable, DisplayName="Get Tiny Rogue Editor Settings")
	static const UTinyRogueEditorSettings* Get();

	UPROPERTY(Config, BlueprintReadOnly, EditDefaultsOnly)
	FDirectoryPath PackagedBuildDirectoryPath;

	UPROPERTY(Config, EditDefaultsOnly)
	TArray<FString> ToDoList;
};
