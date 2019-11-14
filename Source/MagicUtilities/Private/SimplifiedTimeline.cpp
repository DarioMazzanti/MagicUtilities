#include "SimplifiedTimeline.h"


USimplifiedTimeline::~USimplifiedTimeline()
{
	Stop();
}



void USimplifiedTimeline::Play()
{
	if (!bIsPlaying)
	{
		World = GetOuter()->GetWorld();

		if (World != nullptr && Curve != nullptr)
			StartTimer();

		return;
	}
	else
	{
		switch (CurrentDirection)
		{
		case ESimplifiedTimelineDirection::Forward:
			Unpause();
			break;
		case ESimplifiedTimelineDirection::Backward:
			Pause();
			CurrentDirection = ESimplifiedTimelineDirection::Forward;
			Unpause();
			break;
		default:
			break;
		}
	}
}

void USimplifiedTimeline::StartTimer()
{
	// setup start and finish time.
	Curve->GetTimeRange(CurveStartTime, CurveEndTime);

	switch (CurrentDirection)
	{
	case ESimplifiedTimelineDirection::Forward:
		CurrTime = CurveStartTime;
		break;
	case ESimplifiedTimelineDirection::Backward:
		CurrTime = CurveEndTime;
		break;
	default:
		break;
	}

	// setup update interval based on frequency
	UpdateInterval = 1.0 / PlaybackFrequency;

	World->GetTimerManager().SetTimer(TimerUpdateHandle, this, &USimplifiedTimeline::UpdateTimer, PlaybackFrequency, true);
	bIsPlaying = true;
}

void USimplifiedTimeline::Pause()
{
	if (bIsPlaying)
	{
		if(!World->GetTimerManager().IsTimerPaused(TimerUpdateHandle))
			World->GetTimerManager().PauseTimer(TimerUpdateHandle);
	}
}

void USimplifiedTimeline::Unpause()
{
	if (bIsPlaying)
	{
		if (World->GetTimerManager().IsTimerPaused(TimerUpdateHandle))
			World->GetTimerManager().UnPauseTimer(TimerUpdateHandle);
	}
}

void USimplifiedTimeline::Stop()
{
	if (bIsPlaying)
	{		
		if (World != nullptr && TimerUpdateHandle.IsValid())
			World->GetTimerManager().ClearTimer(TimerUpdateHandle);

		bIsPlaying = false;
		CurrTime = 0.0;
	}
}

void USimplifiedTimeline::Reverse()
{
	CurrentDirection = ESimplifiedTimelineDirection::Backward;

	if (!bIsPlaying)
	{
		if (World != nullptr && Curve != nullptr)
		{
			CurrTime = CurveEndTime;
			StartTimer();
		}
	}
}

void USimplifiedTimeline::UpdateTimer()
{
	if (!bIsPlaying)
		return;

	bool bHasFinished = false;
	
	switch (CurrentDirection)
	{
	case ESimplifiedTimelineDirection::Forward:
		
		CurrTime += UpdateInterval;
		if (CurrTime >= CurveEndTime)
		{
			CurrTime = CurveEndTime;
			Stop();
			CurrentValue = Curve->GetFloatValue(CurrTime);

			bHasFinished = true;
		}

		break;
	case ESimplifiedTimelineDirection::Backward:

		CurrTime -= UpdateInterval;
		if (CurrTime <= CurveStartTime)
		{
			CurrTime = CurveStartTime;
			Stop();
			CurrentValue = Curve->GetFloatValue(CurrTime);

			bHasFinished = true;
		}
		break;
	default:
		break;
	}

	OnTimerUpdate.Broadcast(CurrentValue, CurrentDirection);

	if(bHasFinished)
		OnTimerFinished.Broadcast(CurrentValue, CurrentDirection);
}

