#include "cinder/gl/Effect.h"

namespace cinder { namespace gl { namespace effect {

using namespace std;
	
Operation::Qualifier::Qualifier()
: mCount( 1 ), mPrecision( QualifierPrecision_High ),
mStorage( QualifierStorage_None ), mType( QualifierType_None )
{
}

Operation::Operation()
{
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
	mergeQualifiers( rhs.mQualifiers );
}
	
void Operation::operator*=( const Operation& rhs )
{
	mergeQualifiers( rhs.mQualifiers );
}
	
void Operation::operator-=( const Operation& rhs )
{
	mergeQualifiers( rhs.mQualifiers );
}
	
void Operation::operator/=( const Operation& rhs )
{
	mergeQualifiers( rhs.mQualifiers );
}

void Operation::operator%=( const Operation& rhs )
{
	mergeQualifiers( rhs.mQualifiers );
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
			} else if ( a.mPrecision != b.mPrecision ) {
				throw Operation::ExcQualifierMergePrecisionMismatch( "Unable to merge qualifiers. Precision mismatch for \"" + name + "\"." );
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
	
Operation::Exception::Exception( const string& msg ) throw()
{
	sprintf( mMessage, "%s", msg.c_str() );
}

Operation::ExcQualifierMergeCountMismatch::ExcQualifierMergeCountMismatch( const string& msg ) throw()
: Operation::Exception( msg )
{
}
	
Operation::ExcQualifierMergePrecisionMismatch::ExcQualifierMergePrecisionMismatch( const string& msg ) throw()
: Operation::Exception( msg )
{
}
	
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
	
} } }
 