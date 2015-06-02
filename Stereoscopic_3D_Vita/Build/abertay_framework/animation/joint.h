#ifndef _JOINT_H
#define _JOINT_H

#include <abertay_framework.h>
#include <maths/matrix44.h>
#include <maths/transform.h>
#include <system/string_id.h>

#include <ostream>
#include <istream>

namespace abfw
{

	struct Joint
	{
		StringId name_id;
		Matrix44 inv_bind_pose;	// inverse bind pose transform
		Int32 parent;			// parent joint index or -1 if it's the root

		bool Read(std::istream& stream);
		bool Write(std::ostream& stream) const;
	};

	typedef Transform JointPose;
}

#endif // _JOINT_H