// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GeometryUtilities.generated.h"

/**
 * 
 */
UCLASS()
class MAGICUTILITIES_API UGeometryUtilities : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()	
	
	/**
	* Computes the coefficients describing a plane which passes from the provided points 
	* @param PointA - first point resting on the plane to be generated
	* @param PointB - second point resting on the plane to be generated
	* @param PointC - third point resting on the plane to be generated
	* @return PlaneCoefficients - the a, b, c, d coefficients of the plane, stored within the fields of a FVector4
	*/
	UFUNCTION(BlueprintPure, Category = "Magic Utilities - Geometry")
	static const FVector4 MakePlaneFromThreePoints(FVector PointA, FVector PointB, FVector PointC);
	
};
