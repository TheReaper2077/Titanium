#pragma once

#include <string>

namespace ti {
	namespace Component {
		struct Tag {
			std::string name;
			std::string id;
			bool save = false;
		};
	}
}