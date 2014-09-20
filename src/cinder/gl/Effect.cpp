#include "cinder/gl/Effect.h"

namespace cinder { namespace gl { namespace effect {

Operation::Qualifier::Qualifier()
: mCount( 1 ), mPrecision( QualifierPrecision::HIGH ), 
mStorage( QualifierStorage::NONE ), mType( QualifierType::NONE )
{
}

} } }
 