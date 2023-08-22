#include "Transform.h"

namespace meow {

	void Transform::Read(const rapidjson::Value& value)
	{
		READ_DATA(value, position);
		READ_DATA(value, rotation);
		READ_DATA(value, scale);
	}

}