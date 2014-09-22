#pragma once

#include "cinder/Exception.h"
#include <map>
#include <string>
#include <vector>

namespace cinder { namespace gl { namespace effect {

class Operation
{
public:
	
	/////////////////////////////////////////////////////////////////////////////////////////////////
	
	enum : size_t
	{
		QualifierPrecision_None,
		QualifierPrecision_Low,
		QualifierPrecision_Medium,
		QualifierPrecision_High
	} typedef QualifierPrecision;

	enum : size_t
	{
		QualifierStorage_None,
		QualifierStorage_Const,
		QualifierStorage_Attribute,
		QualifierStorage_Uniform,
		QualifierStorage_Varying
	} typedef QualifierStorage;
		
	enum : size_t
	{
		QualifierType_None,
		QualifierType_Bool,
		QualifierType_BVec2, QualifierType_BVec3, QualifierType_BVec4,
		QualifierType_Double,
		QualifierType_DVec2, QualifierType_DVec3, QualifierType_DVec4,
		QualifierType_Int,
		QualifierType_IVec2, QualifierType_IVec3, QualifierType_IVec4,
		QualifierType_DMat2, QualifierType_DMat2x2, QualifierType_DMat2x3, QualifierType_DMat2x4,
		QualifierType_DMat3, QualifierType_DMat3x2, QualifierType_DMat3x3, QualifierType_DMat3x4,
		QualifierType_DMat4, QualifierType_DMat4x2, QualifierType_DMat4x3, QualifierType_DMat4x4,
		QualifierType_Float,
		QualifierType_Mat2, QualifierType_Mat2x2, QualifierType_Mat2x3, QualifierType_Mat2x4,
		QualifierType_Mat3, QualifierType_Mat3x2, QualifierType_Mat3x3, QualifierType_Mat3x4,
		QualifierType_Mat4, QualifierType_Mat4x2, QualifierType_Mat4x3, QualifierType_Mat4x4,
		QualifierType_Sampler1d, QualifierType_Sampler2d, QualifierType_Sampler2dShadow,
		QualifierType_Sampler3d, QualifierType_SamplerCube,
		QualifierType_UINT,
		QualifierType_UVec2, QualifierType_UVec3, QualifierType_UVec4,
		QualifierType_Vec2, QualifierType_Vec3, QualifierType_Vec4
	} typedef QualifierType;

	class Qualifier
	{
	public:
		Qualifier();

		size_t							mCount;
		QualifierPrecision				mPrecision;
		QualifierStorage				mStorage;
		QualifierType					mType;
		std::string						mValue;
	};
	
	/////////////////////////////////////////////////////////////////////////////////////////////////
	
protected:
	enum : size_t
	{
		OperatorType_Add,
		OperatorType_Multiply,
		OperatorType_Subtract,
		OperatorType_Divide,
		OperatorType_Modulate
	} typedef OperatorType;
	
	struct Routine
	{
		OperatorType					mOperatorType;
		std::string						mRoutine;
	};
	
	std::map<std::string, Qualifier>	mQualifiers;
	std::vector<Routine>				mRoutines;
	
	void								mergeQualifiers( const std::map<std::string, Qualifier>& q );
public:
	Operation();
	
	Operation operator+( const Operation& rhs );
	Operation operator*( const Operation& rhs );
	Operation operator-( const Operation& rhs );
	Operation operator/( const Operation& rhs );
	Operation operator%( const Operation& rhs );
	
	void operator+=( const Operation& rhs );
	void operator*=( const Operation& rhs );
	void operator-=( const Operation& rhs );
	void operator/=( const Operation& rhs );
	void operator%=( const Operation& rhs );
	
	/////////////////////////////////////////////////////////////////////////////////////////////////
	
	class Exception : public cinder::Exception
	{
	public:
		Exception( const std::string& msg ) throw();
		virtual const char* what() const throw()
		{
			return mMessage;
		}
	private:
		char mMessage[ 2048 ];
	};
	
	class ExcQualifierMergeCountMismatch : public Exception
	{
	public:
		ExcQualifierMergeCountMismatch( const std::string& msg ) throw();
	};
	
	class ExcQualifierMergePrecisionMismatch : public Exception
	{
	public:
		ExcQualifierMergePrecisionMismatch( const std::string& msg ) throw();
	};
	
	class ExcQualifierMergeStorageMismatch : public Exception
	{
	public:
		ExcQualifierMergeStorageMismatch( const std::string& msg ) throw();
	};
	
	class ExcQualifierMergeTypeMismatch : public Exception
	{
	public:
		ExcQualifierMergeTypeMismatch( const std::string& msg ) throw();
	};
	
	class ExcQualifierMergeValueMismatch : public Exception
	{
	public:
		ExcQualifierMergeValueMismatch( const std::string& msg ) throw();
	};
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