#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <string>
#include <Windows.h>
#include <locale>
#include <cstring>
#include <cwchar>
#include <stdio.h>
#include <inttypes.h>
#include <iostream>


typedef unsigned char BYTE;
typedef unsigned short WORD;

#pragma pack(push, 1)

typedef struct tagFAT_BOOTSECTOR32
{
    // Common fields.
    BYTE sJmpBoot[3];
    BYTE sOEMName[8];
    WORD wBytsPerSec;
    BYTE bSecPerClus;
    WORD wRsvdSecCnt;
    BYTE b2;
    WORD wRootEntCnt;
    WORD wTotSec16;
    BYTE bMedia;
    WORD wFATSz16;
    WORD wSecPerTrk;
    WORD wNumHeads;
    DWORD dHiddSec;
    DWORD dTotSec32;
    // Fat 32/16 only
    DWORD dFATSz32;
    WORD wExtFlags;
    WORD wFSVer;
    DWORD dRootClus;
    WORD wFSInfo;
    WORD wBkBootSec;
    BYTE Reserved[12];
    BYTE bDrvNum;
    BYTE Reserved1;
    BYTE bBootSig;
    DWORD dBS_VolID;
    BYTE sVolLab[11];
    BYTE sBS_FilSysType[8];

} FAT_BOOTSECTOR32;

typedef struct {
    DWORD dLeadSig;         // 0x41615252
    BYTE sReserved1[480];   // zeros
    DWORD dStrucSig;        // 0x61417272
    DWORD dFree_Count;      // 0xFFFFFFFF
    DWORD dNxt_Free;        // 0xFFFFFFFF
    BYTE sReserved2[12];    // zeros
    DWORD dTrailSig;     // 0xAA550000
} FAT_FSINFO;

#pragma pack(pop)

class Drive {

private:
    HANDLE device_descriptor;
    FAT_BOOTSECTOR32* FAT32_boot_sector;
    FAT_FSINFO* FAT32_fsinfo_sector;
    PARTITION_INFORMATION piDrive;
    DWORD fat_size = 0;
    DWORD* FAT32_fat_sector;
    DWORD bytes_per_sector = 0;
    DWORD sector_per_cluster = 0;
    DWORD total_sectors = 0;
    DWORD reserved_sector_count = 0;
    BOOL bGPTMode = FALSE;


    DWORD get_volume_id()
    {
        SYSTEMTIME s;
        DWORD d;
        WORD lo, hi, tmp;

        GetLocalTime(&s);

        lo = s.wDay + (s.wMonth << 8);
        tmp = (s.wMilliseconds / 10) + (s.wSecond << 8);
        lo += tmp;

        hi = s.wMinute + (s.wHour << 8);
        hi += s.wYear;
        d = lo + (hi << 16);
        return(d);
    }

    void die(const char error[])
    {
        LPVOID lpMsgBuf = (LPVOID)1;
        DWORD dw = GetLastError();

        if (dw)
        {
            FormatMessage(
                FORMAT_MESSAGE_ALLOCATE_BUFFER |
                FORMAT_MESSAGE_FROM_SYSTEM |
                FORMAT_MESSAGE_IGNORE_INSERTS,
                NULL,
                dw,
                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                (LPTSTR)&lpMsgBuf,
                0, NULL);
            fprintf(stderr, "%s\nGetLastError()=%d: %s\n", error, dw, lpMsgBuf);
        }
        else
        {
            fprintf(stderr, "%s\n", error);
        }
        LocalFree(lpMsgBuf);
        exit(dw);
    }

    DWORD get_sizeof_fat_sectors() {
        ULONGLONG   Numerator, Denominator;
        ULONGLONG   FatElementSize = 4;
        ULONGLONG   FatSz;

        Numerator = FatElementSize * (total_sectors - 64);
        Denominator = (sector_per_cluster * bytes_per_sector) + (FatElementSize * 2);
        FatSz = Numerator / Denominator;
        FatSz += 1;

        return((DWORD)FatSz);
    }

