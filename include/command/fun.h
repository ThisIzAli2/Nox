/**
 ** This file is part of Novanix.
 @author Ali Mirmohammad
 @file fun.h

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
#ifndef __NOVANIX_KERNEL_FUN_H
#define __NOVANIX_KERNEL_FUN_H

#include <common/init.hpp>
#include <random/rand.h>

#define COW_STRING \
"  \\   ^__^\n" \
"       \\  (oo)\\_______\n" \
"          (__)\\       )\\/\\\n" \
"              ||----w |\n" \
"              ||     ||\n"


// Correct string macros (use double quotes)
#define NEWTON_QUOTE "If I have seen further, it is by standing on the shoulders of giants (Sir Isaac Newton)"
#define MESSAGE_OF_THE_DAY "Welcome to Nova OS - The future is bright!\n"

// Array of pointers to strings
const char* motd_arr[] = {NEWTON_QUOTE, MESSAGE_OF_THE_DAY};
const size_t motd_arr_len = sizeof(motd_arr) / sizeof(motd_arr[0]);

/**
 * Show a random message of the day to the user, just for Fun!
 */
const __always_inline char* randomMotd() {
    if (motd_arr_len == 0) return NULL;
    unsigned int index = krand() % motd_arr_len;
    return motd_arr[index];
}

#endif /* __NOVANIX_KERNEL_FUN_H*/