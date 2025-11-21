// Copyright GAS Aura Course


#include "AbilitySystem/Abilities/AuraSummonAbility.h"

TArray<FVector> UAuraSummonAbility::GetSpawnLocations()
{
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	const float DeltaSpread = SpawnSpread / NumMinions;
	
	const FVector LeftOfSpread = Forward.RotateAngleAxis(-SpawnSpread / 2.f , FVector::UpVector);
	
	TArray<FVector> SpawnLocations;

	for (int32 Index = 0; Index < NumMinions; Index++)
	{
		const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * Index, FVector::UpVector);
		FVector SpawnLocation = Location + Direction * FMath::FRandRange(MinSpawnDistance, MaxSpawnDistance);

		FHitResult HitResult;
		GetWorld()->LineTraceSingleByChannel(
			HitResult,
			SpawnLocation + FVector(0.f , 0.f , 400.f) ,
			SpawnLocation - FVector(0.f , 0.f , 400.f) ,
			ECC_Visibility);

		if (HitResult.bBlockingHit)
		{
			SpawnLocation = HitResult.ImpactPoint;
		}
		
		SpawnLocations.Add(SpawnLocation);
	}
	
	return SpawnLocations;
}

TSubclassOf<APawn> UAuraSummonAbility::GetRandomMinionClass()
{
	const int32 Selection = FMath::RandRange(0 , MinionClass.Num() - 1);
	return MinionClass[Selection];
}
