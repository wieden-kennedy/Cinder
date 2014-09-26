#pragma once

#include "cinder/Exception.h"
#include "cinder/GeomIo.h"
#include <map>
#include <ostream>
#include <string>
#include <vector>

namespace cinder { namespace gl { namespace effect {

class Effect;
class FragmentOperation;
class VertexOperation;

class Operation
{
protected:
	
	/////////////////////////////////////////////////////////////////////////////////////////////////
	
#if defined( CINDER_GL_ES_2 )
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
		QualifierType_Float,
		QualifierType_Int,
		QualifierType_IVec2, QualifierType_IVec3, QualifierType_IVec4,
		QualifierType_Mat2,
		QualifierType_Mat3,
		QualifierType_Mat4,
		QualifierType_Sampler2d,
		QualifierType_SamplerCube,
#if !defined( CINDER_GL_ES_2 )
		QualifierType_Double,
		QualifierType_DVec2, QualifierType_DVec3, QualifierType_DVec4,
		QualifierType_DMat2, QualifierType_DMat2x2, QualifierType_DMat2x3, QualifierType_DMat2x4,
		QualifierType_DMat3, QualifierType_DMat3x2, QualifierType_DMat3x3, QualifierType_DMat3x4,
		QualifierType_DMat4, QualifierType_DMat4x2, QualifierType_DMat4x3, QualifierType_DMat4x4,
		QualifierType_Mat2x2, QualifierType_Mat2x3, QualifierType_Mat2x4,
		QualifierType_Mat3x2, QualifierType_Mat3x3, QualifierType_Mat3x4,
		QualifierType_Mat4x2, QualifierType_Mat4x3, QualifierType_Mat4x4,
		QualifierType_Sampler1d, QualifierType_Sampler2dShadow, QualifierType_Sampler3d,
		QualifierType_Uint,
		QualifierType_UVec2, QualifierType_UVec3, QualifierType_UVec4,
#endif
		QualifierType_Vec2, QualifierType_Vec3, QualifierType_Vec4
	} typedef QualifierType;

	class Qualifier
	{
	public:
		Qualifier();
		Qualifier( QualifierStorage storage, QualifierType type, const std::string& value = "", 
#if defined( CINDER_GL_ES_2 )
				   QualifierPrecision precision = QualifierPrecision_High, 
#endif
				   size_t count = 1
				   );

		size_t					mCount;
#if defined( CINDER_GL_ES_2 )
		QualifierPrecision		mPrecision;
#endif
		QualifierStorage		mStorage;
		QualifierType			mType;
		std::string				mValue;
	};
	
	typedef std::map<std::string, Operation::Qualifier> QualifierMap;

	/////////////////////////////////////////////////////////////////////////////////////////////////
	
	enum : size_t
	{
		OperatorType_Add,
		OperatorType_Divide,
		OperatorType_Multiply,
		OperatorType_Subtract,
	} typedef OperatorType;
	
	class Kernel
	{
	public:
		Kernel();

		Kernel&					bodyExpression( const std::string& exp );
		Kernel&					operatorType( OperatorType type );
		Kernel&					outputExpression( const std::string& exp );

		const std::string&		getBodyExpression() const;
		OperatorType			getOperatorType() const;
		const std::string&		getOutputExpression() const;

		void					setBodyExpression( const std::string& exp );
		void					setOperatorType( OperatorType type );
		void					setOutputExpression( const std::string& exp );
	protected:
		std::string				mExpressionBody;
		std::string				mExpressionOutput;
		OperatorType			mOperatorType;
	};
	
	/////////////////////////////////////////////////////////////////////////////////////////////////

	static std::map<std::string, geom::Attrib>	getDefaultFragmentInputNameToSemanticMap();
	static std::map<geom::Attrib, std::string>	getSemanticToDefaultFragmentInputNameMap();
	static std::map<int32_t, std::string>		getSemanticToDefaultUniformNameMap();
	static std::map<geom::Attrib, std::string>	getSemanticToDefaultVertexInputNameMap();

	static QualifierMap			mergeQualifiers( const QualifierMap& a, const QualifierMap& b );

	static std::string			kernelToString( const Operation& op );
	static std::string			outputToString( const Operation& op );
	static std::string			qualifiersToString( const QualifierMap& qualifers, bool isFragment );
	static std::string			versionToString( const Operation& op );

	void						merge( const Operation& rhs, OperatorType type );

	void						setQualifier( std::string& oldName, const std::string& newName, 
											  const Qualifier& q );

	std::vector<Kernel>			mKernels;
	QualifierMap				mQualifiers;

	friend class				Effect;
	friend class				FragmentOperation;
	friend class				VertexOperation;
public:
	Operation();
	Operation( const QualifierMap& qualifers, const std::vector<Kernel>& kernels );

	virtual Operation					operator+( const Operation& rhs );
	virtual Operation 					operator*( const Operation& rhs );
	virtual Operation 					operator-( const Operation& rhs );
	virtual Operation 					operator/( const Operation& rhs );
	
