/**
 * @author Ali Mirmohammad
 * @file rect.h
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
#ifndef __Novanix__GUI__RECT_H
#define __Novanix__GUI__RECT_H

#include <types.h>
#include <list.h>

namespace Novanix
{
    /**
     * A class that describes a rectangular shape
    */
    class Rectangle
    {
    public:
        /**
         * The width of this rectangle
        */
        int width;
        /**
         * The height of this rectangle
        */
        int height;
        /**
         * The x coördinate of this rectangle
        */
        int x;
        /**
         * The y coördinate of this rectangle
        */
        int y;

        /**
         * Create a new instance of the Rectangle Class
        */
        Rectangle(int w, int h, int x = 0, int y = 0);
        
        /**
         * Create a new instance of the Rectangle Class
        */
        Rectangle();

        /**
         * Get the area of this rectangle
         * Basically just width * height
         */
        int Area();

        /**
         * Get the intersection rectangle between this one and the target 
         * Results holds the intersected rect
         * Returns true for a intersection
        */
        bool Intersect(Rectangle other, Rectangle* result);

        /**
         * Does this rect contain the given point?
        */
        bool Contains(int x, int y);

        /**
         * Explode this rect into a list of contiguous rects
        */
        List<Rectangle>* Split(Rectangle cuttingRect, List<Rectangle>* output = 0);

        /**
         * Insert this rectangle into the clip list, splitting all existing rectangles against it to prevent overlap 
        */
        void PushToClipList(List<Rectangle>* targetList);

        bool operator==(const Rectangle& right)
        {
            return (this->width == right.width && this->height == right.height && this->x == right.x && this->y == right.y);
        }

        // Return a rectangle of size 0, like Rectangle(0, 0, 0, 0)
        static Rectangle Zero();
    };
}

#endif