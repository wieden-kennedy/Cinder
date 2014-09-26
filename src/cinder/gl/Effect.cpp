#include "cinder/gl/Effect.h"

#include "cinder/gl/GlslProg.h"
#include "cinder/Utilities.h"

#define CI_GLSL_OUTPUT_NAME			"ciOutput"
#define CI_GLSL_OUTPUT_NAME_FRAG	"gl_FragColor"
#define CI_GLSL_OUTPUT_NAME_VERT	"gl_Position"
#define CI_GLSL_MAIN_CLOSE			"}\r\n"
#define CI_GLSL_MAIN_OPEN			"void main( void )\r\n{\r\n"

namespace cinder { namespace gl { namespace effect {

using namespace std;
	
Operation::Qualifier::Qualifier()
: mCount( 1 ), 
#if defined( CINDER_GL_ES_2 )
mPrecision( QualifierPrecision_High ),
#endif
mStorage( QualifierStorage_None ), mType( QualifierType_None )
{
}

Operation::Qualifier::Qualifier( QualifierStorage storage, QualifierType type, 
								 const string& value, 
#if defined( CINDER_GL_ES_2 )
								 QualifierPrecision precision, 
#endif
								 size_t count )
: mCount( count ), 
#if defined( CINDER_GL_ES_2 )
mPrecision( precision ), 
#endif
mStorage( storage ), mType( type ), mValue( value )
{
}
	
/////////////////////////////////////////////////////////////////////////////////////////////////

Operation::Kernel::Kernel()
: mExpressionBody( "" ), mExpressionOutput( "" ), mOperatorType( OperatorType_Add )
{
}

Operation::Kernel& Operation::Kernel::bodyExpression( const string& exp )
{
	setBodyExpression( exp );
	return *this;
}

Operation::Kernel& Operation::Kernel::operatorType( Operation::OperatorType type )
{
	setOperatorType( type );
	return *this;
}

Operation::Kernel& Operation::Kernel::outputExpression( const string& exp )
{
	setOutputExpression( exp );
	return *this;
}


const string& Operation::Kernel::getBodyExpression() const
{
	return mExpressionBody;
}

Operation::OperatorType Operation::Kernel::getOperatorType() const
{
	return mOperatorType;
}

const string& Operation::Kernel::getOutputExpression() const
{
	return mExpressionOutput;
}

void Operation::Kernel::setBodyExpression( const string& exp )
{
	mExpressionBody = exp;
}

void Operation::Kernel::setOperatorType( Operation::OperatorType type )
{
	mOperatorType = type;
}

void Operation::Kernel::setOutputExpression( const string& exp )
{
	mExpressionOutput = exp;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

Operation::Operation()
{
	mKernels.push_back( Kernel() );
}

Operation::Operation( const Operation::QualifierMap& qualifers, 
					  const vector<Operation::Kernel>& kernels )
: mKernels( kernels ), mQualifiers( qualifers )
{
}

map<string, geom::Attrib> Operation::getDefaultFragmentInputNameToSemanticMap()
{
	static map<string, geom::Attrib> m;
	if ( m.empty() ) {
		GlslProg::AttribSemanticMap semanticMap = GlslProg::getDefaultAttribNameToSemanticMap();
		for ( GlslProg::AttribSemanticMap::const_iterator iter = semanticMap.begin(); iter != semanticMap.end(); ++iter ) {
			string name = iter->first;
			if ( name.size() >= 2 ) {
				name.erase( 0, 2 );
				name		= "v" + name; // Swap "ci" for "v" (varying)
				m[ name ]	= iter->second;
			}
		}
	}
	return m;
}

map<geom::Attrib, string> Operation::getSemanticToDefaultFragmentInputNameMap()
{
	static map<geom::Attrib, string> m;
	if ( m.empty() ) {
		map<string, geom::Attrib> semanticMap = getDefaultFragmentInputNameToSemanticMap();
		for ( map<string, geom::Attrib>::const_iterator iter = semanticMap.begin(); iter != semanticMap.end(); ++iter ) {
			m[ iter->second ] = iter->first;
		}
	}
	return m;
}

map<int32_t, string> Operation::getSemanticToDefaultUniformNameMap()
{
	static map<int32_t, string> m;
	if ( m.empty() ) {
		GlslProg::UniformSemanticMap semanticMap = GlslProg::getDefaultUniformNameToSemanticMap();
		for ( GlslProg::UniformSemanticMap::const_iterator iter = semanticMap.begin(); iter != semanticMap.end(); ++iter ) {
			m[ iter->second ] = iter->first;
		}
	}
	return m;
}

map<geom::Attrib, string> Operation::getSemanticToDefaultVertexInputNameMap()
{
	static map<geom::Attrib, string> m;
	if ( m.empty() ) {
		GlslProg::AttribSemanticMap semanticMap = GlslProg::getDefaultAttribNameToSemanticMap();
		for ( GlslProg::AttribSemanticMap::const_iterator iter = semanticMap.begin(); iter != semanticMap.end(); ++iter ) {
			m[ iter->second ] = iter->first;
		}
	}
	return m;
}

string Operation::kernelToString( const Operation& op )
{
	string output = "";
	size_t i = 0;
	for ( vector<Kernel>::const_iterator iter = op.getKernels().begin(); iter != op.getKernels().end(); ++iter, ++i ) {
		string index	= cinder::toString( i );
		output			+= "\t";
#if defined( CINDER_GL_ES_2 )
		output			+= "highp ";
#endif
		output			+= "vec4 ";
		output			+= CI_GLSL_OUTPUT_NAME;
		output			+= index + ";\r\n";
		output			+= "\t{\r\n";
		output			+= "\t\t" + iter->getBodyExpression() + "\r\n";
		output			+= "\t\t";
		output			+= CI_GLSL_OUTPUT_NAME;
		output			+= index + " = " + iter->getOutputExpression() + ";\r\n";
		output			+= "\t}\r\n";
	}
	return output;
}

string Operation::outputToString( const Operation& op )
{
	string output = "";
#if defined( CINDER_GL_ES_2 )
	output += "highp ";
#endif
	output += "";
	size_t i = 0;
	for ( vector<Kernel>::const_iterator iter = op.getKernels().begin(); iter != op.getKernels().end(); ++iter, ++i ) {
		if ( iter != op.getKernels().begin() ) {
			output += " ";
			switch ( iter->getOperatorType() ) {
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
			output += " ";
		}
		output += CI_GLSL_OUTPUT_NAME + cinder::toString( i );
	}
	return output;
}

string Operation::qualifiersToString( const QualifierMap& qualifiers, bool isFragment )
{
	string output = "";
	for ( map<string, Operation::Qualifier>::const_iterator iter = qualifiers.begin(); iter != qualifiers.end(); ++iter ) {
		const Qualifier& q	= iter->second;
		string name			= iter->first;
		string line			= "";
		
		string storage		= "";
		switch ( q.mStorage ) {
			case QualifierStorage_Const:
				storage = "const";
				break;
			case QualifierStorage_Input:
#if defined( CINDER_GL_ES_2 )
				if ( isFragment ) {
					storage = "varying";
				} else {
					storage = "attribute";
				}
#else
				storage = "in";
#endif
				break;
			case QualifierStorage_None:
				break;
			case QualifierStorage_Output:
#if defined( CINDER_GL_ES_2 )
				storage = "varying";
#else
				storage = "out";
#endif
				break;
			case QualifierStorage_Uniform:
				storage = "uniform";
				break;
		}
		
#if defined( CINDER_GL_ES_2 )
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
			case QualifierType_Mat3:
				type = "mat3";
				break;
			case QualifierType_Mat4:
				type = "mat4";
				break;
			case QualifierType_None:
				break;
			case QualifierType_Sampler2d:
				type = "sampler2D";
				break;
			case QualifierType_SamplerCube:
				type = "samplerCube";
				break;
#if !defined( CINDER_GL_ES_2 )
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
			case QualifierType_Mat2x2:
				type = "mat2x2";
				break;
			case QualifierType_Mat2x3:
				type = "mat2x3";
				break;
			case QualifierType_Mat2x4:
				type = "mat2x4";
				break;
			case QualifierType_Mat3x2:
				type = "mat3x2";
				break;
			case QualifierType_Mat3x3:
				type = "mat3x3";
				break;
			case QualifierType_Mat3x4:
				type = "mat3x4";
				break;
			case QualifierType_Mat4x2:
				type = "mat4x2";
				break;
			case QualifierType_Mat4x3:
				type = "mat4x3";
				break;
			case QualifierType_Mat4x4:
				type = "mat4x4";
				break;
			case QualifierType_Sampler1d:
				type = "sampler1D";
				break;
			case QualifierType_Sampler2dShadow:
				type = "sampler2DShadow";
				break;
			case QualifierType_Sampler3d:
				type = "sampler3D";
				break;
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
#if defined( CINDER_GL_ES_2 )
		if ( !precision.empty() ) {
			line += precision + " ";
		}
#endif
		if ( !type.empty() ) {
			line += type + " ";
		}
		line += name;
		
		if ( q.mCount > 1 ) {
			line += "[ " + cinder::toString( q.mCount ) + " ]";
		}
		if ( q.mStorage == QualifierStorage_Const && !q.mValue.empty() ) {
			line += " = " + q.mValue;
		}
		
		line	+= ";\r\n";
		output	+= line;
	}
	
	return output;
}

string Operation::versionToString( const Operation& op )
{
	string output	= "#version ";
#if defined( CINDER_GL_ES_2 )
	output += "100 es";
#elif defined( CINDER_GL_ES_3 )
	output += "300 es";
#else
	output += "150";
#endif
	return output;
}

void Operation::setQualifier( string& oldName, const string& newName, 
							  const Operation::Qualifier& q )
{
	if ( mQualifiers.find( oldName ) != mQualifiers.end() ) {
		mQualifiers.erase( oldName );
	}
	oldName = newName;
	mQualifiers[ oldName ] = q;
}

const vector<Operation::Kernel>& Operation::getKernels() const
{
	return mKernels;
}

const Operation::QualifierMap& Operation::getQualifiers() const
{
	return mQualifiers;
}

string Operation::toString() const
{
	string output;
	output = Operation::versionToString( *this ) + "\r\n";
	output += CI_GLSL_MAIN_OPEN;
	output += CI_GLSL_MAIN_CLOSE;
	return output;
}
	
/////////////////////////////////////////////////////////////////////////////////////////////////

Operation::Exception::Exception( const string& msg ) throw()
{
	sprintf( mMessage, "%s", msg.c_str() );
}

FragmentOperation::ExcQualifierMergeCountMismatch::ExcQualifierMergeCountMismatch( const string& msg ) throw()
: Operation::Exception( msg )
{
}
	
#if defined( CINDER_GL_ES_2 )
FragmentOperation::ExcQualifierMergePrecisionMismatch::ExcQualifierMergePrecisionMismatch( const string& msg ) throw()
: Operation::Exception( msg )
{
}
#endif
	
FragmentOperation::ExcQualifierMergeStorageMismatch::ExcQualifierMergeStorageMismatch( const string& msg ) throw()
: Operation::Exception( msg )
{
}

FragmentOperation::ExcQualifierMergeTypeMismatch::ExcQualifierMergeTypeMismatch( const string& msg ) throw()
: Operation::Exception( msg )
{
}

FragmentOperation::ExcQualifierMergeValueMismatch::ExcQualifierMergeValueMismatch( const string& msg ) throw()
: Operation::Exception( msg )
{
}

ostream& operator<<( ostream& out, const Operation& op )
{
	out << op.toString();
	return out;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
	
FragmentOperation::FragmentOperation()
: Operation()
{
}

FragmentOperation::FragmentOperation( const Operation::QualifierMap& qualifers, 
									  const vector<Operation::Kernel>& kernels )
: Operation( qualifers, kernels )
{
}

FragmentOperation FragmentOperation::operator+( const FragmentOperation& rhs )
{
	FragmentOperation lhs = *this;
	lhs += rhs;
	return lhs;
}
	
FragmentOperation FragmentOperation::operator*( const FragmentOperation& rhs )
{
	FragmentOperation lhs = *this;
	lhs *= rhs;
	return lhs;
}
	
FragmentOperation FragmentOperation::operator-( const FragmentOperation& rhs )
{
	FragmentOperation lhs = *this;
	lhs -= rhs;
	return lhs;
}
	
FragmentOperation FragmentOperation::operator/( const FragmentOperation& rhs )
{
	FragmentOperation lhs = *this;
	lhs /= rhs;
	return lhs;
}

void FragmentOperation::operator+=( const FragmentOperation& rhs )
{
	merge( rhs, OperatorType_Add );
}
	
void FragmentOperation::operator*=( const FragmentOperation& rhs )
{
	merge( rhs, OperatorType_Multiply );
}
	
void FragmentOperation::operator-=( const FragmentOperation& rhs )
{
	merge( rhs, OperatorType_Subtract );
}
	
void FragmentOperation::operator/=( const FragmentOperation& rhs )
{
	merge( rhs, OperatorType_Divide );
}

string FragmentOperation::toString() const
{
	string output;
	output = Operation::versionToString( *this ) + "\r\n";
	output += Operation::qualifiersToString( mQualifiers, true ) + "\r\n";
#if !defined( CINDER_GL_ES_2 )
	output += "out vec4 ";
	output += CI_GLSL_OUTPUT_NAME_FRAG;
	output += ";\r\n\r\n";
#endif
	output += CI_GLSL_MAIN_OPEN;
	output += kernelToString( *this );
	output += "\r\n";
	output += "\t";
	output += CI_GLSL_OUTPUT_NAME_FRAG;
	output += " = ";
	output += outputToString( *this );
	output += ";\r\n";
	output += CI_GLSL_MAIN_CLOSE;
	return output;
}

void FragmentOperation::merge( const Operation& rhs, OperatorType type )
{
	if ( !rhs.mKernels.empty() ) {
		mQualifiers = mergeQualifiers( mQualifiers, rhs.mQualifiers );
		for ( vector<Operation::Kernel>::const_iterator iter = rhs.mKernels.begin(); iter != rhs.mKernels.end(); ++iter ) {
			mKernels.push_back( *iter );
			if ( iter == rhs.mKernels.begin() ) {
				mKernels.back().setOperatorType( type );
			}
		}
	}
}

Operation::QualifierMap FragmentOperation::mergeQualifiers( const Operation::QualifierMap& a, const Operation::QualifierMap& b )
{
	QualifierMap merged = a;
	for ( map<string, Operation::Qualifier>::const_iterator iter = b.begin(); iter != b.end(); ++iter ) {
		const string& name = iter->first;
		if ( merged.find( name ) != merged.end() ) {
			const Qualifier& qa = merged.find( name )->second;
			const Qualifier& qb = iter->second;
			if ( qa.mCount != qb.mCount ) {
				throw FragmentOperation::ExcQualifierMergeCountMismatch( "Unable to merge qualifiers. Count mismatch for \"" + name + "\"." );
#if defined( CINDER_GL_ES_2 )
			} else if ( qa.mPrecision != qb.mPrecision ) {
				throw Operation::ExcQualifierMergePrecisionMismatch( "Unable to merge qualifiers. Precision mismatch for \"" + name + "\"." );
#endif
			} else if ( qa.mStorage != qb.mStorage ) {
				throw FragmentOperation::ExcQualifierMergeStorageMismatch( "Unable to merge qualifiers. Storage mismatch for \"" + name + "\"." );
			} else if ( qa.mType != qb.mType ) {
				throw FragmentOperation::ExcQualifierMergeTypeMismatch( "Unable to merge qualifiers. Type mismatch for \"" + name + "\"." );
			} else if ( qa.mValue != qb.mValue ) {
				throw FragmentOperation::ExcQualifierMergeValueMismatch( "Unable to merge qualifiers. Value mismatch for \"" + name + "\"." );
			}
		} else {
			merged[ name ] = iter->second;
		}
	}
	return merged;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

VertexOperation::VertexOperation()
: Operation()
{
}

VertexOperation::VertexOperation( const Operation::QualifierMap& qualifers, 
					  const vector<Operation::Kernel>& kernels )
: Operation( qualifers, kernels )
{
}

string VertexOperation::vertexOutputAssignmentToString( const QualifierMap& q )
{
	string output = "";
	for ( QualifierMap::const_iterator iter = q.begin(); iter != q.end(); ++iter ) {
		const string name	= iter->first;
		const Qualifier& q	= iter->second;
		if ( q.mStorage == QualifierStorage_Output ) {
			const map<string, geom::Attrib>& semanticMap = getDefaultFragmentInputNameToSemanticMap();
			if ( semanticMap.find( name ) != semanticMap.end() ) {
				geom::Attrib a = semanticMap.at( name );
				const map<geom::Attrib, string>& attribMap = getSemanticToDefaultVertexInputNameMap();
				if ( attribMap.find( a ) != attribMap.end() ) {
					const string& attr = attribMap.at( a );
					output += "\t" + name + " = " + attr + ";\r\n";
				}
			}
		}
	}
	return output;
}

string VertexOperation::toString() const
{
	string output;
	output = Operation::versionToString( *this ) + "\r\n";
	output += Operation::qualifiersToString( mQualifiers, false ) + "\r\n";
	output += CI_GLSL_MAIN_OPEN;
	output += vertexOutputAssignmentToString( mQualifiers ) + "\r\n";
	output += kernelToString( *this ) + "\r\n";
	output += "\t";
	output += CI_GLSL_OUTPUT_NAME_VERT;
	output += " = " + outputToString( *this ) + ";\r\n";
	output += CI_GLSL_MAIN_CLOSE;
	return output;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

Effect::Effect( const VertexOperation& vert, const FragmentOperation& frag )
: mFragmentOperation( frag ), mVertexOperation( vert )
{
	createVertexOutputs();
}

FragmentOperation& Effect::getFragmentOperation()
{
	return mFragmentOperation;
}

const FragmentOperation& Effect::getFragmentOperation() const
{
	return mFragmentOperation;
}

VertexOperation& Effect::getVertexOperation()
{
	return mVertexOperation;
}

const VertexOperation& Effect::getVertexOperation() const
{
	return mVertexOperation;
}

void Effect::createVertexOutputs()
{
	Operation::QualifierMap& f = mFragmentOperation.mQualifiers;
	Operation::QualifierMap& v = mVertexOperation.mQualifiers;

	for ( Operation::QualifierMap::iterator iter = f.begin(); iter != f.end(); ++iter ) {
		Operation::Qualifier& q = iter->second;
		if ( q.mStorage == Operation::QualifierStorage_Input ) {
			const string& name = iter->first;
			Operation::Qualifier out( q );
			out.mStorage	= Operation::QualifierStorage_Output;
			v[ name ]		= out;	

			const map<string, geom::Attrib>& semanticMap = Operation::getDefaultFragmentInputNameToSemanticMap();
			if ( semanticMap.find( name ) != semanticMap.end() ) {
				geom::Attrib a = semanticMap.at( name );
				const map<geom::Attrib, string>& attribMap = Operation::getSemanticToDefaultVertexInputNameMap();
				if ( attribMap.find( a ) != attribMap.end() ) {
					const string& attr	= attribMap.at( a );
					v[ attr ]			= q;
				}
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////
	
VertexPassThrough::VertexPassThrough()
: VertexOperation()
{
	string pos = getSemanticToDefaultVertexInputNameMap()[ geom::POSITION ];
	string mvp = getSemanticToDefaultUniformNameMap()[ UNIFORM_MODEL_VIEW_PROJECTION ];
	mQualifiers[ pos ] = Qualifier( QualifierStorage_Input, QualifierType_Vec4 );
	mQualifiers[ mvp ] = Qualifier( QualifierStorage_Uniform, QualifierType_Mat4 );
	
	mKernels.front().outputExpression( mvp + " * " + pos );
}

/////////////////////////////////////////////////////////////////////////////////////////////////

FragmentColor::FragmentColor()
: FragmentOperation()
{
	string attr = getSemanticToDefaultFragmentInputNameMap()[ geom::COLOR ];
	mQualifiers[ attr ] = Qualifier( QualifierStorage_Input, QualifierType_Vec4 );;

	mKernels.front().outputExpression( attr );
}

FragmentTexture2d::FragmentTexture2d()
: FragmentOperation()
{
	string attr = getSemanticToDefaultFragmentInputNameMap()[ geom::TEX_COORD_0 ];
	mQualifiers[ attr ] = Qualifier( QualifierStorage_Input, QualifierType_Vec4 );
	texture( "uTexture0" );
}

FragmentTexture2d& FragmentTexture2d::texture( const string& uniformName )
{
	setQualifier( mNameTexture, uniformName, Qualifier( QualifierStorage_Uniform, QualifierType_Sampler2d ) );

	string attr = getSemanticToDefaultFragmentInputNameMap()[ geom::TEX_COORD_0 ];
	mKernels.front().bodyExpression( "vec4 color = texture( " + mNameTexture + ", " + attr + ".st );" )
		.outputExpression( "color" );
	return *this;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

FragmentExposure::FragmentExposure( FragmentOperation* op )
: FragmentOperation(), mInput( op )
{
	exposure( 1.0f ).offset( 0.0f );
}

FragmentExposure& FragmentExposure::exposure( const string& uniformName )
{
	setQualifier( mNameExposure, uniformName, Qualifier( QualifierStorage_Uniform, QualifierType_Float ) );
	return *this;
}

FragmentExposure& FragmentExposure::exposure( float v )
{
	setQualifier( mNameExposure, "kExposure", Qualifier( QualifierStorage_Const, QualifierType_Float, cinder::toString( v ) ) );
	return *this;
}

FragmentExposure& FragmentExposure::input( FragmentOperation* op )
{
	mInput = op;
	return *this;
}

FragmentExposure& FragmentExposure::offset( const string& uniformName )
{
	setQualifier( mNameOffset, uniformName, Qualifier( QualifierStorage_Uniform, QualifierType_Float ) );
	return *this;
}

FragmentExposure& FragmentExposure::offset( float v )
{
	setQualifier( mNameOffset, "kOffset", Qualifier( QualifierStorage_Const, QualifierType_Float, cinder::toString( v ) ) );
	return *this;
}

string FragmentExposure::toString() const
{
	QualifierMap q		= mergeQualifiers( mQualifiers, mInput->getQualifiers() );
	string inputKernel	= Operation::kernelToString( *mInput );
	string inputOutput	= Operation::outputToString( *mInput );
	string exposureName	= CI_GLSL_OUTPUT_NAME;
	exposureName		+= "Exposure";

	string kernelBody;
	kernelBody = inputKernel + "\r\n";
	kernelBody += "\t";
#if defined( CINDER_GL_ES_2 )
	kernelBody += "highp ";
#endif
	kernelBody += "vec4 " + exposureName + " = " + inputOutput + ";\r\n";
	kernelBody += "\t" + exposureName + " = pow( " + exposureName +  ", vec4( " + mNameExposure + " + " + mNameOffset + " ) );\r\n";

	vector<Kernel> k;
	k.push_back( Kernel().bodyExpression( kernelBody ).outputExpression( exposureName ) );

	FragmentOperation op( q, k );

	return op.toString();

	// TODO this is clumsy -- should leverage base class / kernel more
//	string output;
//	output = Operation::versionToString( *this ) + "\r\n";
//	output += Operation::qualifiersToString( q, true ) + "\r\n";
//#if !defined( CINDER_GL_ES_2 )
//	output += "out vec4 ";
//	output += CI_GLSL_OUTPUT_NAME_FRAG;
//	output += ";\r\n\r\n";
//#endif
//	output += CI_GLSL_MAIN_OPEN;
//	output += inputKernel + "\r\n";
//	output += "\t";
//#if defined( CINDER_GL_ES_2 )
//	output += "highp ";
//#endif
//	output += "vec4 " + exposureName + " = " + inputOutput + ";\r\n";
//	output += "\t" + exposureName + " = pow( " + exposureName +  ", vec4( " + mNameExposure + " + " + mNameOffset + " ) );\r\n";
//	output += "\t";
//	output += CI_GLSL_OUTPUT_NAME_FRAG;
//	output += " = " + exposureName + ";\r\n";
//	output += CI_GLSL_MAIN_CLOSE;
//	return output;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

Exposure::Exposure( float exposure, float offset )
{
	mFragmentOperation	= FragmentTexture2d() * FragmentColor();
	mFragmentExposure	= FragmentExposure( &mFragmentOperation ).exposure( exposure ).offset( offset );
	mVertexOperation	= VertexPassThrough();
	createVertexOutputs();
}

FragmentOperation& Exposure::getFragmentOperation()
{
	return mFragmentExposure;
}

const FragmentOperation& Exposure::getFragmentOperation() const
{
	return mFragmentExposure;
}

Exposure& Exposure::exposure( const string& uniformName )
{
	mFragmentExposure.exposure( uniformName );
	return *this;
}

Exposure& Exposure::exposure( float v )
{
	mFragmentExposure.exposure( v );
	return *this;
}

Exposure& Exposure::input( const FragmentOperation& op )
{
	mFragmentOperation = op;
	return *this;
}

Exposure& Exposure::offset( const string& uniformName )
{
	mFragmentExposure.offset( uniformName );
	return *this;
}

Exposure& Exposure::offset( float v )
{
	mFragmentExposure.offset( v );
	return *this;
}

} } }
 