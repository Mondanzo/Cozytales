// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/RichTextBlockImageDecorator.h"
#include "MyControllerIconDecorator.generated.h"

/**
 * 
 */
UCLASS()
class COZYTALESESSENTIALS_API UMyControllerIconDecorator : public URichTextBlockImageDecorator
{
	GENERATED_BODY()
	
public:
	void URichTextBlockImageDecorator();
	virtual const FSlateBrush* FindImageBrush(FName TagOrId, bool bWarnIfMissing) override;

	UFUNCTION(BlueprintCallable)
		virtual void SetControllerSet(FString new_set);

	UFUNCTION(BlueprintImplementableEvent)
		void IconRequest();

	UPROPERTY(EditAnywhere, Category = Appearance, meta = (RequiredAssetDataTags = "RowStructure=/Script/UMG.RichImageRow"))
		TMap<FString, TObjectPtr<class UDataTable>> ImageSets;
};
