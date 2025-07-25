/**
 * @author Ali Mirmohammad
 * @file vfsmanager.cpp
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
#include <system/vfs/vfsmanager.h>
#include <system/system.h>
#include <typing.hpp>

using namespace Novanix;
using namespace Novanix::common;
using namespace Novanix::core;
using namespace Novanix::system;

VFSManager::VFSManager()
{
    this->Filesystems = new List<VirtualFileSystem*>();
}

INTEGER VFSManager::ExtractDiskNumber(const char* path, uint8_t* idSizeReturn)
{
    if(String::Contains(path, ':') && String::Contains(path, PATH_SEPERATOR_C))
    {
        INTEGER idLength = String::IndexOf(path, ':');

        char* idStr = new char[idLength];
        MemoryOperations::memcpy(idStr, path, idLength);

        INTEGER idValue = 0;

        if(isalpha(idStr[0])) //Are we using a character instead of a integer
        {
            switch (idStr[0])
            {
                case 'b':
                case 'B': //Boot partition
                    idValue = this->bootPartitionID;
                    break;      
                default:
                    delete idStr;
                    return -1;
                    break;
            }
        }
        else
            idValue = Convert::StringToInt(idStr);

        delete idStr;

        if(idSizeReturn != 0)
            *idSizeReturn = idLength;   
           
        return idValue;  
    }
    return -1;
}

VOID VFSManager::Mount(VirtualFileSystem* vfs)
{
    this->Filesystems->push_back(vfs); //Just add it to the list of known filesystems, easy.
}
VOID VFSManager::Unmount(VirtualFileSystem* vfs)
{
    this->Filesystems->Remove(vfs);
}
VOID VFSManager::UnmountByDisk(Disk* disk)
{
    for(VirtualFileSystem* vfs : *Filesystems)
        if(vfs->disk == disk)
            Unmount(vfs);
}

BOOL VFSManager::SearchBootPartition()
{
    List<Disk*> posibleDisks;
    char* pathString = "####:\\boot\\Novanix.bin";

    // First we create a collection of disks which may be the boot disk
    // Could be more then one when there are more than 1 cd drives installed

    uint8_t bootDevice = (System::mbi->boot_device & 0xFF000000) >> 24;
    BiosDriveParameters* diskInfo = System::diskManager->GetDriveInfoBios(bootDevice);
    if(diskInfo->bufLen > 0 && diskInfo->interfaceName[0] != '\0') // Valid structure
    {
        DiskType bootDiskType = HardDisk;
        if(diskInfo->bytesPerSector == 2048) // Probably a cdrom
            bootDiskType = CDROM;
        if(String::strncmp("USB", diskInfo->interfaceName, 3)) // Is a usb-device
            bootDiskType = USBDisk;
        
        for(Disk* disk : System::diskManager->allDisks)
            if(disk->type == bootDiskType) 
            {
                if(bootDiskType == HardDisk) { // Do a sector count check
                    if(divide64(disk->size, diskInfo->bytesPerSector) == diskInfo->totalSectors)
                        posibleDisks.push_back(disk);
                }
                else
                    posibleDisks.push_back(disk);
            }
    }
    else { // Include all disks in the search
        for(Disk* disk : System::diskManager->allDisks)
            posibleDisks.push_back(disk);
    }

    // Now loop though all the filesystems mounted and check if the disk can be the booted one
    // If that is the case check for the kernel file
    // At this point we can assume this is the boot disk
    for(INTEGER i = 0; i < Filesystems->size(); i++)
    {
        if(posibleDisks.IndexOf(Filesystems->GetAt(i)->disk) == -1)
            continue; // This partition is not present on the posible disks we booted from
        
        char* idStr = Convert::IntToString(i);
        INTEGER idStrLen = String::strlen(idStr);

        MemoryOperations::memcpy(pathString + (4-idStrLen), idStr, idStrLen);

        if(Filesystems->GetAt(i)->FileExists(pathString + (4-idStrLen) + 3))
        {
            this->bootPartitionID = i;
            return true;
        }
    }
    return false;
}

List<Novanix::VFSEntry>* VFSManager::DirectoryList(const char* path)
{
    uint8_t idSize = 0;
    INTEGER disk = ExtractDiskNumber(path, &idSize);
    if(disk != -1 && Filesystems->size() > disk)
        return Filesystems->GetAt(disk)->DirectoryList(path + idSize + 2); // skips the 0:\ part
    else
        return 0;
}

uint32_t VFSManager::GetFileSize(const char* path)
{
    uint8_t idSize = 0;
    INTEGER disk = ExtractDiskNumber(path, &idSize);
    if(disk != -1 && Filesystems->size() > disk)
        return Filesystems->GetAt(disk)->GetFileSize(path + idSize + 2); // skips the 0:\ part
    else
        return -1;
}

INTEGER VFSManager::ReadFile(const char* path, uint8_t* buffer, uint32_t offset, uint32_t len)
{
    uint8_t idSize = 0;
    INTEGER disk = ExtractDiskNumber(path, &idSize);

    if(disk != -1 && Filesystems->size() > disk)
        return Filesystems->GetAt(disk)->ReadFile(path + idSize + 2, buffer, offset, len);
    else
        return -1;
}

INTEGER VFSManager::WriteFile(const char* path, uint8_t* buffer, uint32_t len, BOOL create)
{
    uint8_t idSize = 0;
    INTEGER disk = ExtractDiskNumber(path, &idSize);

    if(disk != -1 && Filesystems->size() > disk)
        return Filesystems->GetAt(disk)->WriteFile(path + idSize + 2, buffer, len, create);
    else
        return -1;
}

BOOL VFSManager::FileExists(const char* path)
{
    uint8_t idSize = 0;
    INTEGER disk = ExtractDiskNumber(path, &idSize);

    if(disk != -1 && Filesystems->size() > disk)
        return Filesystems->GetAt(disk)->FileExists(path + idSize + 2);
    else
        return false;
}

BOOL VFSManager::DirectoryExists(const char* path)
{
    uint8_t idSize = 0;
    INTEGER disk = ExtractDiskNumber(path, &idSize);

    if(disk != -1 && Filesystems->size() > disk) {
        if(String::strlen(path) == idSize + 2) //Only disk part, like 0:\ ofcourse this is a directory as well
            return true;
        else
            return Filesystems->GetAt(disk)->DirectoryExists(path + idSize + 2);
    }
    else
        return false;
}

INTEGER VFSManager::CreateFile(const char* path)
{
    uint8_t idSize = 0;
    INTEGER disk = ExtractDiskNumber(path, &idSize);

    if(disk != -1 && Filesystems->size() > disk) {
        if(String::strlen(path) == idSize + 2) //Only disk part, like 0:\ ofcourse this is a directory as well
            return true;
        else
            return Filesystems->GetAt(disk)->CreateFile(path + idSize + 2);
    }
    else
        return -1;
}
INTEGER VFSManager::CreateDirectory(const char* path)
{
    uint8_t idSize = 0;
    INTEGER disk = ExtractDiskNumber(path, &idSize);

    if(disk != -1 && Filesystems->size() > disk) {
        if(String::strlen(path) == idSize + 2) //Only disk part, like 0:\ ofcourse this is a directory as well
            return true;
        else
            return Filesystems->GetAt(disk)->CreateDirectory(path + idSize + 2);
    }
    else
        return -1;
}

BOOL VFSManager::EjectDrive(const char* path)
{
    uint8_t idSize = 0;
    INTEGER disk = ExtractDiskNumber(path, &idSize);

    if(disk != -1 && Filesystems->size() > disk) {
        VirtualFileSystem* fs = Filesystems->GetAt(disk);
        return fs->disk->controller->EjectDrive(fs->disk->controllerIndex);
    }
    else
        return false;
}