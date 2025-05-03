#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "TimerTracker.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTimeTrackerEventDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTimeTrackerTickDelegate, FTimespan, ElapsedTimespan);

enum class EAPTimeTrackerIntervalState : uint8
{
	Play,
	Pause,
	Resume,
	Stop
};

UCLASS()
class TINYROGUE_API UTimeTracker : public UObject, public FTickableGameObject
{
	GENERATED_BODY()
public:
	explicit UTimeTracker(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	static UTimeTracker* CreateTimeTracker(UObject* Outer);

	UPROPERTY(BlueprintAssignable)
	FOnTimeTrackerEventDelegate OnTimeTrackerStartedDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnTimeTrackerEventDelegate OnTimeTrackerPausedDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnTimeTrackerEventDelegate OnTimeTrackerResumedDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnTimeTrackerEventDelegate OnTimeTrackerStoppedDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnTimeTrackerEventDelegate OnTimeTrackerResetDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnTimeTrackerTickDelegate OnTimeTrackerIntervalDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnTimeTrackerTickDelegate OnTimeTrackerTickDelegate;

	UFUNCTION(BlueprintCallable)
	void ClearAllDelegates();

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool IsActive() const { return bIsActive; };

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool IsPaused() const { return bIsPaused; };

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetElapsedTime(const double NewElapsedTime) { ElapsedTime = NewElapsedTime; };

	UFUNCTION(BlueprintCallable)
	FORCEINLINE FTimespan GetElapsedTimespan() const { return FTimespan::FromSeconds(ElapsedTime); };

	UFUNCTION(BlueprintCallable)
	FORCEINLINE double GetElapsedTime() const { return ElapsedTime; };

	UFUNCTION(BlueprintCallable)
	void Start(double IntervalValue = 1.0f, const double StartTime = 0.0f);

	UFUNCTION(BlueprintCallable)
	void Pause();

	UFUNCTION(BlueprintCallable)
	void Resume();

	UFUNCTION(BlueprintCallable)
	void Stop();

	UFUNCTION(BlueprintCallable)
	void Reset();

protected:
	virtual void BeginDestroy() override;

	virtual bool IsTickable() const override;
	virtual TStatId GetStatId() const override;
	virtual ETickableTickType GetTickableTickType() const override;
	virtual UWorld* GetTickableGameObjectWorld() const override;
	virtual void Tick(const float DeltaSeconds) override;

private:
	uint8 bIsActive:1;
	uint8 bIsPaused:1;
	double ElapsedTime;
	double Interval;
	FTimerHandle IntervalTimerHandle;

	double GetTimeDilation() const;
	void SetTimerIntervalState(const EAPTimeTrackerIntervalState IntervalState, const double IntervalValue = -1.0f);

	UFUNCTION()
	void OnTimerIntervalCallback();
};
