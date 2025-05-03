#pragma once

#define LOG_CATEGORY TinyRogueLog
DECLARE_LOG_CATEGORY_EXTERN(TinyRogueLog, All, All);
inline DEFINE_LOG_CATEGORY(TinyRogueLog);

// Network Role  
#define NETWORK_ROLE (!IsValid(GetWorld()) ? TEXT("None")\
    : GetWorld()->GetNetMode() == ENetMode::NM_Standalone ? TEXT("Standalone")\
    : GetWorld()->GetNetMode() == ENetMode::NM_Client ? TEXT("Client")\
    : GetWorld()->GetNetMode() == ENetMode::NM_ListenServer ? TEXT("ListenServer")\
    : TEXT("DedicatedServer"))

// Network Role and ID  
#define NETWORK_ID (FString::Printf(TEXT("%s %i"), NETWORK_ROLE, static_cast<int32>(GPlayInEditorID)))

//Current File Path
#define STR_CUR_FILE_PATH (FString(__FILE__))

//Current Line Number  
#define STR_CUR_LINE (FString::FromInt(__LINE__))

// Platform-specific defines
#if PLATFORM_WINDOWS

// Current File Name
#define STR_CUR_FILE_NAME (FString(strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__))

// Current Class + Function Name
#define STR_CUR_CLASS_FUNC (FString(__FUNCTION__))

// Current Class
#define STR_CUR_CLASS (FString(__FUNCTION__).Left(FString(__FUNCTION__).Find(TEXT(":"))))

// Current Function Name
#define STR_CUR_FUNC (FString(__FUNCTION__).Right(FString(__FUNCTION__).Len() - FString(__FUNCTION__).Find(TEXT("::")) - 2))

// Current Function Signature
#define STR_CUR_FUNC_SIGN (FString(__FUNCSIG__))

#elif PLATFORM_MAC

    // Current File Name
    #define STR_CUR_FILE_NAME (FString(strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__))

    // Current Class + Function Name
    #define STR_CUR_CLASS_FUNC (FString(__PRETTY_FUNCTION__))

    // Current Class
    #define STR_CUR_CLASS (FString(__PRETTY_FUNCTION__).Left(FString(__PRETTY_FUNCTION__).Find(TEXT(":"))))

    // Current Function Name
    #define STR_CUR_FUNC (FString(__PRETTY_FUNCTION__).Right(FString(__PRETTY_FUNCTION__).Len() - FString(__PRETTY_FUNCTION__).Find(TEXT("::")) - 2))

    // Current Function Signature
    #define STR_CUR_FUNC_SIGN (FString(__PRETTY_FUNCTION__))

#elif PLATFORM_ANDROID

    // Current File Name
    #define STR_CUR_FILE_NAME (FString(strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__))

    // Current Class + Function Name
    #define STR_CUR_CLASS_FUNC (FString(__PRETTY_FUNCTION__))

    // Current Class
    #define STR_CUR_CLASS (FString(__PRETTY_FUNCTION__).Left(FString(__PRETTY_FUNCTION__).Find(TEXT(":"))))

    // Current Function Name
    #define STR_CUR_FUNC (FString(__PRETTY_FUNCTION__).Right(FString(__PRETTY_FUNCTION__).Len() - FString(__PRETTY_FUNCTION__).Find(TEXT("::")) - 2))

    // Current Function Signature
    #define STR_CUR_FUNC_SIGN (FString(__PRETTY_FUNCTION__))

#else // Other platforms

    // Current File Name
    #define STR_CUR_FILE_NAME (FString(strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__))

    // Current Class + Function Name
    #define STR_CUR_CLASS_FUNC (FString(__func__))

    // Current Class
    #define STR_CUR_CLASS (FString(__func__).Left(FString(__func__).Find(TEXT(":"))))

    // Current Function Name
    #define STR_CUR_FUNC (FString(__func__).Right(FString(__func__).Len() - FString(__func__).Find(TEXT("::")) - 2))

    // Current Function Signature
    #define STR_CUR_FUNC_SIGN (FString(__func__))

#endif

//Current Class and Line Number
#define STR_CUR_CLASS_LINE (STR_CUR_CLASS + "(" + STR_CUR_LINE + ")")

