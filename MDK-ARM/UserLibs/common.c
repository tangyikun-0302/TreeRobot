#include "common.h"
#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
PUTCHAR_PROTOTYPE
{
	/* Place your implementation of fputc here */
	HAL_UART_Transmit(&huart6, (uint8_t *)&ch, 1, 0xFFFF);
	return ch;
}
/**
 * @brief			print logo  !!!
 * 					没啥用，好看!!!
 */
void Print_Logo_2_Com(void)
{
	INFO_PRINT("\r\n");
	INFO_PRINT("        *============*    *==========*         *===========*         *===========* \r\n");
	INFO_PRINT("         ====*  *====    /           /        /  /                  /  / \r\n");
	INFO_PRINT("            /  /        /          /         /  /                  /  / \r\n");
	INFO_PRINT("           /  /        / * ===== *          /  * ======= *        /  * ======= * \r\n");
	INFO_PRINT("          /  /        /    \\\\              /  /                  /  /  \r\n");
	INFO_PRINT("         /  /        /      \\\\            /  /                  /  /  \r\n");
	INFO_PRINT("        *===*       *=*      *=*         *============= *      * ============= * \r\n");
	INFO_PRINT("\r\n");
}
/* 
	INFO_PRINT("\r\n");
	INFO_PRINT("        *============*    *==========*         *===========*         *===========*    \r\n");
	INFO_PRINT("       /====*  *====/    /           /        /  /                  /  /      		  \r\n");
	INFO_PRINT("		   /  /  		/		   /	     /  /                  /  /          	  \r\n");
	INFO_PRINT("          /  /         / * ===== *		    /  * ======= *        /  * ======= *      \r\n");
	INFO_PRINT("         /  /         /	   \\		 	   /  /                  /  /    			  \r\n");
	INFO_PRINT("        /  /  		 /	    \\  	      /  /   			    /  /   				  \r\n");
	INFO_PRINT("       *===*        *=* 	 *=*         *============= *      *============= * 	  \r\n");
	INFO_PRINT("\r\n");
 */
