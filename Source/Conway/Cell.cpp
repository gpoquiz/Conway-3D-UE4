// Fill out your copyright notice in the Description page of Project Settings.


#include "Cell.h"

UCell::UCell()
{
    // Our root component will be a sphere that reacts to physics

    //this->SetCollisionProfileName(TEXT("NoCollision"));
    //this->SetCollisionEnabled(ECollisionEnabled::)
    this->SetGenerateOverlapEvents(false);
    static ConstructorHelpers::FObjectFinder<UStaticMesh> BoxVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube"));
    static ConstructorHelpers::FObjectFinder<UMaterialInterface> BoxTexture(TEXT("/Game/HollowOutline"));
    static ConstructorHelpers::FObjectFinder<UMaterialInterface> EmptyTexture(TEXT("/Game/Empty"));

    if (BoxVisualAsset.Succeeded() && BoxTexture.Succeeded())
    {
        this->SetStaticMesh(BoxVisualAsset.Object);
        this->SetMaterial(0, BoxTexture.Object);
        this->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
        //aliveMat = BoxTexture.Object;
        //deadMat = EmptyTexture.Object;
        //this->SetWorldScale3D(FVector(0.1f));
        //SphereVisual->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        //SphereVisual->SetGenerateOverlapEvents(false);
    }

    this->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    this->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    this->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
    //this->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
    
}
void UCell::Toggle()
{
    willToggle = false;
    isAlive = !isAlive;
    if (isAlive)
    {
        this->SetVisibility(true);
        this->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);

    }
    else
    {
        this->SetVisibility(false);
        this->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);

    }
}
void UCell::checkStatus()
{
    if (willToggle)
    {
        Toggle();
    }

}
// Called when the game starts or when spawned
void UCell::BeginPlay()
{
    Super::BeginPlay();

}