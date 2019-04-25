/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    Adafruit_LCD_1_8_SD_Joystick/FatFs/App/app_fatfs.c
  * @author  MCD Application Team
  * @brief   FatFs_uSD_Standalone application file
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics. 
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the 
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "app_fatfs.h"
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define FATFS_MKFS_ALLOWED 1
#define BITMAP_BUFFER_SIZE 512//1 * 512 /* You can adapt this size depending on the amount of RAM available */
#define BITMAP_HEADER_SIZE sizeof(BmpHeader) /* Bitmap specificity */
#define MIN(a,b) (((a)<(b))?(a):(b))
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
FATFS SDFatFs;    /* File system object for SD logical drive */
FIL SDFile;       /* File  object for SD */
char SDPath[4];   /* SD logical drive path */
/* USER CODE BEGIN PV */
uint32_t counter = 0;
uint8_t aBuffer[BITMAP_HEADER_SIZE + BITMAP_BUFFER_SIZE];
FILINFO MyFileInfo;
DIR MyDirectory;
FIL MyFile;
UINT BytesWritten, BytesRead;
extern char* pDirectoryFiles[MAX_BMP_FILES];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/**
  * @brief  FatFs initialization
  * @param  None
  * @retval Initialization result 
  */
int32_t MX_FATFS_Init(void) 
{
  /*## FatFS: Link the disk I/O driver(s)  ###########################*/
  if (FATFS_LinkDriver(&SD_Driver, SDPath) != 0)
  /* USER CODE BEGIN FATFS_Init */
  {
    return APP_ERROR;
  }
  else
  {
	/* Initialize the SD mounted on adafruit 1.8" TFT shield */
    if(BSP_SD_Init() != MSD_OK)
    {
      TFT_DisplayErrorMessage(BSP_SD_INIT_FAILED);
	  return APP_ERROR;
    }  
    
    /* Check the mounted device */
    if(f_mount(&SDFatFs, (TCHAR const*)"/", 0) != FR_OK)
    {
      TFT_DisplayErrorMessage(FATFS_NOT_MOUNTED);
	  return APP_ERROR;
    }  
    else
    {
      /* Initialize the Directory Files pointers (heap) */
      for (counter = 0; counter < MAX_BMP_FILES; counter++)
      {
        pDirectoryFiles[counter] = malloc(11); 
      }
    }
    return APP_OK;
  }
  /* USER CODE END FATFS_Init */
}

/**
  * @brief  FatFs application main process
  * @param  None
  * @retval Process result 
  */
int32_t MX_FATFS_Process(void)
{
  /* USER CODE BEGIN FATFS_Process */
  return APP_OK;
  /* USER CODE END FATFS_Process */
}  

/**
  * @brief  Gets Time from RTC (generated when FS_NORTC==0; see ff.c)
  * @param  None
  * @retval Time in DWORD
  */
DWORD get_fattime(void)
{
  /* USER CODE BEGIN get_fattime */
  return 0;
  /* USER CODE END get_fattime */  
}

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN Application */
/**
  * @brief  Open a file and display it on lcd
  * @param  DirName: the Directory name to open
  * @param  FileName: the file name to open
  * @param  BufferAddress: A pointer to a buffer to copy the file to
  * @param  FileLen: the File length
  * @retval err: Error status (0=> success, 1=> fail)
  */
