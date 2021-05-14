#ifndef ENTITY_H
#define ENTITY_H

#include <stdio.h>
#include <bitset>

namespace Lynx {

	using Signature = std::bitset<256>;

	struct Entity {
		unsigned int id;
	};
}

#endif