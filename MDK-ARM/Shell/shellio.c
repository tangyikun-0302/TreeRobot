#include "shellio.h"

/**
 * @brief 			比较两个字符串是否相同，相同则返回1 ，不同则返回 0
 * @param[in]	    str1* 字符 1
 * @param[in]       str2* 字符 2
 * @retval          1 same; 0 no
 */
unsigned char Str_Compare(char * str1, char * str2, unsigned short int len)
{
	for(unsigned short int i = 0; i < len; i++)
	{
		if(str1[i] != str2[i])
		{
			return 0;
		}
	}
	return 1;
}
