/**
 * @author Ali Mirmohammad
 * @file VM8086Manager.cpp
 * @credits Remco123
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
#include <system/virtual8086/VM86Manager.h>
#include <system/system.h>
#include <system/memory/deviceheap.h>
#include <typing.hpp>

using namespace Novanix;
using namespace Novanix::common;
using namespace Novanix::system;
using namespace Novanix::core;

static const uint16_t codeSegment = 0x2000;
static const uint16_t stackSegment = 0x8000;

CPUState vm86CPUState;

extern "C" uint8_t VM86CodeStart;
extern "C" uint8_t VM86CodeEnd;
extern "C" uint8_t Int86;

extern "C" uintptr_t cpuGetEIP();
extern "C" uintptr_t cpuGetESP();
// vm86
extern "C" VOID cpuEnterV86Int(INTEGER ignored, INTEGER arg1, INTEGER arg2, INTEGER arg3, INTEGER arg4, INTEGER arg6);
extern "C" VOID cpuEnterV86(uint32_t ss, uint32_t esp, uint32_t cs, uint32_t eip, uint32_t eax);

Virtual8086Manager::Virtual8086Manager()
: InterruptHandler(0xFD)
{ 
    // Install int86 trampoline code in conventional memory
    uint32_t codeSize = &VM86CodeEnd - &VM86CodeStart;
    MemoryOperations::memcpy((uint8_t*)(codeSegment << 4), &VM86CodeStart, codeSize);
}

uint32_t Virtual8086Manager::HandleInterrupt(common::uint32_t esp)
{
    CPUState* state = (CPUState*)esp;
    uintptr_t args[] = { state->EAX, state->EBX, state->ECX, state->EDX, state->ESI, state->EDI, state->EBP };
    
    InterruptDescriptorTable::DisableInterrupts();

    MemoryOperations::memcpy(&vm86CPUState, state, sizeof(CPUState));

    TSS::GetCurrent()->esp0 = cpuGetESP();
    TSS::GetCurrent()->eip = cpuGetEIP();

    //BootConsole::WriteLine("[CPU] Entering VM8086 Mode");
    cpuEnterV86(args[1], args[2], args[3], args[4], args[5]);

    return esp;
}

VOID Virtual8086Manager::vm86Enter(uint16_t ss, uint16_t sp, uint16_t cs, uint16_t ip, uint32_t arg)
{
    //Log(Info, "[Virtual8086] Making call to %x", cs);

    // Current state of the scheduler
    BOOL curSchedulerState = false;

    // Avoid task switches during VM86 calls
    if(System::scheduler) {
        curSchedulerState = System::scheduler->Enabled;
        System::scheduler->Enabled = false;
    }

    // Allow access to PAGE 0 (needed for BIOS stuff)
    ((PageTable*)VirtualMemoryManager::GetPageTableAddress(0))->entries[0].present = 1;
    invlpg((VOID*)0);

    // Now make the call
    cpuEnterV86Int(0, ss, sp, cs, ip, arg);

    // Remove access from PAGE 0 again
    ((PageTable*)VirtualMemoryManager::GetPageTableAddress(0))->entries[0].present = 0;
    invlpg((VOID*)0);

    // And enable the scheduler again
    if(System::scheduler) System::scheduler->Enabled = curSchedulerState;
}

VOID Virtual8086Manager::CallInterrupt(uint8_t intNumber, VM86Arguments* regs)
{
    MemoryOperations::memcpy((uint8_t*)((codeSegment << 4) + 0x8000), (uint8_t*)regs, sizeof(VM86Arguments));
    vm86Enter(stackSegment, 0x0000, codeSegment, &Int86 - &VM86CodeStart, intNumber);
    MemoryOperations::memcpy((uint8_t*)regs, (uint8_t*)((codeSegment << 4) + 0x8000), sizeof(VM86Arguments));
}

VOID Virtual8086Manager::ExecuteCode(uint32_t instructionStart, uint32_t args)
{
    vm86Enter(stackSegment, 0x0000, codeSegment, instructionStart - (uint32_t)&VM86CodeStart, args);
}