//Current File Name and Line Number
#define STR_CUR_FILE_LINE (STR_CUR_FILE_NAME + "(" + STR_CUR_LINE + ")")

// Current Class Function and Line Number
#define STR_CUR_CLASS_FUN_LINE (STR_CUR_CLASS_FUNC + "(" + STR_CUR_LINE + ")")

//Convert an UEnum into a FString value (including scope name)  
#define UENUM_TO_STRING(UEnumValue) UEnum::GetValueAsString(UEnumValue)

//Convert an UEnum into a FString value (excluding scope name)  
#define UENUM_VALUE_TO_STRING(UEnumValue) StaticEnum<std::decay<decltype(UEnumValue)>::type>()->GetNameStringByValue(static_cast<int64>(UEnumValue))

//Convert an Enum into a FString value
#define ENUM_TO_STRING(EnumValue) LexToString(EnumValue)

//Convert a Bool into a FString value
#define BOOL_TO_STRING(Bool) FString(Bool ? TEXT("true") : TEXT("false"))

#if UE_BUILD_SHIPPING

// Example: LOG("Eat {0} {1}.", 5, TEXT("apples"))
#define LOG(Message, ...)           {}
// Example: LOG_WARNING("Eat {0} {1}.", 5, TEXT("apples"))    
#define LOG_WARNING(Message, ...)   {}
// Example: LOG_ERROR("Eat {0} {1}.", 5, TEXT("apples"))    
#define LOG_ERROR(Message, ...)     {}
// Example: LOG_FATAL("Eat {0} {1}.", 5, TEXT("apples"))    
#define LOG_FATAL(Message, ...)     {}


// Example: CLOG(IsValid(Actor), "Eat {0} {1}.", 5, TEXT("apples"))
#define CLOG(Condition, Message, ...)           {}
// Example: CLOG_WARNING(IsValid(Actor), "Eat {0} {1}.", 5, TEXT("apples"))    
#define CLOG_WARNING(Condition, Message, ...)   {}
// Example: CLOG_ERROR(IsValid(Actor), "Eat {0} {1}.", 5, TEXT("apples"))    
#define CLOG_ERROR(Condition, Message, ...)     {}
// Example: CLOG_FATAL(IsValid(Actor), "Eat {0} {1}.", 5, TEXT("apples"))    
#define CLOG_FATAL(Condition, Message, ...)     {}


// Example: PRINT(10.0f, FColor::Orange, "Eat {0} {1}.", 5, TEXT("apples"))
#define PRINT(DisplayTime, bPrintConsole, Color, Message, ...)     {}
// Example: PRINT_MESSAGE(10.0f, "Eat {0} {1}.", 5, TEXT("apples"))    
#define PRINT_MESSAGE(DisplayTime, bPrintConsole, Message, ...)    {}
// Example: PRINT_WARNING(10.0f, "Eat {0} {1}.", 5, TEXT("apples"))    
#define PRINT_WARNING(DisplayTime, bPrintConsole, Message, ...)    {}
// Example: PRINT_ERROR(10.0f, "Eat {0} {1}.", 5, TEXT("apples"))    
#define PRINT_ERROR(DisplayTime, bPrintConsole, Message, ...)      {}
// Example: PRINT_FATAL(10.0f, "Eat {0} {1}.", 5, TEXT("apples"))    
#define PRINT_FATAL(DisplayTime, bPrintConsole, Message, ...)      {}


// Example: CPRINT(IsValid(Actor), 10.0f, FColor::Orange, "Eat {0} {1}.", 5, TEXT("apples"))
#define CPRINT(Condition, DisplayTime, bPrintConsole, Color, Message, ...)     {}
// Example: CPRINT_MESSAGE(IsValid(Actor), 10.0f, "Eat {0} {1}.", 5, TEXT("apples"))    
#define CPRINT_MESSAGE(Condition, DisplayTime, bPrintConsole, Message, ...)    {}
// Example: CPRINT_WARNING(IsValid(Actor), 10.0f, "Eat {0} {1}.", 5, TEXT("apples"))    
#define CPRINT_WARNING(Condition, DisplayTime, bPrintConsole, Message, ...)    {}
// Example: CPRINT_ERROR(IsValid(Actor), 10.0f, "Eat {0} {1}.", 5, TEXT("apples"))    
#define CPRINT_ERROR(Condition, DisplayTime, bPrintConsole, Message, ...)      {}
// Example: CPRINT_FATAL(IsValid(Actor), 10.0f, "Eat {0} {1}.", 5, TEXT("apples"))    
#define CPRINT_FATAL(Condition, DisplayTime, bPrintConsole, Message, ...)      {}
  

