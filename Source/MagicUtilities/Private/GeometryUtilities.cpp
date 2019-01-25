// Fill out your copyright notice in the Description page of Project Settings.

#include "GeometryUtilities.h"

const FVector4 UGeometryUtilities::MakePlaneFromThreePoints(FVector PointA, FVector PointB, FVector PointC)
{
	FVector4 PlaneCoefficients;

	// we use each value a couple of times, let's compute them just once
	float ByAy = PointB.Y - PointA.Y;
	float CzAz = PointC.Z - PointA.Z;
	float CyAy = PointC.Y - PointA.Y;
	float BzAz = PointB.Z - PointA.Z;
	float CxAx = PointC.X - PointA.X;
	float BxAx = PointB.X - PointA.X;

	// computing a, b, c and d coefficients of this plane (ax + by +cz +d = 0), and storing them in x, y, z, w fields of a vec4
	
	PlaneCoefficients.X = ByAy * CzAz - CyAy * BzAz;
	PlaneCoefficients.Y = BzAz * CxAx - CzAz * BxAx;
	PlaneCoefficients.Z = BxAx * CyAy - CxAx * ByAy;
	PlaneCoefficients.W = -(PlaneCoefficients.X*PointA.X + PlaneCoefficients.Y*PointA.Y + PlaneCoefficients.Z*PointA.Z);
	
	return PlaneCoefficients;
}