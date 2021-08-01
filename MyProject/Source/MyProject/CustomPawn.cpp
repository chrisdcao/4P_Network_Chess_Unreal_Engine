// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomPawn.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>

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
	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT(""));
	RootComponent->SetWorldLocationAndRotation(FVector(2800.f, 0.f, 50.f), FRotator(-60.f, 0.f, 0.f));
	StaticMeshComp->SetupAttachment(RootComponent);
	StaticMeshComp->SetStaticMesh(MeshAsset.Object);
	SpringArmComp->SetupAttachment(StaticMeshComp);
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);

	SpringArmComp->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 50.0f), FRotator(-60.0f, 0.0f, 0.0f));
	SpringArmComp->TargetArmLength = 400.f;
	SpringArmComp->bEnableCameraLag = true;
	SpringArmComp->CameraLagSpeed = 3.f;
	SpringArmComp->bDoCollisionTest = false;

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called to bind functionality to input
void ACustomPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAction("ZoomIn", IE_Pressed, this, &ACustomPawn::ZoomIn);
	InputComponent->BindAction("ZoomIn", IE_Released, this, &ACustomPawn::ZoomOut);

	//Hook up every-frame handling for our four axes
	InputComponent->BindAxis("MoveForward", this, &ACustomPawn::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ACustomPawn::MoveRight);
	InputComponent->BindAxis("CameraPitch", this, &ACustomPawn::PitchCamera);
	InputComponent->BindAxis("CameraYaw", this, &ACustomPawn::YawCamera);
}

void ACustomPawn::MoveForward(float AxisValue)
{
	MovementInput.X = FMath::Clamp<float>(AxisValue, -1.f, 1.f);
}

void ACustomPawn::MoveRight(float AxisValue)
{
	MovementInput.Y = FMath::Clamp<float>(AxisValue, -1.f, 1.f);
}

void ACustomPawn::PitchCamera(float AxisValue)
{
	CameraInput.Y = AxisValue;
}

void ACustomPawn::YawCamera(float AxisValue)
{
	CameraInput.X = AxisValue;
}

void ACustomPawn::ZoomIn()
{
	bZoomingIn = true;
}

void ACustomPawn::ZoomOut()
{
	bZoomingIn = false;
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

	//Zoom in if ZoomIn button is down, zoom back out if it's not
	{
		if (bZoomingIn)
			ZoomFactor += DeltaTime / 0.5f;         //Zoom in over half a second
		else
			ZoomFactor -= DeltaTime / 0.25f;        //Zoom out over a quarter of a second
		ZoomFactor = FMath::Clamp<float>(ZoomFactor, 0.0f, 1.0f);

		//Blend our camera's FOV and our SpringArm's length based on ZoomFactor
		CameraComp->FieldOfView = FMath::Lerp<float>(90.0f, 60.0f, ZoomFactor);
		SpringArmComp->TargetArmLength = FMath::Lerp<float>(400.0f, 300.0f, ZoomFactor);
	}

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
            NewLocation += GetActorForwardVector() * MovementInput.X * DeltaTime;
            NewLocation += GetActorRightVector() * MovementInput.Y * DeltaTime;
            SetActorLocation(NewLocation);
        }
    }
}
