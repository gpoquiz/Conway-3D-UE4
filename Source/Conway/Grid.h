// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Cell.h"
#include "UObject/UObjectArray.h"
#include "Materials/MaterialInstanceDynamic.h"
#include <Runtime\Engine\Classes\Materials\MaterialExpressionConstant.h>
//#include <Editor\UnrealEd\Classes\Factories\MaterialFactoryNew.h>
#include <Runtime\AssetRegistry\Public\AssetRegistryModule.h>
#include <Runtime\Engine\Classes\Materials\MaterialExpressionVectorParameter.h>
#include "Runtime/Core/Public/Async/ParallelFor.h"
#include "Grid.generated.h"

UCLASS()
class CONWAY_API AGrid : public AActor
{
	GENERATED_BODY()
private:
	void redrawHelper(int d1, int d2, int d3, float hue);
	int countAliveAdjacencies(int d1, int d2, int d3);
public:	
	// Sets default values for this actor's properties
	// When I want to make this modular, add a Tarray of ints for the cell dimensions.
	// Then you can iterate over them for your dimensions
	AGrid();
	UFUNCTION(BlueprintCallable, Category = Next)
		void checkGame();
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = vars)
		int32 count = 20;
	UPROPERTY(EditAnywhere, Category = vars)
		TArray<int> bornConds;
	UPROPERTY(EditAnywhere, Category = vars)
		TArray<int> dieConds;
	UPROPERTY(EditAnywhere, Category = vars)
		TArray<int> dimensions;
	UPROPERTY(EditAnywhere, Transient, BlueprintReadWrite, Category = vars)
		TArray<UCell*> cells;
	UFUNCTION(CallInEditor, Category = vars)
		void redraw();
	UPROPERTY(EditAnywhere, Category = vars)
		UMaterial* cellMat;
	UPROPERTY(EditAnywhere, Category = vars)
		float period = .25;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
//namespace AdjacencyCounter
//{
//	static int countAliveAdjacencies(TArray<UCell*> cells, int count, int d3, int d2, int d1)
//	{
//		int adjCount = 0;
//		for (int i = -1; i <= 1; i++)
//			for (int j = -1; j <= 1; j++)
//				for (int k = -1; k <= 1; k++)
//				{
//					int adjD3 = d3 + i;
//					int adjD2 = d2 + j;
//					int adjD1 = d1 + k;
//
//					int adjIndex = (adjD3)*count * count + (adjD2)*count + adjD1;
//					if (adjIndex >= 0 && adjIndex < count * count * count
//						&& !(i == 0 && j == 0 && k == 0)
//						&& adjD3 >= 0 && adjD2 >= 0 && adjD1 >= 0
//						&& adjD3 <= count - 1 && adjD2 <= count - 1 && adjD1 <= count - 1
//						&& cells[adjIndex]->isAlive)
//					{
//						adjCount++;
//					}
//				}
//		return adjCount;
//	}
//}
//class AsyncCalculation : public FNonAbandonableTask
//{
//	TArray<UCell*> cells;
//	int count;
//public:
//	AsyncCalculation(TArray<UCell*> cells, int count)
//	{
//		this->cells = cells;
//		this->count = count;
//	}
//	FORCEINLINE TStatId GetStatId() const
//	{
//		RETURN_QUICK_DECLARE_CYCLE_STAT(AsyncCalculation, STATGROUP_ThreadPoolAsyncTasks);
//	}
//}
//	void DoWork()
//	{
//		AdjacencyCounter::countAliveAdjacencies(TArray<UCell*> cells, int count, int d3, int d2, int d1);
//
//		GLog->Log("--------------------------------------------------------------------");
//		GLog->Log("End of prime numbers calculation on background thread");
//		GLog->Log("--------------------------------------------------------------------");
//	}
//};