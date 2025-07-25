/**
 * @author Ali Mirmohammad
 * @file gui.h
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
#ifndef __Novanix__GUI__GUI_H
#define __Novanix__GUI__GUI_H

#include <gui/widgets/control.h>
#include <gui/widgets/window.h>
#include <gui/context.h>

namespace Novanix
{
    // Event definitions
    enum GUIEvents
    {
        MouseDown,
        MouseUp,
        MouseMove,
        Keypress,
        MouseScroll,
    };
    
    // Communication to the compositor definitions
    enum GUICommunction
    {
        REQUEST_CONTEXT,
        REQUEST_CLOSE,
        CONTEXT_MOVED
    };

    // Buttons present on a regular mouse
    enum MouseButtons
    {
        Left,
        Middle,
        Right
    };

    class Window;
    class GUI
    {
    private:
        static Context* FindTargetContext(int m_x, int m_y);
    public:
        // Current Width of video mode
        static int Width;

        // Current Height of video mode
        static int Height;
    public:
        /**
         * The PID used by the compositor process
        */
        static int compositorPID;

        /**
         * The list of all contexts in this application
        */
        static List<Context*>* contextList;
        
        /**
         * System default font
        */
        static Font* defaultFont;

        /**
         * Initialize the gui for this process
        */
        static void Initialize();

        /**
         * Initialize the text rendering by loading the default font file from disk
        */
        static void SetDefaultFont(const char* filename = "B:\\fonts\\Ubuntu15.cff");
        
        /**
         * Remove all gui elements created by this process
        */
        static void CleanUp();
        
        /**
         * Process all the possible gui events
        */
        static void ProcessEvents();

        /**
         * Draw all the contexts to the screen
        */
        static void DrawGUI();

        /**
         * Create a thread for the gui
        */
        static void MakeAsync();

        /**
         * Is the current number of contexts larger than 0? 
        */
        static bool HasItems();

        /**
         * Request a context buffer for the application to draw to, this buffer is shared between the process and the compositor
         * This buffer can be used for a gui but also for raw drawing to the screen
         * 
         * returns a pointer to the context struct
         * @param width The width of the context
         * @param height The height of the context
         * @param x The position on horizontal axis
         * @param y The position on vertical axis
        */
        static Context* RequestContext(int width, int height, int x, int y);

        /**
         * Find the window associated with the control
        */
        static Window* GetControlWindow(Control* control);
    };
}

#endif