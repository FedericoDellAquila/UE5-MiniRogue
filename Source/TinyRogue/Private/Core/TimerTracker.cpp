#include "Core/TimerTracker.h"
#include "Utility/Log.h"

UTimeTracker::UTimeTracker(const FObjectInitializer& ObjectInitializer/*= FObjectInitializer::Get()*/)
	: Super(ObjectInitializer)
	, bIsActive(false)
	, bIsPaused(false)
	, ElapsedTime(0.0)
	, Interval(0.0)
{}

UTimeTracker* UTimeTracker::CreateTimeTracker(UObject* Outer)
{
	return NewObject<UTimeTracker>(Outer);
}

void UTimeTracker::BeginDestroy()
{
	ClearAllDelegates();
	UObject::BeginDestroy();
}

TStatId UTimeTracker::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UTimerTracker, STATGROUP_Tickables);
}

ETickableTickType UTimeTracker::GetTickableTickType() const
{
	return ETickableTickType::Conditional;
}

bool UTimeTracker::IsTickable() const
{
	return IsValid(GetOuter()) && bIsActive && !bIsPaused;
}

UWorld* UTimeTracker::GetTickableGameObjectWorld() const
{
	return GetWorld();
}

void UTimeTracker::ClearAllDelegates()
{
	OnTimeTrackerStartedDelegate.Clear();
	OnTimeTrackerPausedDelegate.Clear();
	OnTimeTrackerResumedDelegate.Clear();
	OnTimeTrackerStoppedDelegate.Clear();
	OnTimeTrackerResetDelegate.Clear();
	OnTimeTrackerIntervalDelegate.Clear();
	OnTimeTrackerTickDelegate.Clear();
}

void UTimeTracker::Start(const double IntervalValue/*= 1.0f*/, const double StartTime/*= 0.0f*/)
{
	if (!IsValid(GetOuter()))
	{
		LOG_ERROR("{0} - Outer is nullptr. Use UTimeTracker::CreateTimeTracker() to create a UTimeTracker.", this->GetName());
		return;
	}

	if (bIsActive)
	{
		LOG_WARNING("{0}->{1} is already active.", GetOuter()->GetName(), this->GetName());
		return;
	}

	bIsActive = true;
	bIsPaused = false;
	ElapsedTime = StartTime;
	SetTimerIntervalState(EAPTimeTrackerIntervalState::Play, IntervalValue);

	OnTimeTrackerStartedDelegate.Broadcast();
	OnTimeTrackerTickDelegate.Broadcast(GetElapsedTimespan());
}

void UTimeTracker::Pause()
{
	if (!bIsActive)
	{
		LOG_WARNING("{0}->{1} is not active.", GetOuter()->GetName(), this->GetName());
		return;
	}

	if (bIsPaused)
	{
		LOG_WARNING("{0}->{1} is already paused.", GetOuter()->GetName(), this->GetName());
		return;
	}

	bIsPaused = true;
	OnTimeTrackerPausedDelegate.Broadcast();

	SetTimerIntervalState(EAPTimeTrackerIntervalState::Pause);
}

void UTimeTracker::Resume()
{
	if (!bIsActive)
	{
		LOG_WARNING("{0}->{1} is not active.", GetOuter()->GetName(), this->GetName());
		return;
	}

	if (!bIsPaused)
	{
		LOG_WARNING("{0}->{1} is not paused.", GetOuter()->GetName(), this->GetName());
		return;
	}

	bIsPaused = false;
	OnTimeTrackerResumedDelegate.Broadcast();

	SetTimerIntervalState(EAPTimeTrackerIntervalState::Resume);
}

void UTimeTracker::Stop()
{
	if (!bIsActive)
	{
		LOG_WARNING("{0}->{1} is not active.", GetOuter()->GetName(), this->GetName());
		return;
	}

	bIsActive = false;
	bIsPaused = false;

	OnTimeTrackerStoppedDelegate.Broadcast();

	SetTimerIntervalState(EAPTimeTrackerIntervalState::Stop);
}

void UTimeTracker::Reset()
{
	if (!bIsActive)
	{
		LOG_WARNING("{0}->{1} is not active.", GetOuter()->GetName(), this->GetName());
		return;
	}

	ElapsedTime = 0.0;

	OnTimeTrackerResetDelegate.Broadcast();

	SetTimerIntervalState(EAPTimeTrackerIntervalState::Stop);
}

void UTimeTracker::Tick(const float DeltaSeconds)
{
	const double TimeDilation {GetTimeDilation()};
	const double DeltaTime {DeltaSeconds * TimeDilation};
	ElapsedTime += DeltaTime;

	OnTimeTrackerTickDelegate.Broadcast(GetElapsedTimespan());
}

double UTimeTracker::GetTimeDilation() const
{
	const UWorld* World {GetTickableGameObjectWorld()};
	return IsValid(World) ? World->GetWorldSettings()->TimeDilation : 1.0;
}

void UTimeTracker::SetTimerIntervalState(const EAPTimeTrackerIntervalState IntervalState, const double IntervalValue/*= -1.0f*/)
{
	const UWorld* World {GetTickableGameObjectWorld()};
	if (!IsValid(World))
	{
		LOG_ERROR("{0}->{1} World is nullptr.", GetOuter()->GetName(), this->GetName());
		return;
	}

	switch (IntervalState)
	{
		case EAPTimeTrackerIntervalState::Play:
		{
			if (FMath::IsNegativeOrNegativeZero(IntervalValue))
			{
				LOG_WARNING("{0}->{1} IntervalValue must be zero or greater.", GetOuter()->GetName(), this->GetName());
				Interval = World->GetDeltaSeconds();
			}
			else
				Interval = IntervalValue;

			if (IntervalTimerHandle.IsValid())
				World->GetTimerManager().ClearTimer(IntervalTimerHandle);

			FTimerDelegate Delegate {};
			Delegate.BindUObject(this, &UTimeTracker::OnTimerIntervalCallback);
			World->GetTimerManager().SetTimer(IntervalTimerHandle, Delegate, Interval, true);

			break;
		}
		case EAPTimeTrackerIntervalState::Pause:
		{
			World->GetTimerManager().PauseTimer(IntervalTimerHandle);
			break;
		}
		case EAPTimeTrackerIntervalState::Resume:
		{
			World->GetTimerManager().UnPauseTimer(IntervalTimerHandle);
			break;
		}
		case EAPTimeTrackerIntervalState::Stop:
		{
			World->GetTimerManager().ClearTimer(IntervalTimerHandle);
			break;
		}
		default:
		{
			LOG_ERROR("{0}->{1} Invalid interval state.", GetOuter()->GetName(), this->GetName());
			break;
		}
	}
}

void UTimeTracker::OnTimerIntervalCallback()
{
	OnTimeTrackerIntervalDelegate.Broadcast(GetElapsedTimespan());
}