    void write_sect(DWORD Sector, void* Data, DWORD NumSects)
    {
        DWORD dwWritten;
        BOOL ret;

        LONGLONG Offset;
        LONG HiOffset;

        Offset = Sector * bytes_per_sector;
        HiOffset = (LONG)(Offset >> 32);
        SetFilePointer(device_descriptor, (LONG)Offset, &HiOffset, FILE_BEGIN);

        ret = WriteFile(device_descriptor, Data, NumSects * bytes_per_sector, &dwWritten, NULL);

        if (!ret)
            die("Failed to write");
    }

    void zero_sectors()
    {
        DWORD NumSects = (reserved_sector_count + (2 * fat_size) + sector_per_cluster);
        BYTE* pZeroSect;
        DWORD BurstSize = 128;
        DWORD WriteSize;
        BOOL ret;
        DWORD dwWritten;

        pZeroSect = (BYTE*)VirtualAlloc(NULL, bytes_per_sector * BurstSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

        SetFilePointer(device_descriptor, NULL, NULL, FILE_BEGIN);

        while (NumSects)
        {
            if (NumSects > BurstSize)
                WriteSize = BurstSize;
            else
                WriteSize = NumSects;

            ret = WriteFile(device_descriptor, pZeroSect, WriteSize * bytes_per_sector, &dwWritten, NULL);
            if (!ret)
                die("Failed to write");

            NumSects -= WriteSize;
        }
    }

    BYTE get_sectors_per_cluster()
    {
        BYTE ret = 1;
        LONGLONG DiskSizeMB = piDrive.PartitionLength.QuadPart / (1024 * 1024);
        DWORD ClusterSize = 4096;

        if (DiskSizeMB > 512) {
            ClusterSize = 4096;
        }
        if (DiskSizeMB > 8192) {
            ClusterSize = 8192;
        }
        if (DiskSizeMB > 16384) {
            ClusterSize = 16384;
        }
        if (DiskSizeMB > 32768) {
            ClusterSize = 32768;
        }
        ret = ClusterSize / bytes_per_sector;

        return(ret);
    }

    void write_system_sectors() {
        DWORD SystemAreaSize = (reserved_sector_count + (2 * fat_size) + sector_per_cluster);

        printf("Formatting drive:...\n");
        printf("Clearing out %d sectors for system...\n", SystemAreaSize);
        zero_sectors();
        printf("Initialising system sectors...\n");
        write_sect(0, FAT32_boot_sector, 1);
        write_sect(1, FAT32_fsinfo_sector, 1);
        write_sect(6, FAT32_boot_sector, 1);
        write_sect(7, FAT32_fsinfo_sector, 1);

        for (DWORD i = 0; i < 2; i++) {
            int SectorStart = reserved_sector_count + (i * fat_size);
            write_sect(SectorStart, FAT32_fat_sector, 1);
        }

    }

    void print_system_info() {
        printf("Size : %gGB %u sectors\n", (double)(piDrive.PartitionLength.QuadPart / (1024 * 1024 * 1024)), total_sectors);
        printf("%d Bytes Per Sector, Cluster size %d bytes\n", bytes_per_sector, sector_per_cluster * bytes_per_sector);
        printf("%d Reserved Sectors, %d Sectors per FAT, %d fats\n", reserved_sector_count, fat_size, 2);
        printf("%d Free Clusters\n", FAT32_fsinfo_sector->dFree_Count);
        printf("After this we couldnt do anything to retrieve your data.\n");
    }

    void fill_fsinfo(DWORD UserAreaSize) {
        FAT32_fsinfo_sector->dLeadSig = 0x41615252;
        FAT32_fsinfo_sector->dStrucSig = 0x61417272;
        FAT32_fsinfo_sector->dFree_Count = (DWORD)-1;
        FAT32_fsinfo_sector->dNxt_Free = (DWORD)-1;
        FAT32_fsinfo_sector->dTrailSig = 0xaa550000;
        FAT32_fsinfo_sector->dFree_Count = (UserAreaSize / sector_per_cluster) - 1;
        FAT32_fsinfo_sector->dNxt_Free = 3;
    }

    void fill_boot(DISK_GEOMETRY dgDrive) {
        BYTE VolId[11] = "star dust ";
        strcpy((char*)FAT32_boot_sector->sOEMName, "MSWIN4.1");
        FAT32_boot_sector->wBytsPerSec = (WORD)bytes_per_sector;
        FAT32_boot_sector->sJmpBoot[0] = 0xEB;
        FAT32_boot_sector->sJmpBoot[1] = 0x58;
        FAT32_boot_sector->sJmpBoot[2] = 0x90;
        FAT32_boot_sector->bSecPerClus = (BYTE)sector_per_cluster;
        FAT32_boot_sector->b2 = (BYTE)2;
        FAT32_boot_sector->wRootEntCnt = 0;
        FAT32_boot_sector->wTotSec16 = 0;
        FAT32_boot_sector->bMedia = 0xF8;
        FAT32_boot_sector->wFATSz16 = 0;
        FAT32_boot_sector->wSecPerTrk = (WORD)dgDrive.SectorsPerTrack;
        FAT32_boot_sector->wNumHeads = (WORD)dgDrive.TracksPerCylinder;
        FAT32_boot_sector->dHiddSec = (DWORD)piDrive.HiddenSectors;
        FAT32_boot_sector->dTotSec32 = total_sectors;
        FAT32_boot_sector->wRsvdSecCnt = (WORD)reserved_sector_count;
        FAT32_boot_sector->dFATSz32 = fat_size;
        FAT32_boot_sector->wExtFlags = 0;
        FAT32_boot_sector->wFSVer = 0;
        FAT32_boot_sector->dRootClus = 2;
        FAT32_boot_sector->wFSInfo = 1;
        FAT32_boot_sector->wBkBootSec = (WORD)6;
        FAT32_boot_sector->bDrvNum = 0x80;
        FAT32_boot_sector->Reserved1 = 0;
        FAT32_boot_sector->bBootSig = 0x29;
        FAT32_boot_sector->dBS_VolID = get_volume_id();
        memcpy(FAT32_boot_sector->sVolLab, VolId, 10);
        memcpy(FAT32_boot_sector->sBS_FilSysType, "FAT32   ", 8);
        ((BYTE*)FAT32_boot_sector)[510] = 0x55;
        ((BYTE*)FAT32_boot_sector)[511] = 0xaa;
        if (bytes_per_sector != 512)
        {
            ((BYTE*)FAT32_boot_sector)[bytes_per_sector - 2] = 0x55;
            ((BYTE*)FAT32_boot_sector)[bytes_per_sector - 1] = 0xaa;
        }
    }

    void fill_fat() {
        FAT32_fat_sector[0] = 0x0ffffff8;
        FAT32_fat_sector[1] = 0x0fffffff;
        FAT32_fat_sector[2] = 0x0fffffff;
    }

    LPCSTR ConvertWideCharToLPCSTR(const wchar_t* wideString) {
        if (!wideString) return NULL;

        int bufferSize = WideCharToMultiByte(CP_ACP, 0, wideString, -1, NULL, 0, NULL, NULL);
        if (bufferSize == 0) {
            printf("Error in conversion: %lu\n", GetLastError());
            return NULL;
        }
        char* ansiString = (char*)malloc(bufferSize);
        if (!ansiString) {
            printf("Memory allocation failed.\n");
            return NULL;
        }
        if (WideCharToMultiByte(CP_ACP, 0, wideString, -1, ansiString, bufferSize, NULL, NULL) == 0) {
            printf("Conversion failed: %lu\n", GetLastError());
            free(ansiString);
            return NULL;
        }
        return ansiString;
    }

    bool fill_system_info(char volume) {
        wchar_t DriveDevicePath[] = L"\\\\.\\D:";

        DriveDevicePath[4] = (wchar_t)volume;
        device_descriptor = CreateFile(ConvertWideCharToLPCSTR(DriveDevicePath), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_NO_BUFFERING, NULL);
        DISK_GEOMETRY dgDrive;
        if (device_descriptor == INVALID_HANDLE_VALUE) {
            die("Failed to open drive\n");
        }

        if (!DeviceIoControl((HANDLE)device_descriptor, FSCTL_ALLOW_EXTENDED_DASD_IO, NULL, 0, NULL, 0, NULL, NULL)) {
            printf("Failed to allow extended DASD on drive");
        }

        if (!DeviceIoControl(device_descriptor, FSCTL_LOCK_VOLUME, NULL, 0, NULL, 0, NULL, NULL)) {
            die("Failed to lock drive");
        }

        if (!DeviceIoControl(device_descriptor, IOCTL_DISK_GET_DRIVE_GEOMETRY, NULL, 0, &dgDrive, sizeof(dgDrive), NULL, NULL)) {
            die("Failed to get drive geometry");
        }

        bytes_per_sector = dgDrive.BytesPerSector;
        if (!DeviceIoControl(device_descriptor, IOCTL_DISK_GET_PARTITION_INFO, NULL, 0, &piDrive, sizeof(piDrive), NULL, NULL)) {
            PARTITION_INFORMATION_EX xpiDrive;

            if (!DeviceIoControl(device_descriptor, IOCTL_DISK_GET_PARTITION_INFO_EX, NULL, 0, &xpiDrive, sizeof(xpiDrive), NULL, NULL)) {
                die("Failed to get partition info");
            }
            memset(&piDrive, 0, sizeof(piDrive));
            piDrive.StartingOffset.QuadPart = xpiDrive.StartingOffset.QuadPart;
            piDrive.PartitionLength.QuadPart = xpiDrive.PartitionLength.QuadPart;
            piDrive.HiddenSectors = (DWORD)(xpiDrive.StartingOffset.QuadPart / bytes_per_sector);
            bGPTMode = (xpiDrive.PartitionStyle == PARTITION_STYLE_MBR) ? 0 : 1;
        }
        ULONGLONG qtotal_sectors = piDrive.PartitionLength.QuadPart / bytes_per_sector;

        if (qtotal_sectors < 65536) {
            die("This drive is too small for FAT32\n");
        }

        if (qtotal_sectors >= 0xffffffff) {
            die("This drive is too big for FAT32\n");
        }

        FAT32_boot_sector = (FAT_BOOTSECTOR32*)VirtualAlloc(NULL, bytes_per_sector, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
        FAT32_fsinfo_sector = (FAT_FSINFO*)VirtualAlloc(NULL, bytes_per_sector, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
        FAT32_fat_sector = (DWORD*)VirtualAlloc(NULL, bytes_per_sector, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

        if (!FAT32_boot_sector || !FAT32_fsinfo_sector || !FAT32_fat_sector) {
            die("Failed to allocate memory");
        }

        sector_per_cluster = get_sectors_per_cluster();

        total_sectors = (DWORD)qtotal_sectors;

        fat_size = get_sizeof_fat_sectors();
        reserved_sector_count = 64 - (32 + 2 * fat_size) % 32;

        fill_boot(dgDrive);

        fill_fat();

        DWORD UserAreaSize = total_sectors - reserved_sector_count - (2 * fat_size);

        fill_fsinfo(UserAreaSize);

        return true;
    }

    void release_drive() {
        SET_PARTITION_INFORMATION spiDrive;
        if (!bGPTMode) {
            spiDrive.PartitionType = 0x0c; // FAT32 LBA.

            if (!DeviceIoControl(device_descriptor, IOCTL_DISK_SET_PARTITION_INFO, &spiDrive, sizeof(spiDrive), NULL, 0, NULL, NULL)) {
                if (piDrive.HiddenSectors) {
                    die("Failed to set parition info");
                }
            }
        }

        if (!DeviceIoControl(device_descriptor, FSCTL_DISMOUNT_VOLUME, NULL, 0, NULL, 0, NULL, NULL)) {
            die("Failed to dismount device");
        }

        if (!DeviceIoControl(device_descriptor, FSCTL_UNLOCK_VOLUME, NULL, 0, NULL, 0, NULL, NULL)) {
            die("Failed to unlock device");
        }
        CloseHandle(device_descriptor);

    }

    bool changeLabel(char driveLetter, std::string newName) {
        std::wstring drivePath = L"\\\\.\\";
        drivePath += driveLetter;
        drivePath += L":\\";

        std::wstring wideNewName(newName.begin(), newName.end());
        int result = SetVolumeLabelW(drivePath.c_str(), wideNewName.c_str());
        if (!result) {
            die("Volume label error.");
        }
        return true;
    }

public:
    bool format_volume(char volume, std::string newName) {
        fill_system_info(volume);
        print_system_info();
        system("pause");
        write_system_sectors();
        release_drive();
        changeLabel(volume, newName);
        printf("Successsfull formatting.\n");
        return(TRUE);
    }
};