// Example: PRINT_K(4, 10.0f, "Eat {0} {1}.", 5, TEXT("apples"))
#define PRINT_K(Key, DisplayTime, bPrintConsole, Color, Message, ...)      {}
// Example: PRINT_MESSAGE_K(4, 10.0f, "Eat {0} {1}.", 5, TEXT("apples"))    
#define PRINT_MESSAGE_K(Key, DisplayTime, bPrintConsole, Message, ...)     {}
// Example: PRINT_WARNING_K(4, 10.0f, "Eat {0} {1}.", 5, TEXT("apples"))    
#define PRINT_WARNING_K(Key, DisplayTime, bPrintConsole, Message, ...)     {}
// Example: PRINT_ERROR_K(4, 10.0f, "Eat {0} {1}.", 5, TEXT("apples"))    
#define PRINT_ERROR_K(Key, DisplayTime, bPrintConsole, Message, ...)       {}
// Example: PRINT_FATAL_K(4, 10.0f, "Eat {0} {1}.", 5, TEXT("apples"))    
#define PRINT_FATAL_K(Key, DisplayTime, bPrintConsole, Message, ...)       {}


// Example: CPRINT_K(IsValid(Actor), 4, 10.0f, "Eat {0} {1}.", 5, TEXT("apples"))
#define CPRINT_K(Condition, Key, DisplayTime, bPrintConsole, Color, Message, ...)      {}
// Example: CPRINT_MESSAGE_K(IsValid(Actor), 4, 10.0f, "Eat {0} {1}.", 5, TEXT("apples"))    
#define CPRINT_MESSAGE_K(Condition, Key, DisplayTime, bPrintConsole, Message, ...)     {}
// Example: CPRINT_WARNING_K(IsValid(Actor), 4, 10.0f, "Eat {0} {1}.", 5, TEXT("apples"))    
#define CPRINT_WARNING_K(Condition, Key, DisplayTime, bPrintConsole, Message, ...)     {}
// Example: CPRINT_ERROR_K(IsValid(Actor), 4, 10.0f, "Eat {0} {1}.", 5, TEXT("apples"))    
#define CPRINT_ERROR_K(Condition, Key, DisplayTime, bPrintConsole, Message, ...)       {}
// Example: CPRINT_FATAL_K(IsValid(Actor), 4, 10.0f, "Eat {0} {1}.", 5, TEXT("apples"))    
#define CPRINT_FATAL_K(Condition, Key, DisplayTime, bPrintConsole, Message, ...)       {}
    
#else

// Example: LOG("Eat {0} {1}.", 5, TEXT("apples"))
#define LOG(Message, ...)           { UE_LOG(LOG_CATEGORY, Log, TEXT("%s: %s"), *STR_CUR_CLASS_FUN_LINE, *FString::Format(TEXT(Message), TArray<FStringFormatArg>{__VA_ARGS__})); }
// Example: LOG_WARNING("Eat {0} {1}.", 5, TEXT("apples"))    
#define LOG_WARNING(Message, ...)   { UE_LOG(LOG_CATEGORY, Warning, TEXT("%s: %s"), *STR_CUR_CLASS_FUN_LINE, *FString::Format(TEXT(Message), TArray<FStringFormatArg>{__VA_ARGS__})); }
// Example: LOG_ERROR("Eat {0} {1}.", 5, TEXT("apples"))    
#define LOG_ERROR(Message, ...)     { UE_LOG(LOG_CATEGORY, Error, TEXT("%s: %s"), *STR_CUR_CLASS_FUN_LINE, *FString::Format(TEXT(Message), TArray<FStringFormatArg>{__VA_ARGS__})); }
// Example: LOG_FATAL("Eat {0} {1}.", 5, TEXT("apples"))    
#define LOG_FATAL(Message, ...)     { UE_LOG(LOG_CATEGORY, Fatal, TEXT("%s: %s"), *STR_CUR_CLASS_FUN_LINE, *FString::Format(TEXT(Message), TArray<FStringFormatArg>{__VA_ARGS__})); }