uint32_t Storage_OpenReadFile(uint8_t Xpoz, uint16_t Ypoz, const char *BmpName)
{
  uint32_t size = 0;
  FIL bmpfile;
  uint32_t nbline;
  BmpHeader* pbmpheader = (BmpHeader*)aBuffer;
  
  /* Close a bmp file */
  f_open(&bmpfile, BmpName, FA_READ);
  
  /* Read the constant part of the header from the file and store it at the top of aBuffer*/
  f_read(&bmpfile, &aBuffer, BITMAP_HEADER_SIZE, &BytesRead);
  
  /* Get the size of the data stored inside the file */
  size = pbmpheader->fsize - pbmpheader->offset;
  
  /* Start reading at the top of the file */
  f_lseek(&bmpfile, 0);
  
  /* Read the entire header from the file and store it at the top of aBuffer */
  f_read(&bmpfile, &aBuffer, pbmpheader->offset, &BytesRead);
  
  /* Compute the number of entire lines which can be stored inside the buffer */
  nbline = (BITMAP_BUFFER_SIZE - pbmpheader->offset + BITMAP_HEADER_SIZE)/(pbmpheader->w * 2);
  
  /* As long as the entire bitmap file as not been displayed */
  do
  {
    uint32_t nbbytetoread;
    
    /* Get the number of bytes which can be stored inside the buffer */
    nbbytetoread = MIN(size,nbline*pbmpheader->w*2);
  
    /* Adapt the total size of the bitmap, stored inside the header, to this chunck */
    pbmpheader->fsize = pbmpheader->offset + nbbytetoread;
  
    /* Adapt the number of line, stored inside the header, to this chunck */
    pbmpheader->h = nbbytetoread/(pbmpheader->w*2);
    
    /* Start reading at the end of the file */
    f_lseek(&bmpfile, pbmpheader->offset + size - nbbytetoread);
    
    /* Store this chunck (or the entire part if possible) of the file inside a buffer */
    f_read(&bmpfile, aBuffer + pbmpheader->offset, nbbytetoread, &BytesRead);
  
    /* Draw the bitmap */
    BSP_LCD_DrawBitmap(Xpoz, Ypoz, aBuffer);    
      
    /* Update the remaining number of bytes to read */
    size -= nbbytetoread;
    
    /* Change the display position of the next bitmap */
    Ypoz += nbline;
      
  }while (size > 0);
  
  /* Close the bmp file */
  f_close(&bmpfile);
  
  return 0;
}      

/**
  * @brief  Copy file BmpName1 to BmpName2 
  * @param  BmpName1: the source file name
  * @param  BmpName2: the destination file name
  * @retval err: Error status (0=> success, 1=> fail)
  */
uint32_t Storage_CopyFile(const char* BmpName1, const char* BmpName2)
{
  uint32_t index = 0;
  FIL file1, file2;
  
  /* Open an Existent BMP file system */
  f_open(&file1, BmpName1, FA_READ);
  /* Create a new BMP file system */
  f_open(&file2, BmpName2, FA_CREATE_ALWAYS | FA_WRITE);
  
  do 
  {
    f_read(&file1, aBuffer, _MAX_SS, &BytesRead);
    f_write(&file2, aBuffer, _MAX_SS, &BytesWritten);  
    index+= _MAX_SS;
    
  } while(index < f_size(&file1));
  
  f_close(&file1);
  f_close(&file2);
  
  return 1;
}

/**
  * @brief  Opens a file and copies its content to a buffer.
  * @param  DirName: the Directory name to open
  * @param  FileName: the file name to open
  * @param  BufferAddress: A pointer to a buffer to copy the file to
  * @param  FileLen: File length
  * @retval err: Error status (0=> success, 1=> fail)
  */
uint32_t Storage_CheckBitmapFile(const char* BmpName, uint32_t *FileLen)
{
  uint32_t err = 0;
 
  if(f_open(&MyFile, BmpName, FA_READ) != FR_OK)
  {
    err = 1;
  }
  f_close(&MyFile);  
  return err;
}

/**
  * @brief  List up to 25 file on the root directory with extension .BMP
  * @param  DirName: Directory name
  * @param  Files: Buffer to contain read files
  * @retval The number of the found files
  */
uint32_t Storage_GetDirectoryBitmapFiles(const char* DirName, char* Files[])
{
  FRESULT res;
  uint32_t index = 0;

  /* Start to search for wave files */
  res = f_findfirst(&MyDirectory, &MyFileInfo, DirName, "*.bmp");
  
  /* Repeat while an item is found */
  while (MyFileInfo.fname[0])
  {
    if(res == FR_OK)
    {
      if(index < MAX_BMP_FILES)
      {
        sprintf (Files[index++], "%s", MyFileInfo.fname);
      }
      /* Search for next item */
      res = f_findnext(&MyDirectory, &MyFileInfo);
    }
    else
    {
      index = 0;
      break;
    }
  }
  
  f_closedir(&MyDirectory);
  
  return index;
}

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared
  * @param  BufferLength: buffer's length
  * @retval  0: pBuffer1 identical to pBuffer2
  *          1: pBuffer1 differs from pBuffer2
  */
uint8_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
  uint8_t ret = 1;
  
  while (BufferLength--)
  {
    if(*pBuffer1 != *pBuffer2)
    {
      ret = 0;
    }
    
    pBuffer1++;
    pBuffer2++;
  }
  
  return ret;
}

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
