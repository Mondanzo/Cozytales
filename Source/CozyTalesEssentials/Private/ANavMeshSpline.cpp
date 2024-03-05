// Fill out your copyright notice in the Description page of Project Settings.


#include "ANavMeshSpline.h"

#include "AI/NavigationSystemBase.h"


// Sets default values
AANavMeshSpline::AANavMeshSpline() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	Spline->SetupAttachment(RootComponent);
	RootComponent->SetCanEverAffectNavigation(true);
#if WITH_EDITOR
	Spline->ScaleVisualizationWidth = Width;
#endif
}

void AANavMeshSpline::Rebuild() {
	CreateAreas();
	FNavigationSystem::UpdateActorData(*this);
}

// Called when the game starts or when spawned
void AANavMeshSpline::BeginPlay() {
	Super::BeginPlay();

}

void AANavMeshSpline::GetNavigationData(FNavigationRelevantData& Data) const {
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("Grabbing Nav Data for %i!"), Areas.Num()));
	if (Areas.Num() <= 0) {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("NavAreas empty! Rebuild pls."));
		return;
	}

	float length = Spline->GetSplineLength() / Segments;

	for(int32 i = 0; i < Segments; i++) {
		FVector StartPoint = Spline->GetLocationAtDistanceAlongSpline(length * i, ESplineCoordinateSpace::World);
		FVector EndPoint = Spline->GetLocationAtDistanceAlongSpline(length * (i + 1), ESplineCoordinateSpace::World);
		FVector StartRight = Spline->GetRightVectorAtDistanceAlongSpline(length * i, ESplineCoordinateSpace::World);
		FVector EndRight = Spline->GetRightVectorAtDistanceAlongSpline(length * (i + 1), ESplineCoordinateSpace::World);

		TArray<FVector> Points;
		Points.Add(StartPoint + Width * StartRight);
		Points.Add(StartPoint - Width * StartRight);
		Points.Add(StartPoint + Width * StartRight + ZOffset);
		Points.Add(StartPoint - Width * StartRight + ZOffset);
		Points.Add(EndPoint + Width * EndRight);
		Points.Add(EndPoint - Width * EndRight);
		Points.Add(EndPoint + Width * EndRight + ZOffset);
		Points.Add(EndPoint - Width * EndRight + ZOffset);

		FAreaNavModifier Area = FAreaNavModifier(Points, ENavigationCoordSystem::Unreal, FTransform::Identity, AreaClass);
		Data.Modifiers.Add(Area);

	}
}

bool AANavMeshSpline::IsNavigationRelevant() const {
	return true;
}

void AANavMeshSpline::UpdateNavigationBounds() {
	CreateAreas();
}

FBox AANavMeshSpline::GetNavigationBounds() const {
	return Bounds;
}

#if WITH_EDITOR
bool AANavMeshSpline::IsEditorOnly() const {
	return true;
}

void AANavMeshSpline::PostEditMove(bool bFinished) {
	if(bFinished) {
		Rebuild();
	}
}

void AANavMeshSpline::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) {

	static FName NAME_Width = GET_MEMBER_NAME_CHECKED(AANavMeshSpline, Width);
	static FName NAME_Area = GET_MEMBER_NAME_CHECKED(AANavMeshSpline, AreaClass);
	static FName NAME_ZOffset = GET_MEMBER_NAME_CHECKED(AANavMeshSpline, ZOffset);
	static FName NAME_SEGMENTS = GET_MEMBER_NAME_CHECKED(AANavMeshSpline, Segments);

	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName PropName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if(PropName == NAME_Width) {
		Spline->ScaleVisualizationWidth = Width;
		Rebuild();
	} else if (PropName == NAME_Area) {
		FNavigationSystem::UpdateActorData(*this);
	} else if (PropName == NAME_ZOffset) {
		Rebuild();
	} else if (PropName == NAME_SEGMENTS) {
		Rebuild();
	}
}
#endif

