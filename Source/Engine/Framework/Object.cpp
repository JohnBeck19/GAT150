#include "Object.h"

namespace meow
{

	void Object::Read(const json_t& value)
	{
		READ_DATA(value, name);
	}
}