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

string Operation::qualifiersToString() const
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
				// TODO make this "varying" for fragment shaders
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
		
		string type = "";
		switch ( q.mType ) {
			case QualifierType_Bool:
				type = "bool";
				break;
			case QualifierType_BVec2:
				type = "bvec2";
				break;
			case QualifierType_BVec3:
				type = "bvec3";
				break;
			case QualifierType_BVec4:
				type = "bvec4";
				break;
#if !defined ( CINDER_GL_ES_2 )
			case QualifierType_Double:
				type = "double";
				break;
			case QualifierType_DVec2:
				type = "dvec2";
				break;
			case QualifierType_DVec3:
				type = "dvec3";
				break;
			case QualifierType_DVec4:
				type = "dvec4";
				break;
			case QualifierType_DMat2:
				type = "dmat2";
				break;
			case QualifierType_DMat2x2:
				type = "dmat2x2";
				break;
			case QualifierType_DMat2x3:
				type = "dmat2x3";
				break;
			case QualifierType_DMat2x4:
				type = "dmat2x4";
				break;
			case QualifierType_DMat3:
				type = "dmat";
				break;
			case QualifierType_DMat3x2:
				type = "dmat3x2";
				break;
			case QualifierType_DMat3x3:
				type = "dmat3x3";
				break;
			case QualifierType_DMat3x4:
				type = "dmat3x4";
				break;
			case QualifierType_DMat4:
				type = "dmat4";
				break;
			case QualifierType_DMat4x2:
				type = "dmat4x2";
				break;
			case QualifierType_DMat4x3:
				type = "dmat4x3";
				break;
			case QualifierType_DMat4x4:
				type = "dmat4x4";
				break;
#endif
			case QualifierType_Float:
				type = "float";
				break;
			case QualifierType_Int:
				type = "int";
				break;
			case QualifierType_IVec2:
				type = "ive2c";
				break;
			case QualifierType_IVec3:
				type = "ivec3";
				break;
			case QualifierType_IVec4:
				type = "ivec4";
				break;
			case QualifierType_Mat2:
				type = "mat2";
				break;
#if !defined ( CINDER_GL_ES_2 )
			case QualifierType_Mat2x2:
				type = "mat2x2";
				break;
			case QualifierType_Mat2x3:
				type = "mat2x3";
				break;
			case QualifierType_Mat2x4:
				type = "mat2x4";
				break;
#endif
			case QualifierType_Mat3:
				type = "mat3";
				break;
#if !defined ( CINDER_GL_ES_2 )
			case QualifierType_Mat3x2:
				type = "mat3x2";
				break;
			case QualifierType_Mat3x3:
				type = "mat3x3";
				break;
			case QualifierType_Mat3x4:
				type = "mat3x4";
				break;
#endif
			case QualifierType_Mat4:
				type = "mat4";
				break;
#if !defined ( CINDER_GL_ES_2 )
			case QualifierType_Mat4x2:
				type = "mat4x2";
				break;
			case QualifierType_Mat4x3:
				type = "mat4x3";
				break;
			case QualifierType_Mat4x4:
				type = "mat4x4";
				break;
#endif
			case QualifierType_None:
				break;
#if !defined ( CINDER_GL_ES_2 )
			case QualifierType_Sampler1d:
				type = "sampler1D";
				break;
#endif
			case QualifierType_Sampler2d:
				type = "sampler2D";
				break;
#if !defined ( CINDER_GL_ES_2 )
			case QualifierType_Sampler2dShadow:
				type = "sampler2DShadow";
				break;
			case QualifierType_Sampler3d:
				type = "sampler3D";
				break;
#endif
			case QualifierType_SamplerCube:
				type = "samplerCube";
				break;
#if !defined ( CINDER_GL_ES_2 )
			case QualifierType_Uint:
				type = "uint";
				break;
			case QualifierType_UVec2:
				type = "uvec2";
				break;
			case QualifierType_UVec3:
				type = "uvec3";
				break;
			case QualifierType_UVec4:
				type = "uvec4";
				break;
#endif
			case QualifierType_Vec2:
				type = "vec2";
				break;
			case QualifierType_Vec3:
				type = "vec3";
				break;
			case QualifierType_Vec4:
				type = "vec4";
				break;
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
		
		line	+= ";\r\n";
		output	+= line;
	}
	
	return output;
}

