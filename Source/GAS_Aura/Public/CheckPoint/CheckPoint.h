// Copyright GAS Aura Course

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "GAS_Aura/GAS_Aura.h"
#include "Interaction/HighlightInterface.h"
#include "Interaction/SaveInterface.h"
#include "CheckPoint.generated.h"

class USphereComponent;
/**
 * 
 */
UCLASS()
class GAS_AURA_API ACheckPoint : public APlayerStart , public ISaveInterface , public IHighlightInterface
{
	GENERATED_BODY()

public:
	ACheckPoint(const FObjectInitializer& ObjectInitializer);

	/* Save Interface */
	virtual bool ShouldLoadTransform_Implementation() override { return false; }
	virtual void LoadActor_Implementation() override;
	/* Save Interface */

	/* Highlight Interface */
	virtual void SetMoveToLocation_Implementation(FVector& OutDestination) override;
	virtual void HighlightActor_Implementation() override;
	virtual void UnHighlightActor_Implementation() override;
	/* Highlight Interface */

	UPROPERTY(BlueprintReadOnly , SaveGame)
	bool bReached = false;

protected:
	int32 CustomDepthStencilOverride = CUSTOM_DEPTH_TAN;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> MoveToComponent;
	
	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent , AActor* OtherActor , UPrimitiveComponent* OtherComp , int32 OtherBodyIndex , bool bFromSweep ,const FHitResult& SweepResult);

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void CheckPointReached(UMaterialInstanceDynamic* DynamicMaterialInstance);

	void HandleGlowEffects();

	UPROPERTY(VisibleAnywhere , BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> CheckPointMesh;
	
private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<USphereComponent> Sphere;
};
