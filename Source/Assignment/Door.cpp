// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TimelineComponent.h"

// Sets default values
ADoor::ADoor()
{
    //Create our Default Components
    DoorFrame = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorFrameMesh"));
    Door = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
    DoorTimelineComp = CreateDefaultSubobject<UTimelineComponent>(TEXT("DoorTimelineComp"));

    //Setup our Attachments
    DoorFrame->SetupAttachment(RootComponent);
    Door->AttachToComponent(DoorFrame, FAttachmentTransformRules::KeepRelativeTransform);
    Door->SetRelativeLocation(FVector(0, 35, 0));

}

void ADoor::OnInteract()
{
    DoorTimelineComp->Play();
}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();
	
    //Binding our float track to our UpdateTimelineComp Function's output
    UpdateFunctionFloat.BindDynamic(this, &ADoor::UpdateTimelineComp);

    //If we have a float curve, bind its graph to our update function
    if (DoorTimelineFloatCurve)
    {
        DoorTimelineComp->AddInterpFloat(DoorTimelineFloatCurve, UpdateFunctionFloat);
    }
}

void ADoor::UpdateTimelineComp(float Output)
{
    // Create and set our Door's new relative location based on the output from our Timeline Curve
    FRotator DoorNewRotation = FRotator(0.0f, Output, 0.f);
    Door->SetRelativeRotation(DoorNewRotation);
}