// Example: CLOG(IsValid(Actor), "Eat {0} {1}.", 5, TEXT("apples"))
#define CLOG(Condition, Message, ...)           { if (Condition) { UE_LOG(LOG_CATEGORY, Log, TEXT("%s: %s"), *STR_CUR_CLASS_FUN_LINE, *FString::Format(TEXT(Message), TArray<FStringFormatArg>{__VA_ARGS__})); }}
// Example: CLOG_WARNING(IsValid(Actor), "Eat {0} {1}.", 5, TEXT("apples"))    
#define CLOG_WARNING(Condition, Message, ...)   { if (Condition) { UE_LOG(LOG_CATEGORY, Warning, TEXT("%s: %s"), *STR_CUR_CLASS_FUN_LINE, *FString::Format(TEXT(Message), TArray<FStringFormatArg>{__VA_ARGS__})); }}
// Example: CLOG_ERROR(IsValid(Actor), "Eat {0} {1}.", 5, TEXT("apples"))    
#define CLOG_ERROR(Condition, Message, ...)     { if (Condition) { UE_LOG(LOG_CATEGORY, Error, TEXT("%s: %s"), *STR_CUR_CLASS_FUN_LINE, *FString::Format(TEXT(Message), TArray<FStringFormatArg>{__VA_ARGS__})); }}
// Example: CLOG_FATAL(IsValid(Actor), "Eat {0} {1}.", 5, TEXT("apples"))    
#define CLOG_FATAL(Condition, Message, ...)     { if (Condition) { UE_LOG(LOG_CATEGORY, Fatal, TEXT("%s: %s"), *STR_CUR_CLASS_FUN_LINE, *FString::Format(TEXT(Message), TArray<FStringFormatArg>{__VA_ARGS__})); }}


// Example: PRINT(10.0f, FColor::Orange, "Eat {0} {1}.", 5, TEXT("apples"))
#define PRINT(DisplayTime, bPrintConsole, Color, Message, ...)     { if (GEngine) GEngine->AddOnScreenDebugMessage(-1, DisplayTime, Color, *FString::Format(TEXT(Message), TArray<FStringFormatArg>{__VA_ARGS__}), true); if (bPrintConsole) {LOG(Message, __VA_ARGS__);} }
// Example: PRINT_MESSAGE(10.0f, "Eat {0} {1}.", 5, TEXT("apples"))    
#define PRINT_MESSAGE(DisplayTime, bPrintConsole, Message, ...)    { if (GEngine) GEngine->AddOnScreenDebugMessage(-1, DisplayTime, FColor::Cyan, *FString::Format(TEXT(Message), TArray<FStringFormatArg>{__VA_ARGS__}), true); if (bPrintConsole) {LOG(Message, __VA_ARGS__);} }
// Example: PRINT_WARNING(10.0f, "Eat {0} {1}.", 5, TEXT("apples"))    
#define PRINT_WARNING(DisplayTime, bPrintConsole, Message, ...)    { if (GEngine) GEngine->AddOnScreenDebugMessage(-1, DisplayTime, FColor::Yellow, *FString::Format(TEXT(Message), TArray<FStringFormatArg>{__VA_ARGS__}), true); if (bPrintConsole) {LOG_WARNING(Message, __VA_ARGS__);} }
// Example: PRINT_ERROR(10.0f, "Eat {0} {1}.", 5, TEXT("apples"))    
#define PRINT_ERROR(DisplayTime, bPrintConsole, Message, ...)      { if (GEngine) GEngine->AddOnScreenDebugMessage(-1, DisplayTime, FColor::Red, *FString::Format(TEXT(Message), TArray<FStringFormatArg>{__VA_ARGS__}), true); if (bPrintConsole) {LOG_ERROR(Message, __VA_ARGS__);} }
// Example: PRINT_FATAL(10.0f, "Eat {0} {1}.", 5, TEXT("apples"))    
#define PRINT_FATAL(DisplayTime, bPrintConsole, Message, ...)      { if (GEngine) GEngine->AddOnScreenDebugMessage(-1, DisplayTime, FColor::Red, *FString::Format(TEXT(Message), TArray<FStringFormatArg>{__VA_ARGS__}), true); if (bPrintConsole) {LOG_FATAL(Message, __VA_ARGS__);} }


