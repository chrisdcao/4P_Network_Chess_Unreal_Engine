// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawn.h"
#include "GameFramework/Pawn.h"
#include "CustomGameState.h"
#include "CustomPlayerState.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

// TODO: functionalize all the moves to implement Server-Client model
APlayerPawn::APlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    bReplicates = true;
    bAlwaysRelevant = true;
    bNetUseOwnerRelevancy = true;

    // defaultly = -1 so that from player5 onward cannot manipulate the game
    // TODO: this is just a temporary way to prevent > 4 players. Later will do session and match making condition

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CameraPhysicalDebug"));
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));

	// add a cube from the starter content to this place
	RootComponent->SetWorldLocationAndRotation(FVector(0.f, 0.f, 0.f), FRotator(0.f, 0.f, 0.f));
	StaticMeshComp->SetupAttachment(RootComponent);
	/// CAN LATER ADD THE CUBE OR SOMETHING TO DEBUG IF NEEDED
	// static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT(""));
	// StaticMeshComp->SetStaticMesh(MeshAsset.Object);
	SpringArmComp->SetupAttachment(StaticMeshComp);
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);

	//SpringArmComp->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 50.0f), FRotator(-60.0f, 0.0f, 0.0f));
    SpringArmComp->SetRelativeLocationAndRotation(FVector(140.f, 0.f, 60.f), FRotator(0.f, -30.f, 0.f));
	SpringArmComp->TargetArmLength = 400.f;
	SpringArmComp->bEnableCameraLag = true;
	SpringArmComp->CameraLagSpeed = 7.f;
	SpringArmComp->bDoCollisionTest = false;


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
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAction("SmoothZoomIn", IE_Pressed, this, &APlayerPawn::SmoothZoomIn);
	InputComponent->BindAction("SmoothZoomOut", IE_Pressed, this, &APlayerPawn::SmoothZoomOut);
	//Hook up every-frame handling for our four axes
	InputComponent->BindAxis("MoveForward", this, &APlayerPawn::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &APlayerPawn::MoveRight);
	InputComponent->BindAxis("MoveHigher", this, &APlayerPawn::MoveHigher);

/*	
	* Using Blueprint's Gate Logic, where: if {Gate Open} then {Receive input} else {Stop Receive input}
	* Apply here: Right Mouse Button 'Pressed' state triggers Gate Open 
			      Right Mouse Button 'Released' state triggers Gate Close
	-> Only during this time between 'Open' and 'Close' states, we accept receival of Axis Keybindings
	-> Defaultly the Gate is Close, Left Mouse Button trigger is not set to open the Gate and thus will not be involved in this Axis Keybinding events 
*/
	InputComponent->BindAction("Pan", IE_Pressed, this, &APlayerPawn::GateOpen);
	InputComponent->BindAction("Pan", IE_Released, this, &APlayerPawn::GateClose);
	/* We just have to set the trigger keys trung` nhau (i.e. RMB <Pressed until Release> == RMB <Hold>) and it will feel like all 4 functions are activated through only 1 Action */
	InputComponent->BindAxis("CameraPitch", this, &APlayerPawn::PitchCamera);
	InputComponent->BindAxis("CameraYaw", this, &APlayerPawn::YawCamera);
}

void APlayerPawn::OnLeftMouseClick(FVector hitLocation)
{
    // must include your OWN player state header for GetPlayerState() type to be completed
    // Must use GetPlayerId() from player state instead of playerController's NetplayerIndex as PlayerController is NOT replicated
    // only player state is Replicated!
    if (GetPlayerState()->GetPlayerId() == chessManager->playerTurn) 
    {
        chessManager->OnLeftMouseClick(hitLocation);
        Server_OnLeftMouseClick(hitLocation);
    }
}

void APlayerPawn::Server_OnLeftMouseClick_Implementation(FVector hitLocation)
{
    //UE_LOG(LogTemp, Warning, TEXT("Server_onLMB is fired"));
    chessManager->OnLeftMouseClick(hitLocation);
    Multicast_OnLeftMouseClick(hitLocation);
}

