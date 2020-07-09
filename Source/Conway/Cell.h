// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Cell.generated.h"

/**
 * 
 */
UCLASS()
class CONWAY_API UCell : public UStaticMeshComponent
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, CallInEditor, Category = Toggle)
		void Toggle();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Transient)
		bool isAlive = false;
	UPROPERTY(EditAnywhere, Transient)
		bool willToggle = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Location)
		TArray<int> coords;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Location)
		int x;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Location)
		int y;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Location)
		int z;
	//UPROPERTY(Transient)
	//	UMaterialInterface* aliveMat = nullptr;
	//UPROPERTY(Transient)
	//	UMaterialInterface* deadMat = nullptr;
	UCell();
	void checkStatus();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