// Example: CPRINT(IsValid(Actor), 10.0f, FColor::Orange, "Eat {0} {1}.", 5, TEXT("apples"))
#define CPRINT(Condition, DisplayTime, bPrintConsole, Color, Message, ...)     { if (Condition) { if (GEngine) GEngine->AddOnScreenDebugMessage(-1, DisplayTime, Color, *FString::Format(TEXT(Message), TArray<FStringFormatArg>{__VA_ARGS__}), true); if (bPrintConsole) {LOG(Message, __VA_ARGS__);} }}
// Example: CPRINT_MESSAGE(IsValid(Actor), 10.0f, "Eat {0} {1}.", 5, TEXT("apples"))    
#define CPRINT_MESSAGE(Condition, DisplayTime, bPrintConsole, Message, ...)    { if (Condition) { if (GEngine) GEngine->AddOnScreenDebugMessage(-1, DisplayTime, FColor::Cyan, *FString::Format(TEXT(Message), TArray<FStringFormatArg>{__VA_ARGS__}), true); if (bPrintConsole) {LOG(Message, __VA_ARGS__);} }}
// Example: CPRINT_WARNING(IsValid(Actor), 10.0f, "Eat {0} {1}.", 5, TEXT("apples"))    
#define CPRINT_WARNING(Condition, DisplayTime, bPrintConsole, Message, ...)    { if (Condition) { if (GEngine) GEngine->AddOnScreenDebugMessage(-1, DisplayTime, FColor::Yellow, *FString::Format(TEXT(Message), TArray<FStringFormatArg>{__VA_ARGS__}), true); if (bPrintConsole) {LOG_WARNING(Message, __VA_ARGS__);} }}
// Example: CPRINT_ERROR(IsValid(Actor), 10.0f, "Eat {0} {1}.", 5, TEXT("apples"))    
#define CPRINT_ERROR(Condition, DisplayTime, bPrintConsole, Message, ...)      { if (Condition) { if (GEngine) GEngine->AddOnScreenDebugMessage(-1, DisplayTime, FColor::Red, *FString::Format(TEXT(Message), TArray<FStringFormatArg>{__VA_ARGS__}), true); if (bPrintConsole) {LOG_ERROR(Message, __VA_ARGS__);} }}
// Example: CPRINT_FATAL(IsValid(Actor), 10.0f, "Eat {0} {1}.", 5, TEXT("apples"))    
#define CPRINT_FATAL(Condition, DisplayTime, bPrintConsole, Message, ...)      { if (Condition) { if (GEngine) GEngine->AddOnScreenDebugMessage(-1, DisplayTime, FColor::Red, *FString::Format(TEXT(Message), TArray<FStringFormatArg>{__VA_ARGS__}), true); if (bPrintConsole) {LOG_FATAL(Message, __VA_ARGS__);} }}


