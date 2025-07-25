/**
 * @author Ali Mirmohammad
 * @file gdt.cpp
 * @Credits Remco123
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
#include <core/gdt.h>

using namespace Novanix::common;
using namespace Novanix::core;

/*/////////////////
// Variables
/*/////////////////
GDTEntry gdtEntries[6];
GDTPointer gdtPointer;

extern "C" void gdt_flush(uint32_t);

/*/////////////////
// Public functions
/*/////////////////
void GlobalDescriptorTable::SetDescriptor(int number, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
{
    gdtEntries[number].base_low    = (base & 0xFFFF);
    gdtEntries[number].base_middle = (base >> 16) & 0xFF;
    gdtEntries[number].base_high   = (base >> 24) & 0xFF;

    gdtEntries[number].limit_low   = (limit & 0xFFFF);
    gdtEntries[number].granularity = (limit >> 16) & 0x0F;

    gdtEntries[number].granularity |= gran & 0xF0;
    gdtEntries[number].access      = access;
}

GDTEntry* GlobalDescriptorTable::GetDescriptor(int number)
{
    return &gdtEntries[number];
}

void GlobalDescriptorTable::Init()
{
    gdtPointer.limit = (sizeof(GDTEntry) * 6) - 1;
    gdtPointer.base = (uint32_t)&gdtEntries;

    SetDescriptor(0, 0, 0, 0, 0);                // Null segment
    SetDescriptor(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code segment
    SetDescriptor(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment
    SetDescriptor(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User mode code segment
    SetDescriptor(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User mode data segment
    // TSS descriptor will be loaded by the TSS class

    gdt_flush((uint32_t)&gdtPointer);
}