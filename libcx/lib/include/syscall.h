/**
 * @author Ali Mirmohammad
 * @file syscall.h
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
#ifndef __NovanixLIB__SYSCALL_H
#define __NovanixLIB__SYSCALL_H

namespace Novanix
{
    #define SYSCALL_RET_SUCCES 1
    #define SYSCALL_RET_ERROR 0
    #define PROC_ARG_LEN_MAX 100

    enum Systemcalls {
        SYSCALL_EXIT = 0, // Tells kernel that procces is done and can be removed

        /////////////
        // Logging
        /////////////
    
        SYSCALL_LOG,
        SYSCALL_PRINT,

        /////////////
        // VFS
        /////////////

        SYSCALL_FILE_EXISTS,
        SYSCALL_DIR_EXISTS,
        SYSCALL_GET_FILESIZE,
        SYSCALL_READ_FILE,
        SYSCALL_WRITE_FILE,
        SYSCALL_CREATE_FILE,
        SYSCALL_CREATE_DIRECTORY,
        SYSCALL_EJECT_DISK,

        //////////////
        // GUI
        //////////////
        
        SYSCALL_GUI_GETLFB,
        SYSCALL_GET_SCREEN_PROPERTIES,

        //////////////
        // Memory
        //////////////

        SYSCALL_GET_HEAP_START,
        SYSCALL_GET_HEAP_END,
        SYSCALL_SET_HEAP_SIZE,
        SYSCALL_CREATE_SHARED_MEM,
        SYSCALL_REMOVE_SHARED_MEM,
        SYSCALL_MAP_SYSINFO,

        //////////////
        // Scheduler
        //////////////

        SYSCALL_RUN_PROC,
        SYSCALL_SLEEP_MS,
        SYSCALL_START_THREAD,
        SYSCALL_YIELD,
        SYSCALL_PROC_EXIST,
        SYSCALL_UNBLOCK,
        SYSCALL_SET_SCHEDULER,
        SYSCALL_GET_ARGUMENTS,

        //////////////
        // IPC
        //////////////

        SYSCALL_IPC_SEND,
        SYSCALL_IPC_RECEIVE,
        SYSCALL_IPC_AVAILABLE,

        //////////////
        // Clock
        //////////////

        SYSCALL_GET_TICKS,
        SYSCALL_GET_DATETIME,

        //////////////
        // Power
        //////////////

        SYSCALL_SHUTDOWN,
        SYSCALL_REBOOT,

        //////////////
        // STDIO
        //////////////

        SYSCALL_READ_STDIO,
        SYSCALL_WRITE_STDIO,
        SYSCALL_REDIRECT_STDIO,
        SYSCALL_STDIO_AVAILABLE,

        //////////////
        // Listings
        //////////////

        SYSCALL_BEGIN_LISTING,
        SYSCALL_LISTING_ENTRY,
        SYSCALL_END_LISTING,
        SYSCALL_GET_SYSINFO_VALUE,

        //////////////
        // Other
        //////////////
        SYSCALL_SET_Novanix_LIB = 0xFFFF,
    };

    int DoSyscall(unsigned int intNum, unsigned int arg1 = 0, unsigned int arg2 = 0, unsigned int arg3 = 0, unsigned int arg4 = 0, unsigned int arg5 = 0);
}

#endif