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
	
#if defined ( CINDER_GL_ES_2 )
	enum : size_t
	{
		QualifierPrecision_None,
		QualifierPrecision_Low,
		QualifierPrecision_Medium,
		QualifierPrecision_High
	} typedef QualifierPrecision;
#endif

	enum : size_t
	{
		QualifierStorage_None,
		QualifierStorage_Const,
		QualifierStorage_Input,
		QualifierStorage_Output,
		QualifierStorage_Uniform
	} typedef QualifierStorage;
		
	enum : size_t
	{
		QualifierType_None,
		QualifierType_Bool,
		QualifierType_BVec2, QualifierType_BVec3, QualifierType_BVec4,
		QualifierType_Double,
		QualifierType_DVec2, QualifierType_DVec3, QualifierType_DVec4,
		QualifierType_DMat2, QualifierType_DMat2x2, QualifierType_DMat2x3, QualifierType_DMat2x4,
		QualifierType_DMat3, QualifierType_DMat3x2, QualifierType_DMat3x3, QualifierType_DMat3x4,
		QualifierType_DMat4, QualifierType_DMat4x2, QualifierType_DMat4x3, QualifierType_DMat4x4,
		QualifierType_Float,
		QualifierType_Int,
		QualifierType_IVec2, QualifierType_IVec3, QualifierType_IVec4,
		QualifierType_Mat2, QualifierType_Mat2x2, QualifierType_Mat2x3, QualifierType_Mat2x4,
		QualifierType_Mat3, QualifierType_Mat3x2, QualifierType_Mat3x3, QualifierType_Mat3x4,
		QualifierType_Mat4, QualifierType_Mat4x2, QualifierType_Mat4x3, QualifierType_Mat4x4,
		QualifierType_Sampler1d, QualifierType_Sampler2d, QualifierType_Sampler2dShadow,
		QualifierType_Sampler3d, QualifierType_SamplerCube,
		QualifierType_Uint,
		QualifierType_UVec2, QualifierType_UVec3, QualifierType_UVec4,
		QualifierType_Vec2, QualifierType_Vec3, QualifierType_Vec4
	} typedef QualifierType;

	class Qualifier
	{
	public:
		Qualifier();

		size_t							mCount;
#if defined ( CINDER_GL_ES_2 )
		QualifierPrecision				mPrecision;
#endif
		QualifierStorage				mStorage;
		QualifierType					mType;
		std::string						mValue;
	};
	
	/////////////////////////////////////////////////////////////////////////////////////////////////
	
//protected:
	enum : size_t
	{
		OperatorType_Add,
		OperatorType_Divide,
		OperatorType_Multiply,
		OperatorType_Subtract,
	} typedef OperatorType;
	
	class Routine
	{
	public:
		Routine();
		
		OperatorType					mOperatorType;
		std::string						mResult;
		std::string						mRoutine;
	};
	
	std::map<std::string, Qualifier>	mQualifiers;
	std::vector<Routine>				mRoutines;
	
	void								merge( const Operation& rhs, OperatorType type );
	void								mergeQualifiers( const std::map<std::string, Qualifier>& q );
	std::string							qualifiersToString() const;
public:
	Operation();
	
	Operation							operator+( const Operation& rhs );
	Operation 							operator*( const Operation& rhs );
	Operation 							operator-( const Operation& rhs );
	Operation 							operator/( const Operation& rhs );
	
	void								operator+=( const Operation& rhs );
	void								operator*=( const Operation& rhs );
	void								operator-=( const Operation& rhs );
	void								operator/=( const Operation& rhs );

	// TODO dot, cross, etc
	
	virtual std::string					toString() const;
	
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
	
#if defined ( CINDER_GL_ES_2 )
	class ExcQualifierMergePrecisionMismatch : public Exception
	{
	public:
		ExcQualifierMergePrecisionMismatch( const std::string& msg ) throw();
	};
#endif
	
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

/////////////////////////////////////////////////////////////////////////////////////////////////

class FragmentOperation : public Operation
{
public:
	FragmentOperation();

	virtual std::string	toString() const;
};

class VertexOperation : public Operation
{
public:
	VertexOperation();

	virtual std::string	toString() const;
};

/////////////////////////////////////////////////////////////////////////////////////////////////

class VertexPassThrough : public VertexOperation
{
public:
	VertexPassThrough();
};

/////////////////////////////////////////////////////////////////////////////////////////////////

class FragmentTexture : public FragmentOperation
{
public:
	FragmentTexture();
};

class FragmentExposure : public FragmentOperation
{
public:
	FragmentExposure( FragmentOperation* op = nullptr );

	FragmentExposure&	exposure( const std::string& uniformName );
	FragmentExposure&	input( FragmentOperation* op );
	FragmentExposure&	offset( const std::string& uniformName );

	const std::string&	getExposureUniform() const;
	FragmentOperation*	getInput() const;
	const std::string&	getOffsetUniform() const;

	void				setExposureUniform( const std::string& uniformName );
	FragmentExposure&	setInput( FragmentOperation* op );
	void				setOffsetUniform( const std::string& uniformName );

	std::string			toString() const;
protected:
	void				setUniform( const std::string& uniformName );

	FragmentOperation*	mInput;
	std::string			mUniformExposure;
	std::string			mUniformOffset;
};
	
} } }
 