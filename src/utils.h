/*
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
// CONTRIBUTORS AND COPYRIGHT HOLDERS (c) 2013:
// Bob Mottram (bob@robotics.uk.to)
// Dag Robøle (BM-2DAS9BAs92wLKajVy9DS1LFcDiey5dxp5c)

#ifndef BM_UTILS_H
#define BM_UTILS_H

#include <stdint.h>
#include <string>
#include <botan/auto_rng.h>
#include "btypes.h"

namespace bm {

namespace utils {

extern const std::string BASE58;

Botan::AutoSeeded_RNG& random_number_generator();
SecureVector random_bytes(uint32_t count);
uint32_t seconds_since_epoc();
std::string remove_prefix(const std::string& source, const std::string& prefix);

} //namespace utils

} //namespace bm

#endif
