// sam.hpp -- Modified version of SAM's main.c file, to integrate into GORP.
// Modified from SAM code by Raine "Gravecat" Simmons, 2023-2025.

#pragma once

#include <string>

namespace gorp {
namespace sam {

void sam_say(std::string phrase);
void cleanup();

} } // namespace sam, gorp
