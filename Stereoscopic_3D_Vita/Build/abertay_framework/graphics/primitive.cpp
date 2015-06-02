#include <graphics/primitive.h>
#include <cstddef>

namespace abfw
{

Primitive::Primitive() :
num_indices_(0),
texture_(NULL),
type_(UNDEFINED)
{
}

Primitive::~Primitive()
{
}

}