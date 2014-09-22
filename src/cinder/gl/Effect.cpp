#include "cinder/gl/Effect.h"

#include "cinder/Utilities.h"

namespace cinder { namespace gl { namespace effect {

using namespace std;
	
Operation::Qualifier::Qualifier()
: mCount( 1 ),
#if defined ( CINDER_GL_ES_2 )
mPrecision( QualifierPrecision_High ),
#endif
mStorage( QualifierStorage_None ), mType( QualifierType_None )
{
}
	
/////////////////////////////////////////////////////////////////////////////////////////////////

	
Operation::Routine::Routine()
: mOperatorType( OperatorType_Add ), mRoutine( "" )
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////

Operation::Operation()
{
	mRoutines.push_back( Routine() );
}

void Operation::merge( const Operation& rhs, OperatorType type )
{
	if ( !rhs.mRoutines.empty() ) {
		mergeQualifiers( rhs.mQualifiers );
		for ( vector<Operation::Routine>::const_iterator iter = rhs.mRoutines.begin(); iter != rhs.mRoutines.end(); ++iter ) {
			mRoutines.push_back( *iter );
			if ( iter == rhs.mRoutines.begin() ) {
				mRoutines.back().mOperatorType = type;
			}
		}
	}
}
	
Operation Operation::operator+( const Operation& rhs )
{
	Operation lhs = *this;
	lhs += rhs;
	return lhs;
}
	
Operation Operation::operator*( const Operation& rhs )
{
	Operation lhs = *this;
	lhs *= rhs;
	return lhs;
}
	
Operation Operation::operator-( const Operation& rhs )
{
	Operation lhs = *this;
	lhs -= rhs;
	return lhs;
}
	
Operation Operation::operator/( const Operation& rhs )
{
	Operation lhs = *this;
	lhs /= rhs;
	return lhs;
}

Operation Operation::operator%( const Operation& rhs )
{
	Operation lhs = *this;
	lhs %= rhs;
	return lhs;
}

void Operation::operator+=( const Operation& rhs )
{
	merge( rhs, OperatorType_Add );
}
	
void Operation::operator*=( const Operation& rhs )
{
	merge( rhs, OperatorType_Multiply );
}
	
void Operation::operator-=( const Operation& rhs )
{
	merge( rhs, OperatorType_Subtract );
}
	
void Operation::operator/=( const Operation& rhs )
{
	merge( rhs, OperatorType_Divide );
}

void Operation::operator%=( const Operation& rhs )
{
	merge( rhs, OperatorType_Modulate );
}
	
void Operation::mergeQualifiers( const map<string, Operation::Qualifier>& q )
{
	for ( map<string, Operation::Qualifier>::const_iterator iter = q.begin(); iter != q.end(); ++iter ) {
		const string& name = iter->first;
		if ( mQualifiers.find( name ) != mQualifiers.end() ) {
			const Qualifier& a = mQualifiers.find( name )->second;
			const Qualifier& b = iter->second;
			if ( a.mCount != b.mCount ) {
				throw Operation::ExcQualifierMergeCountMismatch( "Unable to merge qualifiers. Count mismatch for \"" + name + "\"." );
#if defined ( CINDER_GL_ES_2 )
			} else if ( a.mPrecision != b.mPrecision ) {
				throw Operation::ExcQualifierMergePrecisionMismatch( "Unable to merge qualifiers. Precision mismatch for \"" + name + "\"." );
#endif
			} else if ( a.mStorage != b.mStorage ) {
				throw Operation::ExcQualifierMergeStorageMismatch( "Unable to merge qualifiers. Storage mismatch for \"" + name + "\"." );
			} else if ( a.mType != b.mType ) {
				throw Operation::ExcQualifierMergeTypeMismatch( "Unable to merge qualifiers. Type mismatch for \"" + name + "\"." );
			} else if ( a.mValue != b.mValue ) {
				throw Operation::ExcQualifierMergeValueMismatch( "Unable to merge qualifiers. Value mismatch for \"" + name + "\"." );
			}
		} else {
			mQualifiers[ name ] = iter->second;
		}
	}
}

string Operation::toString() const
{
	string output = "";
	for ( map<string, Operation::Qualifier>::const_iterator iter = mQualifiers.begin(); iter != mQualifiers.end(); ++iter ) {
		const Qualifier& q	= iter->second;
		string name			= iter->first;
		string line			= "";
		
		string storage		= "";
		switch ( q.mStorage ) {
			case QualifierStorage_Const:
				storage = "const";
				break;
			case QualifierStorage_Input:
#if defined ( CINDER_GL_ES_2 )
				storage = "attribute";
#else
				storage = "in";
#endif
				break;
			case QualifierStorage_None:
				break;
			case QualifierStorage_Output:
#if defined ( CINDER_GL_ES_2 )
				storage = "varying";
#else
				storage = "out";
#endif
				break;
			case QualifierStorage_Uniform:
				storage = "uniform";
				break;
		}
		
#if defined ( CINDER_GL_ES_2 )
		string precision	= "";
		switch ( q.mPrecision ) {
			case QualifierPrecision_High:
				precision = "highp";
				break;
			case QualifierPrecision_Low:
				precision = "lowp";
				break;
			case QualifierPrecision_Medium:
				precision = "medp";
				break;
			case QualifierPrecision_None:
				break;
		}
#endif
		
		// TODO consider ES2
		string type = "";
		switch ( q.mType ) {
			case QualifierType_Bool:
				type = "";
				break;
			case QualifierType_BVec2:
				type = "";
				break;
			case QualifierType_BVec3:
				type = "";
				break;
			case QualifierType_BVec4:
				type = "";
				break;
			case QualifierType_Double:
				type = "";
				break;
			case QualifierType_DVec2:
				type = "";
				break;
			case QualifierType_DVec3:
				type = "";
				break;
			case QualifierType_DVec4:
				type = "";
				break;
			case QualifierType_Int:
				type = "";
				break;
			case QualifierType_IVec2:
				type = "";
				break;
			case QualifierType_IVec3:
				type = "";
				break;
			case QualifierType_IVec4:
				type = "";
				break;
			case QualifierType_DMat2:
				type = "";
				break;
			case QualifierType_DMat2x2:
				type = "";
				break;
			case QualifierType_DMat2x3:
				type = "";
				break;
			case QualifierType_DMat2x4:
				type = "";
				break;
			case QualifierType_DMat3:
				type = "";
				break;
			case QualifierType_DMat3x2:
				type = "";
				break;
			case QualifierType_DMat3x3:
				type = "";
				break;
			case QualifierType_DMat3x4:
				type = "";
				break;
			case QualifierType_DMat4:
				type = "";
				break;
			case QualifierType_DMat4x2:
				type = "";
				break;
			case QualifierType_DMat4x3:
				type = "";
				break;
			case QualifierType_DMat4x4:
				type = "";
				break;
			case QualifierType_Float:
				type = "";
				break;
			case QualifierType_Mat2:
				type = "";
				break;
			case QualifierType_Mat2x2:
				type = "";
				break;
			case QualifierType_Mat2x3:
				type = "";
				break;
			case QualifierType_Mat2x4:
				type = "";
				break;
			case QualifierType_Mat3:
				type = "";
				break;
			case QualifierType_Mat3x2:
				type = "";
				break;
			case QualifierType_Mat3x3:
				type = "";
				break;
			case QualifierType_Mat3x4:
				type = "";
				break;
			case QualifierType_Mat4:
				type = "";
				break;
			case QualifierType_Mat4x2:
				type = "";
				break;
			case QualifierType_Mat4x3:
				type = "";
				break;
			case QualifierType_Mat4x4:
				type = "";
				break;
			case QualifierType_None:
				break;
			case QualifierType_Sampler1d:
				type = "";
				break;
			case QualifierType_Sampler2d:
				type = "";
				break;
			case QualifierType_Sampler2dShadow:
				type = "";
				break;
			case QualifierType_Sampler3d:
				type = "";
				break;
			case QualifierType_SamplerCube:
				type = "";
				break;
			case QualifierType_Uint:
				type = "";
				break;
			case QualifierType_UVec2:
				type = "";
				break;
			case QualifierType_UVec3:
				type = "";
				break;
			case QualifierType_UVec4:
				type = "";
				break;
			case QualifierType_Vec2:
				type = "";
				break;
			case QualifierType_Vec3:
				type = "";
				break;
			case QualifierType_Vec4:
				type = "";
				break;
				
				
				/*
				 TODO put these string up there ^
				 
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
				 
				 uvec2
				 uvec3
				 uvec4
				 
				 vec2
				 vec3
				 vec4
				 */
		}
		
		if ( !storage.empty() ) {
			line = storage + " ";
		}
#if defined ( CINDER_GL_ES_2 )
		if ( !precision.empty() ) {
			line += precision + " ";
		}
#endif
		if ( !type.empty() ) {
			line += type + " ";
		}
		line += name;
		
		if ( q.mCount > 1 ) {
			line += "[" + cinder::toString( q.mCount ) + "]";
		}
		if ( !q.mValue.empty() ) {
			line += " = " + q.mValue;
		}
		
		line	+= ";\n\r";
		output	+= line;
	}
	
	output += "\n\rvoid main( void ) {\n\r";
	// TODO add body
	output += "}\r";
	
	return output;
}
	
/////////////////////////////////////////////////////////////////////////////////////////////////

Operation::Exception::Exception( const string& msg ) throw()
{
	sprintf( mMessage, "%s", msg.c_str() );
}

Operation::ExcQualifierMergeCountMismatch::ExcQualifierMergeCountMismatch( const string& msg ) throw()
: Operation::Exception( msg )
{
}
	
#if defined ( CINDER_GL_ES_2 )
Operation::ExcQualifierMergePrecisionMismatch::ExcQualifierMergePrecisionMismatch( const string& msg ) throw()
: Operation::Exception( msg )
{
}
#endif
	
Operation::ExcQualifierMergeStorageMismatch::ExcQualifierMergeStorageMismatch( const string& msg ) throw()
: Operation::Exception( msg )
{
}

Operation::ExcQualifierMergeTypeMismatch::ExcQualifierMergeTypeMismatch( const string& msg ) throw()
: Operation::Exception( msg )
{
}

Operation::ExcQualifierMergeValueMismatch::ExcQualifierMergeValueMismatch( const string& msg ) throw()
: Operation::Exception( msg )
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////

FragmentOperation::FragmentOperation()
: Operation()
{
}

VertexOperation::VertexOperation()
: Operation()
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////
	
VertexPassThrough::VertexPassThrough()
: VertexOperation()
{
	Qualifier i4;
	i4.mStorage	= QualifierStorage_Input;
	i4.mType	= QualifierType_Vec4;
	
	Qualifier o4;
	o4.mStorage	= QualifierStorage_Output;
	o4.mType	= QualifierType_Vec4;
	
	Qualifier u4x4;
	u4x4.mStorage	= QualifierStorage_Uniform;
	u4x4.mType		= QualifierType_Mat4;
	
	mQualifiers[ "ciPosition" ]				= i4;
	mQualifiers[ "ciTexCoord0" ]			= i4;
	mQualifiers[ "vTexCoord0" ]				= o4;
	mQualifiers[ "ciModelViewProjection" ]	= u4x4;
	
	mRoutines.front().mRoutine	= "vTexCoord0 = ciTexCoord0;";
	mRoutines.front().mResult	= "ciModelViewProjection * ciPosition;";
}

/////////////////////////////////////////////////////////////////////////////////////////////////

FragmentTexture::FragmentTexture()
: FragmentOperation()
{
	Qualifier i4;
	i4.mStorage	= QualifierStorage_Input;
	i4.mType	= QualifierType_Vec4;
	
	Qualifier uSampler2d;
	uSampler2d.mStorage	= QualifierStorage_Uniform;
	uSampler2d.mType	= QualifierType_Sampler2d;
	
	mQualifiers[ "vTexCoord0" ] = i4;
	mQualifiers[ "uTexture" ]	= uSampler2d;
	
	mRoutines.front().mRoutine	= "vec4 color = texture( uTexture, vTexCoord0.st );";
	mRoutines.front().mResult	= "color;";
}
	
/////////////////////////////////////////////////////////////////////////////////////////////////


} } }
 