// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomPawn.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>

/* THINK OF IMPLENTING ALL THE PRESSED AND RELEASE WITH GATE LOGIC, SO THAT WE CAN REMOVE TICK AND SEE IF THE PERFORMANCE IMPROVES */
// Sets default values
ACustomPawn::ACustomPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CameraPhysicalDebug"));
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));

	// add a cube from the starter content to this place
	RootComponent->SetWorldLocationAndRotation(FVector(2800.f, 0.f, 50.f), FRotator(0.f, 0.f, 0.f));
	StaticMeshComp->SetupAttachment(RootComponent);
	/// CAN LATER ADD THE CUBE OR SOMETHING TO DEBUG IF NEEDED
	// static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT(""));
	// StaticMeshComp->SetStaticMesh(MeshAsset.Object);
	SpringArmComp->SetupAttachment(StaticMeshComp);
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);

	SpringArmComp->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 50.0f), FRotator(-60.0f, 0.0f, 0.0f));
	SpringArmComp->TargetArmLength = 400.f;
	//SpringArmComp->bEnableCameraLag = true;
	//SpringArmComp->CameraLagSpeed = 3.f;
	SpringArmComp->bDoCollisionTest = false;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// SmoothZoom Defaults -- Change inside of your BP where the actor component is located
	DesiredArmLength = SpringArmComp->TargetArmLength;
	MinTargetLength = 300.0f;
	MaxTargetLength = 4000.0f;
	ZoomUnits = 150.0f;
	ZoomSmoothness = 9.0f;
	// Gate Logic Boolean
	bIsOpen = false;

}

// Called to bind functionality to input
void ACustomPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAction("SmoothZoomIn", IE_Pressed, this, &ACustomPawn::SmoothZoomIn);
	InputComponent->BindAction("SmoothZoomOut", IE_Pressed, this, &ACustomPawn::SmoothZoomOut);
	//Hook up every-frame handling for our four axes
	InputComponent->BindAxis("MoveForward", this, &ACustomPawn::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ACustomPawn::MoveRight);
	InputComponent->BindAxis("MoveHigher", this, &ACustomPawn::MoveHigher);

/*	
	* Using Blueprint's Gate Logic, where: if {Gate Open} then {Receive input} else {Stop Receive input}
	* Apply here: Right Mouse Button 'Pressed' state triggers Gate Open 
			      Right Mouse Button 'Released' state triggers Gate Close
	-> Only during this time between 'Open' and 'Close' states, we accept receival of Axis Keybindings
	-> Defaultly the Gate is Close, Left Mouse Button trigger is not set to open the Gate and thus will not be involved in this Axis Keybinding events 
*/
	InputComponent->BindAction("Pan", IE_Pressed, this, &ACustomPawn::GateOpen);
	InputComponent->BindAction("Pan", IE_Released, this, &ACustomPawn::GateClose);
	/* We just have to set the trigger keys trung` nhau (i.e. RMB <Pressed until Release> == RMB <Hold>) and it will feel like all 4 functions are activated through only 1 Action */
	InputComponent->BindAxis("CameraPitch", this, &ACustomPawn::PitchCamera);
	InputComponent->BindAxis("CameraYaw", this, &ACustomPawn::YawCamera);
}

void ACustomPawn::GateOpen()
{
	bIsOpen = true;
}

void ACustomPawn::GateClose()
{
	bIsOpen = false;
}

void ACustomPawn::SmoothZoomIn()
{
	UE_LOG(LogTemp, Warning, TEXT("SmoothZoomIn is called!"));
	DesiredArmLength = SpringArmComp->TargetArmLength + ZoomUnits * -1 * 2;

	if (DesiredArmLength > MaxTargetLength || DesiredArmLength < MinTargetLength)
		DesiredArmLength = FMath::Min<float>(FMath::Max<float>(DesiredArmLength, MinTargetLength), MaxTargetLength);
}

void ACustomPawn::SmoothZoomOut()
{
	UE_LOG(LogTemp, Warning, TEXT("SmoothZoomOut is called!"));
	DesiredArmLength = SpringArmComp->TargetArmLength + ZoomUnits * 2;

	if (DesiredArmLength > MaxTargetLength || DesiredArmLength < MinTargetLength)
		DesiredArmLength = FMath::Min<float>(FMath::Max<float>(DesiredArmLength, MinTargetLength), MaxTargetLength);
}

// MODIFY THESE TO CHANGE THE MOVEFORWARD AND MOVERIGHT SPEED
void ACustomPawn::MoveForward(float AxisValue)
{
	MovementInput.X = FMath::Clamp<float>(AxisValue, -1.f, 1.f);
}

void ACustomPawn::MoveRight(float AxisValue)
{
	MovementInput.Y = FMath::Clamp<float>(AxisValue, -1.f, 1.f);
}

void ACustomPawn::MoveHigher(float AxisValue)
{
	MovementInput.Z = FMath::Clamp<float>(AxisValue, -1.f, 1.f);
}

// MODIFY THESE TO CHANGE THE CAMERA RATE OF CHANGE SPEED
void ACustomPawn::PitchCamera(float AxisValue)
{
	if (bIsOpen) CameraInput.Y = AxisValue;
	else		 CameraInput.Y = 0.f;
}

void ACustomPawn::YawCamera(float AxisValue)
{
	if (bIsOpen) CameraInput.X = AxisValue;
	else		 CameraInput.X = 0.f;
}

// Called when the game starts or when spawned
void ACustomPawn::BeginPlay()
{
	Super::BeginPlay();
	PlayerController = Cast<ACustomPlayerController>(GetController());
}

// Call every frame
void ACustomPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//if (!ensure(SpringArmComp)) return;

	//if (!FMath::IsNearlyEqual(SpringArmComp->TargetArmLength, DesiredArmLength, 0.5f)) 
		//SpringArmComp->TargetArmLength = FMath::FInterpTo(SpringArmComp->TargetArmLength, DesiredArmLength, DeltaTime, ZoomSmoothness);

    //Rotate our actor's yaw, which will turn our camera because we're attached to it
    {
        FRotator NewRotation = GetActorRotation();
        NewRotation.Yaw += CameraInput.X;
        SetActorRotation(NewRotation);
    }

    //Rotate our camera's pitch, but limit it so we're always looking downward
    {
        FRotator NewRotation = SpringArmComp->GetComponentRotation();
        NewRotation.Pitch = FMath::Clamp(NewRotation.Pitch + CameraInput.Y, -80.0f, -15.0f);
        SpringArmComp->SetWorldRotation(NewRotation);
    }

    //Handle movement based on our "MoveX" and "MoveY" axes
    {
        if (!MovementInput.IsZero())
        {
            //Scale our movement input axis values by 100 units per second
            MovementInput = MovementInput.GetSafeNormal() * 100.0f;
            FVector NewLocation = GetActorLocation();
            NewLocation += GetActorForwardVector() * MovementInput.X * DeltaTime * 15;
            NewLocation += GetActorRightVector() * MovementInput.Y * DeltaTime * 15;
            NewLocation += GetActorUpVector() * MovementInput.Z * DeltaTime * 12;
            SetActorLocation(NewLocation);
        }
    }
}
