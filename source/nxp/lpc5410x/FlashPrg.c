/* FlashAlgo
 * Copyright (c) 2016 Freescale Semiconductors, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "FlashOS.H"        // FlashOS Structures
#include "chip.h"
#include "iap.h"

enum {
    kSectorSize = 32768,
    kPageSize = 256,
};

const uint32_t ExtClockIn = 0;

// The highest flash sector number. Used for erasing the entire chip.
uint32_t g_topSector = 15; // 16 * 32kB = 512kB

//! Convert a memory address to a flash sector number.
static inline uint32_t address_to_sector(uint32_t adr)
{
    return adr / kSectorSize;
}

uint32_t Init(uint32_t adr, uint32_t clk, uint32_t fnc)
{
    SystemCoreClockUpdate();

    // Check if this is a 256kB device.
    if ((LPC_SYSCON->DEVICE_ID0 & 0x00f00000) == 0x00400000)
    {
        g_topSector = 7; // 8 * 32kB = 256kB
    }

// 	uint32_t part_id = Chip_IAP_ReadPID();
// 	DEBUGOUT("Part ID is: %x\r\n", part_id);

    return 0;
}


/*
 *  De-Initialize Flash Programming Functions
 *    Parameter:      fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */

uint32_t UnInit(uint32_t fnc)
{
  return 0;
}


/*  Blank Check Block in Flash Memory
 *    Parameter:      adr:  Block Start Address
 *                    sz:   Block Size (in bytes)
 *                    pat:  Block Pattern
 *    Return Value:   0 - OK,  1 - Failed
 */

uint32_t BlankCheck(uint32_t adr, uint32_t sz, uint8_t pat)
{
//     return (flash_verify_erase(&g_flash, adr, sz, kFlashMargin_Normal) != kStatus_Success);
    return 0;
}

/*
 *  Verify Flash Contents
 *    Parameter:      adr:  Start Address
 *                    sz:   Size (in bytes)
 *                    buf:  Data
 *    Return Value:   (adr+sz) - OK, Failed Address
 */
uint32_t Verify(uint32_t adr, uint32_t sz, uint32_t *buf)
{
//     uint32_t failedAddress;
//     status_t status = flash_verify_program(&g_flash, adr, sz,
//                               (const uint8_t *)buf, kFlashMargin_Normal,
//                               &failedAddress, NULL);
//
//     if (status == kStatus_Success)
//     {
//         // Finished without Errors
//         return (adr+sz);
//     }
//     else
//     {
//         return failedAddress;
//     }
    return 0;
}

/*
 *  Erase complete Flash Memory
 *    Return Value:   0 - OK,  1 - Failed
 */

uint32_t EraseChip(void)
{
	// Prepare to erase the specified sector.
	uint8_t ret_code = Chip_IAP_PreSectorForReadWrite(0, g_topSector);
	if (ret_code != IAP_CMD_SUCCESS)
	{
		return ret_code;
	}

	// Erase the sector.
	ret_code = Chip_IAP_EraseSector(0, g_topSector);
	if (ret_code != IAP_CMD_SUCCESS)
	{
		return ret_code;
	}

    return ret_code;
}


/*
 *  Erase Sector in Flash Memory
 *    Parameter:      adr:  Sector Address
 *    Return Value:   0 - OK,  1 - Failed
 */
uint32_t EraseSector(uint32_t adr)
{
    uint32_t sector = address_to_sector(adr);

	// Prepare to erase the specified sector.
	uint8_t ret_code = Chip_IAP_PreSectorForReadWrite(sector, sector);
	if (ret_code != IAP_CMD_SUCCESS)
	{
		return ret_code;
	}

	// Erase the sector.
	ret_code = Chip_IAP_EraseSector(sector, sector);
	if (ret_code != IAP_CMD_SUCCESS)
	{
		return ret_code;
	}

	// Verify the sector was erased.
	return Chip_IAP_BlankCheckSector(sector, sector);
}

/*
 *  Program Page in Flash Memory
 *    Parameter:      adr:  Page Start Address
 *                    sz:   Page Size
 *                    buf:  Page Data
 *    Return Value:   0 - OK,  1 - Failed
 */
uint32_t ProgramPage(uint32_t adr, uint32_t sz, uint32_t *buf)
{
    uint32_t sector = address_to_sector(adr);

	// Prepare to write the sector.
	uint8_t ret_code = Chip_IAP_PreSectorForReadWrite(sector, sector);
	if (ret_code != IAP_CMD_SUCCESS)
	{
		return ret_code;
	}

	// Write to the sector.
	ret_code = Chip_IAP_CopyRamToFlash(adr, buf, sz);
	if (ret_code != IAP_CMD_SUCCESS)
	{
		return ret_code;
	}

    return ret_code;
}

