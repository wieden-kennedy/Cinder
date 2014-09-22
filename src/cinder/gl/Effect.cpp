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
#if defined ( CINDER_GL_ES_2 )
		string precision	= "";
#endif
		string type			= "";
		switch ( q.mType ) {
			case QualifierType_Bool:
				
				break;
				
				
				
				/*
				 Leaving this here for later use
				 
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
		}
		
		if ( q.mCount > 1 ) {
			line			+= "[" + cinder::toString( q.mCount ) + "]";
		}
		if ( !q.mValue.empty() ) {
		}
	}
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
	Qualifier a4;
	a4.mStorage	= QualifierStorage_Attribute;
	a4.mType	= QualifierType_Vec4;
	
	Qualifier v4;
	v4.mStorage	= QualifierStorage_Varying;
	v4.mType	= QualifierType_Vec4;
	
	Qualifier u4x4;
	u4x4.mStorage	= QualifierStorage_Uniform;
	u4x4.mType		= QualifierType_Mat4;
	
	mQualifiers[ "ciPosition" ]				= a4;
	mQualifiers[ "ciTexCoord0" ]			= a4;
	mQualifiers[ "vTexCoord0" ]				= v4;
	mQualifiers[ "ciModelViewProjection" ]	= u4x4;
	
	mRoutines.front().mRoutine	= "vTexCoord0 = ciTexCoord0;";
	mRoutines.front().mResult	= "ciModelViewProjection * ciPosition;";
}

/////////////////////////////////////////////////////////////////////////////////////////////////

FragmentTexture::FragmentTexture()
: FragmentOperation()
{
	Qualifier v4;
	v4.mStorage	= QualifierStorage_Attribute;
	v4.mType	= QualifierType_Vec4;
	
	Qualifier uSampler2d;
	uSampler2d.mStorage	= QualifierStorage_Uniform;
	uSampler2d.mType	= QualifierType_Sampler2d;
	
	mQualifiers[ "vTexCoord0" ] = v4;
	mQualifiers[ "uTexture" ]	= uSampler2d;
	
	mRoutines.front().mRoutine	= "vec4 color = texture( uTexture, vTexCoord0.st );";
	mRoutines.front().mResult	= "color;";
}
	
/////////////////////////////////////////////////////////////////////////////////////////////////


} } }
 