void APlayerPawn::Multicast_OnLeftMouseClick_Implementation(FVector hitLocation)
{
    chessManager->OnLeftMouseClick(hitLocation);
}

void APlayerPawn::GateOpen()
{
	bIsOpen = true;
}

void APlayerPawn::GateClose()
{
	bIsOpen = false;
}

void APlayerPawn::SmoothZoomIn()
{
	UE_LOG(LogTemp, Warning, TEXT("SmoothZoomIn is called!"));
	DesiredArmLength = SpringArmComp->TargetArmLength + ZoomUnits * -1 * 2;

	if (DesiredArmLength > MaxTargetLength || DesiredArmLength < MinTargetLength)
		DesiredArmLength = FMath::Min<float>(FMath::Max<float>(DesiredArmLength, MinTargetLength), MaxTargetLength);
}

void APlayerPawn::SmoothZoomOut()
{
	UE_LOG(LogTemp, Warning, TEXT("SmoothZoomOut is called!"));
	DesiredArmLength = SpringArmComp->TargetArmLength + ZoomUnits * 2;

	if (DesiredArmLength > MaxTargetLength || DesiredArmLength < MinTargetLength)
		DesiredArmLength = FMath::Min<float>(FMath::Max<float>(DesiredArmLength, MinTargetLength), MaxTargetLength);
}

// MODIFY THESE TO CHANGE THE MOVEFORWARD AND MOVERIGHT SPEED
void APlayerPawn::MoveForward(float AxisValue)
{
	MovementInput.X = FMath::Clamp<float>(AxisValue, -1.f, 1.f);
}

void APlayerPawn::MoveRight(float AxisValue)
{
	MovementInput.Y = FMath::Clamp<float>(AxisValue, -1.f, 1.f);
}

void APlayerPawn::MoveHigher(float AxisValue)
{
	MovementInput.Z = FMath::Clamp<float>(AxisValue, -1.f, 1.f);
}

// MODIFY THESE TO CHANGE THE CAMERA RATE OF CHANGE SPEED
void APlayerPawn::PitchCamera(float AxisValue)
{
	if (bIsOpen) CameraInput.Y = AxisValue;
	else		 CameraInput.Y = 0.f;
}

void APlayerPawn::YawCamera(float AxisValue)
{
	if (bIsOpen) CameraInput.X = AxisValue;
	else		 CameraInput.X = 0.f;
}

// Called when the game starts or when spawned
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();

    if (HasAuthority())
    { // how many times per second to server check a client and update that client
        NetUpdateFrequency = 1;
    }

    chessManager = Cast<ASpawnManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ASpawnManager::StaticClass()));
    //auto currentPlayerState = Cast<APlayerState>(GetPlayerState());
    //currentPlayerState->GetPlayerId();
}

FString GetEnumText(ENetRole Role)
{
    switch (Role)
    {
    case ROLE_None:
        return "None";
        // this role is from server Authority role down to Actor in that local machine that IS NOT controlled/simulated locally (not spawned from the world of local machine)
    case ROLE_SimulatedProxy:
        return "SimulatedProxy";
        // this role is from server Authority role down to Actor in that Local machine that IS controlled locally
        // Autonomous movement will NOT be propagated back down from the server (meaning that autonomous will not listen to server for its own movement), it will only upload the information to the server to update to other clients
    case ROLE_AutonomousProxy:
        return "AutonomousProxy";
        // all Actors on Server will be Authority Role
    case ROLE_Authority:
        return "Authority";
    default:
        return "ERROR";
    }
}

// Call every frame
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//DrawDebugString(GetWorld(), FVector(0, 0, 0), GetEnumText(GetLocalRole()), this, FColor::White, DeltaTime);

	//if (!ensure(SpringArmComp)) return;

	if (!FMath::IsNearlyEqual(SpringArmComp->TargetArmLength, DesiredArmLength, 0.5f)) 
		SpringArmComp->TargetArmLength = FMath::FInterpTo(SpringArmComp->TargetArmLength, DesiredArmLength, DeltaTime, ZoomSmoothness);

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
