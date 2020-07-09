// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid.h"

// Sets default values
AGrid::AGrid()
{
    count = 5;
    dimensions.Add(count);
    dimensions.Add(count);
    dimensions.Add(count);

    for (int i = 0; i <= 3; i++)
        dieConds.Add(i);

    for (int i = 5; i <= 5; i++)
        bornConds.Add(i);
    for (int i = 6; i <= 26; i++)
        dieConds.Add(i);

    // bornConds.Add(3);
     // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    UBoxComponent* BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("RootComponent"));
    RootComponent = BoxComponent;
    this->SetActorRelativeScale3D(FVector(.5f));
    this->redraw();

    //cells[count / 2 * count * count + count / 2 * count + count / 2]->Toggle();
}
void AGrid::redraw()
{
    cells.Empty();
    int max = 1.0;
    for (int i : dimensions)
    {
        max *= i;
    }

    cells.Reserve(max);
    double hue = 0.0;
    double interval = 180.0 / (max);
    for (int32 dx = 0; dx < max; dx++)
    {
        hue += interval;
        redrawHelper(dx, hue);
    }
}
void AGrid::redrawHelper(int32 dx, float hue)
{
    
    FString namestring = FString(TEXT(""));
    TArray<int> coords;
    int index = dx;
    int div = 1;
    coords.Add(dx % dimensions[0]);

    for (int d = 1; d < dimensions.Num(); d++)
    {
        div *= dimensions[d-1];
        coords.Add(dx / div  % dimensions[d]);
        /*
        if (index < dimensions[d])
        {
            coords.Add(0);
        }
        else
        {
            coords.Add((index/div) % dimensions[d]);
        }
        div *= dimensions[d];*/
    }

    /*
    TArray<int> newCoords;
    for (int c = coords.Num()-1; c >= 0; c--)
    {
        newCoords.Add(coords[c]);
    }
    coords = newCoords;
    *//*
    for (int d : dimensions)
    {
        if (index < d)
        {
            coords.Add(0);
        }
        coords.Add(index % d);
        index /= d;

    }*/
    
    for (int c : coords)
    {
        namestring = namestring + FString::FromInt(c);
    }
    FName name(*namestring);
    //UE_LOG(LogTemp, Warning, TEXT("%s"), *name.ToString());

    UCell* NewComp = NewObject<UCell>(RootComponent, name);

    NewComp->SetupAttachment(RootComponent);
    UMaterial* curMat = NewComp->GetMaterial(0)->GetMaterial();
    UMaterialInstanceDynamic* mat = UMaterialInstanceDynamic::Create(NewComp->GetMaterial(0), NewComp);
    //UE_LOG(LogTemp, Log, TEXT("%s"), *(NewComp->GetTransform().ToString()));

    NewComp->SetRelativeLocation(FVector(coords[0] * 100, coords[1] * 100/* + (d4 * (count * 100 + 100))*/, coords[2] * 100));
    NewComp->coords = coords;
    NewComp->x = coords[0];
    NewComp->y = coords[1];
    NewComp->z = coords[2];
    FLinearColor hsv = FLinearColor::MakeFromHSV8((uint8)hue, (uint8)255, (uint8)255);
    //points[i]->GetMaterial(0)->GetMaterial()->Opacity

    mat->SetVectorParameterValue(TEXT("Color"), hsv);
    //NewComp->aliveMat = mat;
    //NewComp->aliveMat = mat;
    //TODO why can't I do NewComp->SetMaterial(0, NewComp->aliveMat);

    NewComp->SetMaterial(0, mat);
    //if (NewComp->isAlive)
    //    NewComp->SetMaterial(0, NewComp->aliveMat);
    //else NewComp->SetMaterial(0, NewComp->deadMat);

    NewComp->Toggle();
    cells.Add(NewComp);
}
// Called when the game starts or when spawned
void AGrid::BeginPlay()
{
    Super::BeginPlay();

}

int AGrid::countAliveAdjacencies(int d3, int d2, int d1)
{
    int adjCount = 0;
    for (int i = -1; i <= 1; i++)
        for (int j = -1; j <= 1; j++)
            for (int k = -1; k <= 1; k++)
            {
                int32 adjD3 = d3 + i;
                int32 adjD2 = d2 + j;
                int32 adjD1 = d1 + k;

                int32 adjIndex = (adjD3)*count * count + (adjD2)*count + adjD1;
                if (adjIndex >= 0 && adjIndex < count * count * count
                    && !(i == 0 && j == 0 && k == 0)
                    && adjD3 >= 0 && adjD2 >= 0 && adjD1 >= 0
                    && adjD3 <= count - 1 && adjD2 <= count - 1 && adjD1 <= count - 1
                    && cells[adjIndex]->isAlive)
                {
                    adjCount++;
                }
            }
    return adjCount;
}
//check the game for updates
void AGrid::checkGame()
{
    ParallelFor(count, [&](uint32 d3) {
        ParallelFor(count, [&](uint32 d2) {
            ParallelFor(count, [&](uint32 d1) {
                int index = d3 * count * count + d2 * count + d1;
                UCell* cell = cells[index];

                int adj = countAliveAdjacencies(d3, d2, d1);
                for (int b : bornConds)
                    if (adj == b && !cell->isAlive)
                        cell->willToggle = true;
                for (int d : dieConds)
                    if (adj == d && cell->isAlive)
                        cell->willToggle = true;
                });
            });
        });
    /*   for (int d3 = 0; d3 < count; d3++)
           for (int d2 = 0; d2 < count; d2++)
               for (int d1 = 0; d1 < count; d1++)
               {
                   int index = d3 * count * count + d2 * count + d1;
                   UCell* cell = cells[index];

                   int adj = countAliveAdjacencies(d3, d2, d1);
                   if (adj <= 8)
                   {
                       UE_LOG(LogTemp, Warning, TEXT("Toggling: %d"), adj);
                   }
                   for (int b : bornConds)
                       if (adj == b && !cell->isAlive)
                           cell->willToggle = true;
                   for (int d : dieConds)
                       if (adj == d && cell->isAlive)
                           cell->willToggle = true;
               }*/
    FCriticalSection Mutex;

    ParallelFor(count * count * count, [&](int32 cell) {
        Mutex.Lock();
        cells[cell]->checkStatus();
        Mutex.Unlock();
        });
    /*
    ParallelFor(count* count* count, [&](int32 cell) {

        if (cells[cell]->willToggle)
        {
            cells[cell]->willToggle = false;
            cells[cell]->isAlive = !cells[cell]->isAlive;
            if (cells[cell]->isAlive)
            {
                Mutex.Lock();
                cells[cell]->SetVisibility(true);
                Mutex.Unlock();

            }
            else
            {
                Mutex.Lock();
                cells[cell]->SetVisibility(false);
                Mutex.Unlock();
            }
        }
        });*/
        //for (UCell* cell : cells)
        //    cell->checkStatus();
}
float timeAgg = 0.0;
// Called every frame
void AGrid::Tick(float DeltaTime)
{
    /*
    Super::Tick(DeltaTime);
    timeAgg += DeltaTime;
    if (timeAgg >= period)
    {
        //UE_LOG(LogTemp, Warning, TEXT("Checking Game: %f"), timeAgg);
        timeAgg = 0;
        checkGame();
    }*/
}

