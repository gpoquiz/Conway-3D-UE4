// Fill out your copyright notice in the Description page of Project Settings.


#include "Grid.h"

// Sets default values
AGrid::AGrid()
{
    count = 5;
    dimensions.Add(5);
    dimensions.Add(5);
    dimensions.Add(5);
    dimensions.Add(5);
    //dimensions.Add(count);

    for (int i = 0; i <= 3; i++)
        dieConds.Add(i);

    for (int i = 5; i <= 5; i++)
        bornConds.Add(i);
    for (int i = 6; i <= 80; i++)
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
int AGrid::coordsToIndex(TArray<int> coords)
{
    int index = 0;
    int offset = 1;
    for (int i = 0; i < dimensions.Num(); i++)
    {
        index += coords[i] * offset;
        offset *= dimensions[i];
    }
    return index;
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
        div *= dimensions[d - 1];
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

    TArray<float> location;
    location.Add(0.0);
    location.Add(0.0);
    location.Add(0.0);
    for (int i = 0; i < coords.Num(); i++)
    {
        if (i < 3)
        {
            location[i % 3] = coords[i] * 100;
        }
        else
        {
            location[i % 3] += coords[i] * (dimensions[i%3] * 100  + 100);
        }
    }
    NewComp->SetRelativeLocation(FVector(location[0], location[1], location[2]));
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

int AGrid::countAliveAdjacencies(UCell* cell)
{

    TArray<int> coords = cell->coords;
    int adjacencyCount = 1;
    for (int d : dimensions){adjacencyCount *= 3;}

    int aliveCount = 0;
    for (int adjIndex = 0; adjIndex <= adjacencyCount - 1; adjIndex++)
    {

        // skip self
        if (adjIndex == adjacencyCount / 2)
            continue;
        int div = 1;

        TArray<int> adj;
        for (int d = 0; d < dimensions.Num(); d++)
        {
            int offset = adjIndex / div % 3 - 1;
            adj.Add(coords[d] + offset);
            div *= 3;
        }
        
        bool validIndex = true;
        for (int i = 0; i < dimensions.Num(); i++)
        {
            if (adj[i] < 0 || adj[i] >= dimensions[i])
                validIndex = false;
        }
        int index = coordsToIndex(adj);

        if (validIndex && cells[index]->isAlive)
        {
            aliveCount++;
        }
        //UE_LOG(LogTemp, Warning, TEXT("Checking Game: %d"), w);
        FString f;

        // UE_LOG(LogTemp, Warning, TEXT("Checking Game: %d%d%d"), adj[0], adj[1], adj[2]);
    }
    return aliveCount;
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
    int max = 1;
    for (int i : dimensions)
    {
        max *= i;
    }
    ParallelFor(max, [&](uint32 index)
    {
            UCell* cell = cells[index];

            int adj = countAliveAdjacencies(cell);
            UE_LOG(LogTemp, Warning, TEXT("%d"), adj);
            for (int b : bornConds)
                if (adj == b && !cell->isAlive)
                    cell->willToggle = true;
            for (int d : dieConds)
                if (adj == d && cell->isAlive)
                    cell->willToggle = true;
    });

    FCriticalSection Mutex;
    ParallelFor(max, [&](int32 cell) {
        Mutex.Lock();
        cells[cell]->checkStatus();
        Mutex.Unlock();
        });
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

