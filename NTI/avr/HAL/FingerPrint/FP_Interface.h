#ifndef FINGER_PRINT_H
#define FINGER_PRINT_H

#include "../../LIB/Datatypes.h"

uint8 FP_Collect_Image(); // done
uint8 FP_Upload_Image();
uint8 FP_Download_Image();
uint8 FP_Gen_Char_File();
uint8 FP_Gen_Template(uint8 char_file_1, uint8 char_file_2);
uint8 FP_Upload_Template(uint8 *temp);
uint8 FP_Download_Template(uint8 *temp);
uint8 FP_Store_Template(uint8 *temp);
uint8 FP_Read_Template(uint8 *temp);
uint8 FP_Delete_Template(uint8 temp_id);
uint8 FP_Empty_Library(void);
uint8 FP_Match_Templates(uint8 tmp1_id, uint8 tmp2_id);
uint8 FP_Search_Templates(uint8 tmp1_id);

#endif