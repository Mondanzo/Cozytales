// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Actor.h"
#include "NavAreas/NavArea.h"
#include "Math/Box.h"

#include "ANavMeshSpline.generated.h"

UCLASS()
class COZYTALESESSENTIALS_API AANavMeshSpline : public AActor, public INavRelevantInterface {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AANavMeshSpline();

	UFUNCTION(BlueprintCallable, CallInEditor)
	void Rebuild();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USplineComponent* Spline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Width = 300.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Segments = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector ZOffset = FVector(0, 0, -700.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UNavArea> AreaClass;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	TArray<TArray<FVector>> Areas;

#if WITH_EDITOR
	FDelegateHandle OnNavAreaRegisteredDelegateHandle;
	FDelegateHandle OnNavAreaUnregisteredDelegateHandle;
#endif

public:
	virtual void GetNavigationData(FNavigationRelevantData& Data) const override;
	virtual bool IsNavigationRelevant() const override;
	virtual void UpdateNavigationBounds() override;
	virtual FBox GetNavigationBounds() const override;

	virtual void PostInitProperties() override;
	virtual void BeginDestroy() override;

#if WITH_EDITOR
	virtual bool IsEditorOnly() const override;

	virtual void PostEditMove(bool bFinished) override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	void OnNavAreaRegistered(const UWorld& World, const UClass* Class);
	void OnNavAreaUnregistered(const UWorld& World, const UClass* Class);
	virtual void PostEditUndo() override;
#endif


private:
	void CreateAreas();

	mutable FBox Bounds;
};