// Red Panda 2024

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "HAL/IConsoleManager.h"

#include "MyCheatController.generated.h"

static TAutoConsoleVariable CVarDebugGoodSlide(
	TEXT("d.goodSlide"),
	false,
	TEXT("Toggles the fun intense slide."),
	ECVF_Cheat);

static TAutoConsoleVariable<bool> CVarDebugHideCameraShow(
	TEXT("d.HideCameraTrack"),
	false,
	TEXT("Toggles the camera track at the beginning."),
	ECVF_SaveForNextBoot
);

static TAutoConsoleVariable<bool> CVarDebugShowMarkers(
	TEXT("d.ShowMarkers"),
	false,
	TEXT("Toggles the debug markers on the map."),
	ECVF_Cheat
);

static TAutoConsoleVariable<bool> CVarDebugShowMarkersInWorld(
	TEXT("d.ShowMarkersInWorld"),
	false,
	TEXT("Toggles the visibility of placed markers."),
	ECVF_Cheat
);


static TAutoConsoleVariable<float> CVarDebugSpeedModifier(
	TEXT("d.SpeedModifier"),
	1.0,
	TEXT("Modify the speed of the player."),
	ECVF_Cheat
);


UCLASS()
class COZYTALESESSENTIALS_API UMyCheatController : public UCheatManager
{
	GENERATED_BODY()

public:
	UMyCheatController();
	~UMyCheatController();

	// Reveal the full map.
	UFUNCTION( BlueprintImplementableEvent, exec )
	void RevealMap();

	// Reset the Map Fog.
	UFUNCTION( BlueprintImplementableEvent, exec )
	void ResetMap();

	// Set the quest status using a quest id and a new quest state (0 = Not Active, 1 = Active, 2 = Completed)
	UFUNCTION( BlueprintImplementableEvent, exec )
	void SetQuest(UPARAM(DisplayName="Quest ID") int quest,UPARAM(DisplayName="Quest State (0 = Not Active, 1 = Active, 2 = Completed)") int quest_state);

	UFUNCTION(exec)
	void Crash();
};