void AANavMeshSpline::CreateAreas() {
	if(!Spline) {
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Spline missing! Oh no!"));
		return;
	}

	Areas.Empty();
	Bounds.Init();


	float length = Spline->GetSplineLength() / Segments;

	for(int32 i = 0; i < Segments; i++) {
		FVector StartPoint = Spline->GetLocationAtDistanceAlongSpline(length * i, ESplineCoordinateSpace::World);
		FVector EndPoint = Spline->GetLocationAtDistanceAlongSpline(length * (i + 1), ESplineCoordinateSpace::World);
		FVector StartRight = Spline->GetRightVectorAtDistanceAlongSpline(length * i, ESplineCoordinateSpace::World);
		FVector EndRight = Spline->GetRightVectorAtDistanceAlongSpline(length * (i + 1), ESplineCoordinateSpace::World);

		TArray<FVector> Points;
		Points.Add(StartPoint + Width * StartRight);
		Points.Add(StartPoint - Width * StartRight);
		Points.Add(StartPoint + Width * StartRight + ZOffset);
		Points.Add(StartPoint - Width * StartRight + ZOffset);
		Points.Add(EndPoint + Width * EndRight);
		Points.Add(EndPoint - Width * EndRight);
		Points.Add(EndPoint + Width * EndRight + ZOffset);
		Points.Add(EndPoint - Width * EndRight + ZOffset);

		Areas.Add(Points);

		for (auto Point : Points) {
			Bounds += Point;
		}

	}

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Created %i Areas!"), Areas.Num()));
}

//----------------------------------------------------------------------//
// This code is mostly copied from ANavModifierVolume.cpp
//----------------------------------------------------------------------//

#if WITH_EDITOR
namespace UE::Navigation::ModVolume::Private
{
	void OnNavAreaRegistrationChanged(AANavMeshSpline& AANavMeshSpline, const UWorld& World, const UClass* NavAreaClass)
	{
		if (NavAreaClass && NavAreaClass == AANavMeshSpline.AreaClass && &World == AANavMeshSpline.GetWorld())
		{
			FNavigationSystem::UpdateActorData(AANavMeshSpline);
		}
	}
} // UE::Navigation::ModVolumne::Private
#endif // WITH_EDITOR

void AANavMeshSpline::PostInitProperties()
{
	Super::PostInitProperties();

#if WITH_EDITOR
	if (GIsEditor && !HasAnyFlags(RF_ClassDefaultObject))
	{
		OnNavAreaRegisteredDelegateHandle = UNavigationSystemBase::OnNavAreaRegisteredDelegate().AddUObject(this, &AANavMeshSpline::OnNavAreaRegistered);
		OnNavAreaUnregisteredDelegateHandle = UNavigationSystemBase::OnNavAreaUnregisteredDelegate().AddUObject(this, &AANavMeshSpline::OnNavAreaUnregistered);
	}
#endif // WITH_EDITOR
}

void AANavMeshSpline::BeginDestroy() {
	Super::BeginDestroy();
#if WITH_EDITOR
	if (GIsEditor && !HasAnyFlags(RF_ClassDefaultObject))
	{
		UNavigationSystemBase::OnNavAreaRegisteredDelegate().Remove(OnNavAreaRegisteredDelegateHandle);
		UNavigationSystemBase::OnNavAreaUnregisteredDelegate().Remove(OnNavAreaUnregisteredDelegateHandle);
	}
#endif
}

#if WITH_EDITOR
void AANavMeshSpline::PostEditUndo() {
	Super::PostEditUndo();
	Rebuild();
}

// This function is only called if GIsEditor == true
void AANavMeshSpline::OnNavAreaRegistered(const UWorld& World, const UClass* NavAreaClass)
{
	UE::Navigation::ModVolume::Private::OnNavAreaRegistrationChanged(*this, World, NavAreaClass);
}

// This function is only called if GIsEditor == true
void AANavMeshSpline::OnNavAreaUnregistered(const UWorld& World, const UClass* NavAreaClass)
{
	UE::Navigation::ModVolume::Private::OnNavAreaRegistrationChanged(*this, World, NavAreaClass);
}
#endif // WITH_EDITOR