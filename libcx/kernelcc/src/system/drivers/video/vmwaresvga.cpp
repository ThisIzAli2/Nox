/**
 * @author Ali Mirmohammad
 * @file vmwaresvga.cpp
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
#include <system/drivers/video/vmwaresvga.h>

#include <system/system.h>

using namespace Novanix;
using namespace Novanix::common;
using namespace Novanix::system;
using namespace Novanix::system::drivers;
using namespace Novanix::core;

VMWARESVGAII::VMWARESVGAII(PCIDevice* pciDev)
: Driver("VMWare SVGAII", "VMWare Graphics Adapter"),
  GraphicsDevice("WMWare SVGAII Adapter")
{
    this->width = 0;
    this->height = 0;
    this->bpp = 0;
    this->framebufferPhys = 0;

    this->pciDevice = pciDev;
}

void VMWARESVGAII::WriteRegister(uint32_t index, uint32_t value)
{
    outportl(pciDevice->portBase + SVGA_INDEX_PORT, index);
    outportl(pciDevice->portBase + SVGA_VALUE_PORT, value);
}
uint32_t VMWARESVGAII::ReadRegister(uint32_t index)
{
    outportl(pciDevice->portBase + SVGA_INDEX_PORT, index);
    return inportl(pciDevice->portBase + SVGA_VALUE_PORT);
}   

bool VMWARESVGAII::Initialize()
{
    Log(Info, "Intializing VMWare Graphics Device");
    
    //Enable Device Memory
    uint16_t com = System::pci->Read(pciDevice->bus, pciDevice->device, pciDevice->function, 0x04);
    com |= 0x0007;
    System::pci->Write(pciDevice->bus, pciDevice->device, pciDevice->function, 0x04, com);

    WriteRegister(SVGA_REG_ID, SVGA_ID_2);
    if (ReadRegister(SVGA_REG_ID) != SVGA_ID_2)
        return false;

    this->framebufferPhys = ReadRegister(SVGA_REG_FB_START);
    Log(Info, "Framebuffer is at: %x", this->framebufferPhys);

    Log(Warning, "Replacing fallback gfx device");
    GraphicsDevice* old = System::gfxDevice;
    System::gfxDevice = this;
    delete old;

    return true;
}

bool VMWARESVGAII::SelectBestVideoMode()
{
    WriteRegister(SVGA_REG_FB_Width, DEFAULT_SCREEN_WIDTH);
    WriteRegister(SVGA_REG_FB_Height, DEFAULT_SCREEN_HEIGHT);
    WriteRegister(SVGA_REG_FB_BitsPerPixel, DEFAULT_SCREEN_BPP);
    WriteRegister(SVGA_REG_FB_Enable, 1);

    this->width = DEFAULT_SCREEN_WIDTH;
    this->height = DEFAULT_SCREEN_HEIGHT;
    this->bpp = DEFAULT_SCREEN_BPP;
    
    return true;
}