string Operation::toString() const
{
	string output	= qualifiersToString();
	output			+= "\r\nvoid main( void ) {\r\n";

	output			+= "}\r";
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

string FragmentOperation::toString() const
{
	string output = qualifiersToString();
#if !defined( CINDER_GL_ES_2 )
	output += "out vec4 gl_FragColor;\r\n";
#endif
	output += "\r\nvoid main( void ) {\r\n";
	
	string resultName = "ciResult";
	size_t i = 0;
	for ( vector<Routine>::const_iterator iter = mRoutines.begin(); iter != mRoutines.end(); ++iter ) {
		string index	= cinder::toString( i );
		string block	= "\t";
#if defined( CINDER_GL_ES_2 )
		block			+= "highp ";
#endif
		block			+= "vec4 " + resultName + index + ";\r\n";
		block			+= "\t{\r\n";
		block			+= "\t\t" + iter->mRoutine;
		block			+= "\t}\r\n";
		block			+= "\t" + resultName + index + " = " + iter->mResult + ";\r\n";
	}

	output += "\tgl_FragColor = ";
#if defined( CINDER_GL_ES_2 )
	output += "highp ";
#endif
	output += "vec4( 0.0, 0.0, 0.0, 0.0 )";
	for ( vector<Routine>::const_iterator iter = mRoutines.begin(); iter != mRoutines.end(); ++iter ) {
		output += " ";
		switch ( iter->mOperatorType ) {
		case OperatorType_Add:
			output += "+";
			break;
		case OperatorType_Divide:
			output += "/";
			break;
		case OperatorType_Multiply:
			output += "*";
			break;
		case OperatorType_Subtract:
			output += "-";
			break;
		}
		string index	= cinder::toString( i );
		output			+= " " + resultName + index;
	}

	output += ";\r\n";
	output += "}\r";
	return output;
}

VertexOperation::VertexOperation()
: Operation()
{
}

string VertexOperation::toString() const
{
	string output	= qualifiersToString();
	output			+= "\r\nvoid main( void ) {\r\n";
	
	string resultName = "ciResult";
	size_t i = 0;
	for ( vector<Routine>::const_iterator iter = mRoutines.begin(); iter != mRoutines.end(); ++iter ) {
		string index	= cinder::toString( i );
		string block	= "\t";
#if defined( CINDER_GL_ES_2 )
		block			+= "highp ";
#endif
		block			+= "vec4 " + resultName + index + ";\r\n";
		block			+= "\t{\r\n";
		block			+= "\t\t" + iter->mRoutine;
		block			+= "\t}\r\n";
		block			+= "\t" + resultName + index + " = " + iter->mResult + ";\r\n";
	}

	output += "\tgl_Position = ";
#if defined( CINDER_GL_ES_2 )
	output += "highp ";
#endif
	output += "vec4( 0.0, 0.0, 0.0, 0.0 )";
	for ( vector<Routine>::const_iterator iter = mRoutines.begin(); iter != mRoutines.end(); ++iter ) {
		output += " ";
		switch ( iter->mOperatorType ) {
		case OperatorType_Add:
			output += "+";
			break;
		case OperatorType_Divide:
			output += "/";
			break;
		case OperatorType_Multiply:
			output += "*";
			break;
		case OperatorType_Subtract:
			output += "-";
			break;
		}
		string index	= cinder::toString( i );
		output			+= " " + resultName + index;
	}

	output += ";\r\n";
	output += "}\r";
	return output;
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

FragmentExposure::FragmentExposure( FragmentOperation* op )
: FragmentOperation(), mInput( op )
{
	setExposureUniform( "uExposure" );
	setOffsetUniform( "uOffset" );
}

FragmentExposure& FragmentExposure::exposure( const string& uniformName )
{
	setExposureUniform( uniformName );
	return *this;
}

FragmentExposure& FragmentExposure::input( FragmentOperation* op )
{
	setInput( op );
	return *this;
}

FragmentExposure& FragmentExposure::offset( const string& uniformName )
{
	setOffsetUniform( uniformName );
	return *this;
}

const string& FragmentExposure::getExposureUniform() const
{
	return mUniformExposure;
}

FragmentOperation* FragmentExposure::getInput() const
{
	return mInput;
}

const string& FragmentExposure::getOffsetUniform() const{
	return mUniformExposure;
}

void FragmentExposure::setExposureUniform( const string& uniformName )
{
	if ( mQualifiers.find( mUniformExposure ) != mQualifiers.end() ) {
		mQualifiers.erase( mUniformExposure );
	}
	mUniformExposure = uniformName;
	setUniform( uniformName );
}

FragmentExposure& FragmentExposure::setInput( FragmentOperation* op )
{
	mInput = op;
}

void FragmentExposure::setOffsetUniform( const string& uniformName )
{
	if ( mQualifiers.find( mUniformOffset ) != mQualifiers.end() ) {
		mQualifiers.erase( mUniformOffset );
	}
	mUniformOffset = uniformName;
	setUniform( uniformName );
}

void FragmentExposure::setUniform( const std::string& uniformName )
{
	Qualifier uf;
	uf.mStorage	= QualifierStorage_Uniform;
	uf.mType	= QualifierType_Float;

	mQualifiers[ uniformName ] = uf;
}

string FragmentExposure::toString() const
{
	FragmentOperation op = *this;
	map<string, Operation::Qualifier> q = mInput->mQualifiers;
	op.mergeQualifiers( q );

	// TODO 
	// write input operation's result to local var
	// use result as input for exposure
	
	string output = op.qualifiersToString();

	return "";
}

/////////////////////////////////////////////////////////////////////////////////////////////////


} } }
 