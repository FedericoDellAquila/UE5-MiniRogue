#include "Core/DieFace.h"

UDieFace::UDieFace()
	: Value(-1)
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UDieFace::SetValue(int32 InValue)
{
	Value = InValue;
	SetText(FText::FromString(FString::FromInt(Value)));
}

FVector UDieFace::GetFaceDirection() const
{
	return GetForwardVector();
}
