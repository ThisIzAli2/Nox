/**
 * @author Ali Mirmohammad
 * @file main.cpp
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
#include <new.h>
#include <gui/gui.h>
#include <gui/widgets/window.h>
#include <gui/widgets/button.h>
#include <gui/directgui.h>
#include <proc.h>
#include <syscall.h>
#include <vfs.h>
#include <log.h>

void PoweroffButtonCallback(void* sender, MouseButtonArgs arg);
void RebootButtonCallback(void* sender, MouseButtonArgs arg);
void PoweroffAndEjectButtonCallback(void* sender, MouseButtonArgs arg);

int main(int argc, char** argv)
{
    GUI::SetDefaultFont();
    
    Window* mainWindow = new Window(300, 100 + 30, GUI::Width/2 - 150, GUI::Height/2 - 65);
    mainWindow->titleString = "Power Options";
    mainWindow->backColor = 0xFF150534;

    Button* shutdownButton = new Button("Poweroff");
    shutdownButton->width = 150 - 2;
    shutdownButton->height = 47;
    shutdownButton->x = 1;
    shutdownButton->y = 2;
    shutdownButton->MouseClick += PoweroffButtonCallback;

    Button* rebootButton = new Button("Reboot");
    rebootButton->width = 150 - 2;
    rebootButton->height = 47;
    rebootButton->x = 150 + 1;
    rebootButton->y = 2;
    rebootButton->MouseClick += RebootButtonCallback;

    Button* shutdownAndEjectButton = new Button("Eject CD+Shutdown");
    shutdownAndEjectButton->width = 300 - 2;
    shutdownAndEjectButton->height = 49;
    shutdownAndEjectButton->y = 50;
    shutdownAndEjectButton->x = 1;
    shutdownAndEjectButton->MouseClick += PoweroffAndEjectButtonCallback;

    mainWindow->AddChild(shutdownButton);
    mainWindow->AddChild(shutdownAndEjectButton);
    mainWindow->AddChild(rebootButton);
    
    while(GUI::HasItems()) {
        GUI::DrawGUI();
        GUI::ProcessEvents();
    }

    return 0;
}
 
void PoweroffButtonCallback(void* sender, MouseButtonArgs arg)
{
    DoSyscall(SYSCALL_SHUTDOWN);
}
void RebootButtonCallback(void* sender, MouseButtonArgs arg)
{
    DoSyscall(SYSCALL_REBOOT);
}
void PoweroffAndEjectButtonCallback(void* sender, MouseButtonArgs arg)
{
    Print("EjectDisk returned %d\n", (int)EjectDisk("B:\\"));
    DoSyscall(SYSCALL_SHUTDOWN);
}