#pragma once

#include <map>
#include <string>

namespace cinder { namespace gl { namespace effect {

class Operation
{
protected:
	enum : size_t
	{
		NONE, LOW, MEDIUM, HIGH
	} typedef QualifierPrecision;

	enum : size_t
	{
		NONE, CONST, ATTRIBUTE, UNIFORM, VARYING
	} typedef QualifierStorage;
		
	enum : size_t // Will probably just make this a string instead of enum
	{
		NONE, 
		BOOL, 
		BVEC2, BVEC3, BVEC4, 
		DOUBLE, 
		DVEC2, DVEC3, DVEC4, 
		INT, 
		IVEC2, IVEC3, IVEC4, 
		DMAT2, DMAT2X2, DMAT2X3, DMAT2X4, 
		DMAT3, DMAT3X2, DMAT3X3, DMAT3X4, 
		DMAT4, DMAT4X2, DMAT4X3, DMAT4X4, 
		FLOAT, 
		MAT2, MAT2X2, MAT2X3, MAT2X4, 
		MAT3, MAT3X2, MAT3X3, MAT3X4, 
		MAT4, MAT4X2, MAT4X3, MAT4X4, 
		SAMPLER1D, SAMPLER2D, SAMPLER2DSHADOW, SAMPLER3D, SAMPLERCUBE, 
		UINT, 
		UVEC2, UVEC3, UVEC4, 
		VEC2, VEC3, VEC4
	} typedef QualifierType;

	class Qualifier
	{
	public:
		Qualifier();

		size_t							mCount;
		QualifierPrecision				mPrecision;
		QualifierStorage				mStorage;
		QualifierType					mType;
	};

	std::map<std::string, Qualifier>	mQualifiers;
};

} } }
 

/*
Leaving his here for later use

bool
bvec2
bvec3
bvec4

double
dvec2
dvec3
dvec4

int
ivec2
ivec3
ivec4

uvec2
uvec3
uvec4

vec2
vec3
vec4

dmat2
dmat2x2
dmat2x3
dmat2x4
dmat3
dmat3x2
dmat3x3
dmat3x4
dmat4
dmat4x2
dmat4x3
dmat4x4

float

mat2
mat2x2
mat2x3
mat2x4
mat3
mat3x2
mat3x3
mat3x4
mat4
mat4x2
mat4x3
mat4x4

uint

sampler1D
sampler2D
sampler2DShadow
sampler3D
samplerCube
*/