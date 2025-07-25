/**
 * @author Ali Mirmohammad
 * @file pit.cpp
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
#include <system/components/pit.h>
#include <core/idt.h>

using namespace Novanix;
using namespace Novanix::common;
using namespace Novanix::core;
using namespace Novanix::system;

PIT::PIT()
: SystemComponent("PIT", "Legacy Programmable Interval Timer"),
InterruptHandler(IDT_INTERRUPT_OFFSET + 0)
{
    timer_ticks = 0;

    uint64_t divisor = 1193180 / PIT_FREQUENCY; //Default is 1000 Hz

    outportb(0x43, 0x34);
    outportb(0x40, (uint8_t)divisor);
    outportb(0x40, (uint8_t)(divisor >> 8));
}

uint32_t PIT::HandleInterrupt(uint32_t esp)
{
    timer_ticks++;

    return esp;
}
void PIT::Sleep(uint32_t ms)
{
    uint64_t targetTicks = timer_ticks + ms;
    while(timer_ticks < targetTicks)
        asm ("hlt"); // Wait for next interrupt
}


void PIT::PlaySound(common::uint32_t nFrequence)
{
    uint32_t Div;
 	uint8_t tmp;
 
    //Set the PIT to the desired frequency
 	Div = 1193180 / nFrequence;
 	outportb(0x43, 0xb6);
 	outportb(0x42, (uint8_t) (Div) );
 	outportb(0x42, (uint8_t) (Div >> 8));
 
    //And play the sound using the PC speaker
 	tmp = inportb(0x61);
  	if (tmp != (tmp | 3)) {
 		outportb(0x61, tmp | 3);
 	}
}
void PIT::NoSound()
{
    uint8_t tmp = inportb(0x61) & 0xFC;
 
 	outportb(0x61, tmp);
}
void PIT::Beep()
{
    Beep(800); //800 is default beep frequency
}

void PIT::Beep(common::uint32_t freq)
{
    Beep(freq, 200); //200 is default beep duration
}
void PIT::Beep(common::uint32_t freq, common::uint32_t duration)
{
    if(duration == 0)
        return;
    if(freq == 0)
        return;

    PlaySound(freq);
 	Sleep(duration);
 	NoSound();
}

uint64_t PIT::Ticks()
{
    return this->timer_ticks;
}