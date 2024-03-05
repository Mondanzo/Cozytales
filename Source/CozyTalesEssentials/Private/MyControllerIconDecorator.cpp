// Fill out your copyright notice in the Description page of Project Settings.


#include "MyControllerIconDecorator.h"

const FSlateBrush* UMyControllerIconDecorator::FindImageBrush(FName TagOrId, bool bWarnIfMissing) {
	IconRequest();
	return Super::FindImageBrush(TagOrId, bWarnIfMissing);
}

void UMyControllerIconDecorator::SetControllerSet(FString NewSet) {
	if (ImageSets.Contains(NewSet)) {
		ImageSet = ImageSets[NewSet];
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Failed to find %s!"), *NewSet)
	}
}