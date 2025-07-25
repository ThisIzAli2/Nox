/**
 * @author Ali Mirmohammad
 * @file ports.h
 ** This file is part of Novanix.

**Novanix is free software: you can redistribute it and/or modify
**it under the terms of the GNU Affero General Public License as published by
**the Free Software Foundation, either version 3 of the License, or
**(at your option) any later version.

**Novanix is distributed in the hope that it will be useful,
**but WITHOUT ANY WARRANTY; without even the implied warranty of
**MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
**GNU Affero General Public License for more details.

**You should have received a copy of the GNU Affero General Public License
**along with Novanix. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef ___NOVANIX_PORTS_H
#define ___NOVANIX_PORTS_H


#include <common/types.h>

using namespace Novanix::common;

// inline void outb(uint16_t port, uint8_t data) {
//     asm volatile ("outb %0, %1" : : "a"(data), "dN"(port));
// }

// inline uint8_t inb(uint16_t port) {
//     uint8_t result;
//     asm volatile ("inb %1, %0" : "=a"(result) : "dN"(port));
//     return result;
// }

#endif