// Example: PRINT_K(4, 10.0f, "Eat {0} {1}.", 5, TEXT("apples"))
#define PRINT_K(Key, DisplayTime, bPrintConsole, Color, Message, ...)      { if (GEngine) GEngine->AddOnScreenDebugMessage(Key, DisplayTime, Color, *FString::Format(TEXT(Message), TArray<FStringFormatArg>{__VA_ARGS__}), true); if (bPrintConsole) {LOG(Message, __VA_ARGS__);} }
// Example: PRINT_MESSAGE_K(4, 10.0f, "Eat {0} {1}.", 5, TEXT("apples"))    
#define PRINT_MESSAGE_K(Key, DisplayTime, bPrintConsole, Message, ...)     { if (GEngine) GEngine->AddOnScreenDebugMessage(Key, DisplayTime, FColor::Cyan, *FString::Format(TEXT(Message), TArray<FStringFormatArg>{__VA_ARGS__}), true); if (bPrintConsole) {LOG(Message, __VA_ARGS__);} }
// Example: PRINT_WARNING_K(4, 10.0f, "Eat {0} {1}.", 5, TEXT("apples"))    
#define PRINT_WARNING_K(Key, DisplayTime, bPrintConsole, Message, ...)     { if (GEngine) GEngine->AddOnScreenDebugMessage(Key, DisplayTime, FColor::Yellow, *FString::Format(TEXT(Message), TArray<FStringFormatArg>{__VA_ARGS__}), true); if (bPrintConsole) {LOG_WARNING(Message, __VA_ARGS__);} }
// Example: PRINT_ERROR_K(4, 10.0f, "Eat {0} {1}.", 5, TEXT("apples"))    
#define PRINT_ERROR_K(Key, DisplayTime, bPrintConsole, Message, ...)       { if (GEngine) GEngine->AddOnScreenDebugMessage(Key, DisplayTime, FColor::Red, *FString::Format(TEXT(Message), TArray<FStringFormatArg>{__VA_ARGS__}), true); if (bPrintConsole) {LOG_ERROR(Message, __VA_ARGS__);} }
// Example: PRINT_FATAL_K(4, 10.0f, "Eat {0} {1}.", 5, TEXT("apples"))    
#define PRINT_FATAL_K(Key, DisplayTime, bPrintConsole, Message, ...)       { if (GEngine) GEngine->AddOnScreenDebugMessage(Key, DisplayTime, FColor::Red, *FString::Format(TEXT(Message), TArray<FStringFormatArg>{__VA_ARGS__}), true); if (bPrintConsole) {LOG_FATAL(Message, __VA_ARGS__);} }


// Example: CPRINT_K(IsValid(Actor), 4, 10.0f, "Eat {0} {1}.", 5, TEXT("apples"))
#define CPRINT_K(Condition, Key, DisplayTime, bPrintConsole, Color, Message, ...)      { if (Condition) { if (GEngine) GEngine->AddOnScreenDebugMessage(Key, DisplayTime, Color, *FString::Format(TEXT(Message), TArray<FStringFormatArg>{__VA_ARGS__}), true); if (bPrintConsole) {LOG(Message, __VA_ARGS__);} }}
// Example: CPRINT_MESSAGE_K(IsValid(Actor), 4, 10.0f, "Eat {0} {1}.", 5, TEXT("apples"))    
#define CPRINT_MESSAGE_K(Condition, Key, DisplayTime, bPrintConsole, Message, ...)     { if (Condition) { if (GEngine) GEngine->AddOnScreenDebugMessage(Key, DisplayTime, FColor::Cyan, *FString::Format(TEXT(Message), TArray<FStringFormatArg>{__VA_ARGS__}), true); if (bPrintConsole) {LOG(Message, __VA_ARGS__);} }}
// Example: CPRINT_WARNING_K(IsValid(Actor), 4, 10.0f, "Eat {0} {1}.", 5, TEXT("apples"))    
#define CPRINT_WARNING_K(Condition, Key, DisplayTime, bPrintConsole, Message, ...)     { if (Condition) { if (GEngine) GEngine->AddOnScreenDebugMessage(Key, DisplayTime, FColor::Yellow, *FString::Format(TEXT(Message), TArray<FStringFormatArg>{__VA_ARGS__}), true); if (bPrintConsole) {LOG_WARNING(Message, __VA_ARGS__);} }}
// Example: CPRINT_ERROR_K(IsValid(Actor), 4, 10.0f, "Eat {0} {1}.", 5, TEXT("apples"))    
#define CPRINT_ERROR_K(Condition, Key, DisplayTime, bPrintConsole, Message, ...)       { if (Condition) { if (GEngine) GEngine->AddOnScreenDebugMessage(Key, DisplayTime, FColor::Red, *FString::Format(TEXT(Message), TArray<FStringFormatArg>{__VA_ARGS__}), true); if (bPrintConsole) {LOG_ERROR(Message, __VA_ARGS__);} }}
// Example: CPRINT_FATAL_K(IsValid(Actor), 4, 10.0f, "Eat {0} {1}.", 5, TEXT("apples"))    
#define CPRINT_FATAL_K(Condition, Key, DisplayTime, bPrintConsole, Message, ...)       { if (Condition) { if (GEngine) GEngine->AddOnScreenDebugMessage(Key, DisplayTime, FColor::Red, *FString::Format(TEXT(Message), TArray<FStringFormatArg>{__VA_ARGS__}), true); if (bPrintConsole) {LOG_FATAL(Message, __VA_ARGS__);} }}

#endif
