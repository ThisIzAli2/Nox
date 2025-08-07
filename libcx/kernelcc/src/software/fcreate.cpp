/**
 * @author Ali Mirmohammad
 * @file fcreate.cpp
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


#include <software/fcreate.h>
#include <software/help/ops.h>
#include <fs/fs.h>
#include <global.h>
#include <software/edit.h>

using namespace Novanix::common;
using namespace Novanix::system;

static uint32_t seed = 1;



/**
 * @brief A function that creates a file for the system
 */
file_t __create_file(char* name, char* data, INTEGER size,bool access_req) {
    file_t file;
    // i_file = 0;

    if (size > 0) {
        file.name = name;
        file.data = data;
        file.size = size;

        files[i_file].name = file.name;
        files[i_file].data = file.data;
        files[i_file].size = file.size;
        files[i_file].dir = current_directory;
        if (access_req){
            files[i_file].access = true;
        }

        printk(VGA_COLOR_WHITE, "File created!", 1);
        printk(VGA_COLOR_WHITE, "File name: ", 0);
        printk(VGA_COLOR_WHITE, files[i_file].name, 1);
        printk(VGA_COLOR_WHITE, "File size: ", 0);
        printk(VGA_COLOR_WHITE, stringify(files[i_file].size), 1);
        printk(VGA_COLOR_WHITE, "In the directory: ", 0);
        printk(VGA_COLOR_WHITE, files[i_file].dir, 1);

        i_file++;
        file_c++;
    }

    return file;
}

static inline uint32_t rand_next() {
    seed = (1103515245 * seed + 12345) & 0x7fffffff;
    return seed;
}

// Always generate exactly 4-character random string
char* generate_random_string() {
    static char buffer[5]; // 4 chars + null terminator
    static const char charset[] =
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789";
    
    uint32_t charset_size = sizeof(charset) - 1;

    for (uint32_t i = 0; i < 4; i++) {
        buffer[i] = charset[rand_next() % charset_size];
    }

    buffer[4] = '\0';
    return buffer;
}

/**
 * @brief a function that creates a file, gives the data as an input.
 */
VOID create_file_function(char* data,char* name){
    file_t file = __create_file(name,data,String::strlen(data)+1,false);
    files[i_file++].data = file.data;
}

file_t __create_empty_file(char* name){
    file_t file;

    file.name = name;
    file.data = "";
    file.size = DEFAULT_FILE_SIZE; 
    return file;
}


/**
 * @brief This function is used for the "touch" command.
 */
VOID create_empty_file(char* name){
    file_t file = __create_empty_file(name);
    files[i_file].name = file.name;
    files[i_file].data = file.data;
    files[i_file].size = file.size;
    
}