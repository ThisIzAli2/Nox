/**
 ** This file is part of Novanix kernel.
 @Credits Remco123

**AliNix is free software: you can redistribute it and/or modify
**it under the terms of the GNU Affero General Public License as published by
**the Free Software Foundation, either version 3 of the License, or
**(at your option) any later version.

**AliNix is distributed in the hope that it will be useful,
**but WITHOUT ANY WARRANTY; without even the implied warranty of
**MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
**GNU Affero General Public License for more details.

**You should have received a copy of the GNU Affero General Public License
**along with AliNix. If not, see <https://www.gnu.org/licenses/>.
*/
/**
 * MIT License

Copyright (c) 2020 Remco123

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#include <system/bootconsole.h>
#include <typing.hpp>

using namespace Novanix;
using namespace Novanix::common;
using namespace Novanix::system;

/*/////////////////
// Static variable initialisations
/*/////////////////
INTEGER BootConsole::XOffset = 0;
INTEGER BootConsole::YOffset = 0;
uint8_t BootConsole::BackgroundColor = VGA_COLOR_BLACK; //Default console background color
uint8_t BootConsole::ForegroundColor = VGA_COLOR_WHITE; //Default console foreground color
BOOL BootConsole::writeToSerial = false;

/*/////////////////
// Variables
/*/////////////////
static uint16_t* videoMemory = (uint16_t*)0xC00B8000;


/*/////////////////
// Private functions
/*/////////////////
VOID BootConsole::Scroll()
{
    for(INTEGER i = 0; i < 24; i++){
        for (INTEGER m = 0; m < 80; m++){
            videoMemory[i * 80 + m] = videoMemory[(i + 1) * 80 + m];
        }
    }

    for(INTEGER x = 0; x < 80; x++)
    {
        uint16_t attrib = (BackgroundColor << 4) | (ForegroundColor & 0x0F);
        volatile uint16_t * where;
        where = (volatile uint16_t *)videoMemory + (24 * VGA_WIDTH + x) ;
        *where = ' ' | (attrib << 8);
    }
}



/*/////////////////
// Public functions
/*/////////////////
VOID BootConsole::Init(BOOL enableSerial)
{
    BootConsole::writeToSerial = enableSerial;
    if(enableSerial)
    {
        Serialport::Init(COMPort::COM1);
        BootConsole::WriteLine("Start of serial log for Novanix");
    }
}

VOID BootConsole::Write(char c)
{
    static char* str = " ";
    str[0] = c;
    Write(str);
}

VOID BootConsole::Write(char* str)
{
    if (writeToSerial)
        Serialport::WriteStr(str);

    for(INTEGER i = 0; str[i] != '\0'; ++i)
    {
        switch(str[i])
        {
            case '\n':
                XOffset = 0;
                YOffset++;
                break;
            case '\t':
                Write("    "); //4 spaces for tab
                break;
            default:
                uint16_t attrib = (BackgroundColor << 4) | (ForegroundColor & 0x0F);
                volatile uint16_t * where;
                where = (volatile uint16_t *)videoMemory + (YOffset * 80 + XOffset) ;
                *where = str[i] | (attrib << 8);
                XOffset++;
                break;
        }

        if(XOffset >= 80)
        {
            XOffset = 0;
            YOffset++;
        }

        if(YOffset >= 25)
        {
            Scroll();
            XOffset = 0;
            YOffset = 24;
        }
    }
}
VOID BootConsole::WriteLine(char* str)
{
    BootConsole::Write(str);
    BootConsole::Write("\n");
}
VOID BootConsole::WriteLine()
{
    BootConsole::Write("\n");
}
VOID BootConsole::Clear()
{
    for(INTEGER y = 0; y < VGA_HEIGHT; y++)
        for(INTEGER x = 0; x < VGA_WIDTH; x++) {
                uint16_t attrib = (BackgroundColor << 4) | (ForegroundColor & 0x0F);
                volatile uint16_t * where;
                where = (volatile uint16_t *)videoMemory + (y * VGA_WIDTH + x) ;
                *where = ' ' | (attrib << 8);
        }

    XOffset = 0;
    YOffset = 0;
}
uint16_t* BootConsole::GetBuffer()
{
    return videoMemory;
}

VOID BootConsole::SetX(INTEGER x)
{
    XOffset = x;
}
VOID BootConsole::SetY(INTEGER y)
{
    YOffset = y;
}