	virtual void						operator+=( const Operation& rhs );
	virtual void						operator*=( const Operation& rhs );
	virtual void						operator-=( const Operation& rhs );
	virtual void						operator/=( const Operation& rhs );

	// TODO dot, cross, etc

	virtual const std::vector<Kernel>&	getKernels() const;
	virtual const QualifierMap&			getQualifiers() const;

	virtual std::string					toString() const;

	/////////////////////////////////////////////////////////////////////////////////////////////////
	
	class Exception : public cinder::Exception
	{
	public:
		Exception( const std::string& msg ) throw();
		virtual const char*		what() const throw()
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
	
#if defined( CINDER_GL_ES_2 )
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

std::ostream& operator<<( std::ostream& out, const Operation& op );

/////////////////////////////////////////////////////////////////////////////////////////////////

class FragmentOperation : public Operation
{
public:
	FragmentOperation();
	FragmentOperation( const QualifierMap& qualifers, const std::vector<Kernel>& kernels );

	virtual FragmentOperation	operator+( const FragmentOperation& rhs );
	virtual FragmentOperation 	operator*( const FragmentOperation& rhs );
	virtual FragmentOperation 	operator-( const FragmentOperation& rhs );
	virtual FragmentOperation 	operator/( const FragmentOperation& rhs );
	
	virtual void				operator+=( const FragmentOperation& rhs );
	virtual void				operator*=( const FragmentOperation& rhs );
	virtual void				operator-=( const FragmentOperation& rhs );
	virtual void				operator/=( const FragmentOperation& rhs );

	virtual std::string			toString() const;
};

/////////////////////////////////////////////////////////////////////////////////////////////////

class VertexOperation : public Operation
{
public:
	VertexOperation();
	VertexOperation( const QualifierMap& qualifers, const std::vector<Kernel>& kernels );

	virtual VertexOperation		operator+( const VertexOperation& rhs );
	virtual VertexOperation 	operator*( const VertexOperation& rhs );
	virtual VertexOperation 	operator-( const VertexOperation& rhs );
	virtual VertexOperation 	operator/( const VertexOperation& rhs );
	
	virtual void				operator+=( const VertexOperation& rhs );
	virtual void				operator*=( const VertexOperation& rhs );
	virtual void				operator-=( const VertexOperation& rhs );
	virtual void				operator/=( const VertexOperation& rhs );

	virtual std::string			toString() const;
protected:
	static std::string			vertexOutputAssignmentToString( const QualifierMap& qualifers );
	friend class				Effect;
};

/////////////////////////////////////////////////////////////////////////////////////////////////

class Effect
{
public:
	Effect( const VertexOperation& vert = VertexOperation(), 
			const FragmentOperation& frag = FragmentOperation() );

	virtual FragmentOperation&			getFragmentOperation();
	virtual const FragmentOperation&	getFragmentOperation() const;
	virtual VertexOperation&			getVertexOperation();
	virtual const VertexOperation&		getVertexOperation() const;
protected:
	virtual  void						createVertexOutputs();

	FragmentOperation					mFragmentOperation;
	VertexOperation						mVertexOperation;
};

/////////////////////////////////////////////////////////////////////////////////////////////////

class VertexPassThrough : public VertexOperation
{
public:
	VertexPassThrough();
};

/////////////////////////////////////////////////////////////////////////////////////////////////

class FragmentColor : public FragmentOperation
{
public:
	FragmentColor();
};

class FragmentTexture2d : public FragmentOperation
{
public:
	FragmentTexture2d();

	FragmentTexture2d&	texture( const std::string& uniformName );
protected:
	std::string			mNameTexture;
};

class FragmentExposure : public FragmentOperation
{
public:
	FragmentExposure( FragmentOperation* op = nullptr );

	FragmentExposure&	exposure( const std::string& uniformName );
	FragmentExposure&	exposure( float v );
	FragmentExposure&	input( FragmentOperation* op );
	FragmentExposure&	offset( const std::string& uniformName );
	FragmentExposure&	offset( float v );

	std::string			toString() const;
protected:
	FragmentOperation*	mInput;
	std::string			mNameExposure;
	std::string			mNameOffset;
};

/////////////////////////////////////////////////////////////////////////////////////////////////

class Exposure : public Effect
{
public:
	Exposure( float exposure = 1.0f, float offset = 0.0f );

	virtual FragmentOperation&			getFragmentOperation();
	virtual const FragmentOperation&	getFragmentOperation() const;

	Exposure&			exposure( const std::string& uniformName );
	Exposure&			exposure( float v );
	Exposure&			input( const FragmentOperation& op );
	Exposure&			offset( const std::string& uniformName );
	Exposure&			offset( float v );
protected:
	FragmentExposure	mFragmentExposure;
	FragmentOperation	mFragmentInput;
};

} } }
 