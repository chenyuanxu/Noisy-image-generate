/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */

#include "display_demo.h"
#include "display_ctrl/display_ctrl.h"
#include <stdio.h>
#include "math.h"
#include <ctype.h>
#include <stdlib.h>
#include "xil_types.h"
#include "xil_cache.h"
#include "xparameters.h"
#include "pic_800_600.h"
#include "sleep.h"
#include <sys/time.h>
#include "xgpiops.h"
#include "xgpio.h"
//# define MAX_N 14400   /*锟斤拷锟街滴狽锟斤拷锟皆讹拷锟斤拷锟斤拷锟襟长讹拷*/
# define MAX_M 1440000 
# define N 100
/*
 * XPAR redefines
 */
#define DYNCLK_BASEADDR XPAR_AXI_DYNCLK_0_BASEADDR
#define VGA_VDMA_ID XPAR_AXIVDMA_0_DEVICE_ID
#define DISP_VTC_ID XPAR_VTC_0_DEVICE_ID
#define GPIO_ID_0 XPAR_AXI_GPIO_0_DEVICE_ID
#define GPIO_ID_1 XPAR_AXI_GPIO_1_DEVICE_ID
#define GPIO_ID_2 XPAR_AXI_GPIO_2_DEVICE_ID
#define GPIO_ID_3 XPAR_AXI_GPIO_3_DEVICE_ID
/* ------------------------------------------------------------ */
/*				Global Variables								*/
/* ------------------------------------------------------------ */

/*
 * Display Driver structs
 */
DisplayCtrl dispCtrl;
XAxiVdma vdma;

/*
 * Framebuffers for video data
 */
u8 frameBuf[DISPLAY_NUM_FRAMES][DEMO_MAX_FRAME] __attribute__ ((aligned(64)));
u8 *pFrames[DISPLAY_NUM_FRAMES]; //array of pointers to the frame buffers

/* ------------------------------------------------------------ */
/*				Procedure Definitions							*/
/* ------------------------------------------------------------ */
int z[MAX_M];
int Junyun[MAX_M];
//int temp[MAX_M];

// XGpio Butt;
// XGpio_Initialize(&Butt,GPIO_ID);

// int Butt_val;
// Butt_val = XGpio_DiscreteRead(&Butt,1);
int Butt_val_0_0 = 0;
int Butt_val_0_1 = -1;//鍔犲櫔
int Butt_val_1_0 = 0;
int Butt_val_1_1 = -1;//閫夋嫨鍣０
int Butt_val_2_0 = 0;
int Butt_val_2_1 = -1;//绾櫔澹�
int Butt_val_3 = -1;//娓呴浂
int fuwei_0 = 0;
int fuwei_1 = 0;
int fuwei_2 = 0;
int fuwei_3 = 0;

// XGpio Butt_0;
// XGpio Butt_1;
// XGpio Butt_2;
// XGpio Butt_3;

int main(void)
{

	int Status;
	XAxiVdma_Config *vdmaConfig;
	int i;
	XGpio Butt_0;
	XGpio_Initialize(&Butt_0,GPIO_ID_0);
	XGpio Butt_1;
	XGpio_Initialize(&Butt_1,GPIO_ID_1);
	XGpio Butt_2;
	XGpio_Initialize(&Butt_2,GPIO_ID_2);
	XGpio Butt_3;
	XGpio_Initialize(&Butt_3,GPIO_ID_3);	
	
	DemoNoise();

	// Butt_val = XGpio_DiscreteRead(&Butt,1);

	/*
	 * Initialize an array of pointers to the 3 frame buffers
	 */
	for (i = 0; i < DISPLAY_NUM_FRAMES; i++)
	{
		pFrames[i] = frameBuf[i];
	}


	/*
	 * Initialize VDMA driver
	 */
	vdmaConfig = XAxiVdma_LookupConfig(VGA_VDMA_ID);
	if (!vdmaConfig)
	{
		xil_printf("No video DMA found for ID %d\r\n", VGA_VDMA_ID);

	}
	Status = XAxiVdma_CfgInitialize(&vdma, vdmaConfig, vdmaConfig->BaseAddress);
	if (Status != XST_SUCCESS)
	{
		xil_printf("VDMA Configuration Initialization failed %d\r\n", Status);

	}

	/*
	 * Initialize the Display controller and start it
	 */
	Status = DisplayInitialize(&dispCtrl, &vdma, DISP_VTC_ID, DYNCLK_BASEADDR, pFrames, DEMO_STRIDE);
	if (Status != XST_SUCCESS)
	{
		xil_printf("Display Ctrl initialization failed during demo initialization%d\r\n", Status);

	}
	Status = DisplayStart(&dispCtrl);
	if (Status != XST_SUCCESS)
	{
		xil_printf("Couldn't start display during demo initialization%d\r\n", Status);

	}
	
	Butt_val_0_0 = XGpio_DiscreteRead(&Butt_0,1);
	Butt_val_1_0 = XGpio_DiscreteRead(&Butt_1,1);
	Butt_val_2_0 = XGpio_DiscreteRead(&Butt_2,1);
	Butt_val_3 = XGpio_DiscreteRead(&Butt_3,1);
	while(1)
	{
		Butt_val_0_0 = XGpio_DiscreteRead(&Butt_0,1);
		Butt_val_1_0 = XGpio_DiscreteRead(&Butt_1,1);
		Butt_val_2_0 = XGpio_DiscreteRead(&Butt_2,1);
		Butt_val_3 = XGpio_DiscreteRead(&Butt_3,1);
		
		if(Butt_val_1_1-fuwei_1 != Butt_val_1_0-fuwei_1)
		{
			Butt_val_1_1 = Butt_val_1_0;
			if(Butt_val_1_0-fuwei_1 == 0)
			{
				DemoPrintTest(Butt_val_0_0-fuwei_0,dispCtrl.framePtr[dispCtrl.curFrame], dispCtrl.vMode.width, dispCtrl.vMode.height, dispCtrl.stride, DEMO_PATTERN_1);
			}
			else if(Butt_val_1_0-fuwei_1 == 1)
			{
				DemoPrintTest(Butt_val_0_0-fuwei_0,dispCtrl.framePtr[dispCtrl.curFrame], dispCtrl.vMode.width, dispCtrl.vMode.height, dispCtrl.stride, DEMO_PATTERN_2);
			}
			else if(Butt_val_1_0-fuwei_1 == 2)
			{
				DemoPrintTest(Butt_val_0_0-fuwei_0,dispCtrl.framePtr[dispCtrl.curFrame], dispCtrl.vMode.width, dispCtrl.vMode.height, dispCtrl.stride, DEMO_PATTERN_3);
			}
			else if(Butt_val_1_0-fuwei_1 == 3)
			{
				DemoPrintTest(Butt_val_0_0-fuwei_0,dispCtrl.framePtr[dispCtrl.curFrame], dispCtrl.vMode.width, dispCtrl.vMode.height, dispCtrl.stride, DEMO_PATTERN_4);
			}
		}
		if(Butt_val_0_1-fuwei_0 != Butt_val_0_0-fuwei_0)
		{
			Butt_val_0_1 = Butt_val_0_0;
			if(Butt_val_1_0-fuwei_1 == 0)
			{
				DemoPrintTest(Butt_val_0_0-fuwei_0,dispCtrl.framePtr[dispCtrl.curFrame], dispCtrl.vMode.width, dispCtrl.vMode.height, dispCtrl.stride, DEMO_PATTERN_1);
			}
			else if(Butt_val_1_0-fuwei_1 == 1)
			{
				DemoPrintTest(Butt_val_0_0-fuwei_0,dispCtrl.framePtr[dispCtrl.curFrame], dispCtrl.vMode.width, dispCtrl.vMode.height, dispCtrl.stride, DEMO_PATTERN_2);
			}
			else if(Butt_val_1_0-fuwei_1 == 2)
			{
				DemoPrintTest(Butt_val_0_0-fuwei_0,dispCtrl.framePtr[dispCtrl.curFrame], dispCtrl.vMode.width, dispCtrl.vMode.height, dispCtrl.stride, DEMO_PATTERN_3);
			}
			else if(Butt_val_1_0-fuwei_1 == 3)
			{
				DemoPrintTest(Butt_val_0_0-fuwei_0,dispCtrl.framePtr[dispCtrl.curFrame], dispCtrl.vMode.width, dispCtrl.vMode.height, dispCtrl.stride, DEMO_PATTERN_4);
			}
		}
		if(Butt_val_3 - fuwei_3 != 0)
		{
			fuwei_0 = Butt_val_0_0;
			fuwei_1 = Butt_val_1_0;
			fuwei_2 = Butt_val_2_0;
			fuwei_3 = Butt_val_3;
			DemoPrintTest(Butt_val_0_0-fuwei_0,dispCtrl.framePtr[dispCtrl.curFrame], dispCtrl.vMode.width, dispCtrl.vMode.height, dispCtrl.stride, DEMO_PATTERN_0);
		}
	}
	


	return 0;
}

// void juanji(float a[],float b[])
// {
// 	float a[MAX_M];
// 	float b[MAX_M];
// 	int temp[MAX_M];
	
// 	int i,j;
// 	for(i=0;i<MAX_M;i++)
// 	{
// 		temp[i]=0;
// 	}
// 	for(i=0;i<MAX_M;i++)
// 	{
// 		for(j=0;j<MAX_M;j++)
// 		{
// 			if((i-j+MAX_M)>=0&&(i-j+MAX_M)<MAX_M)
// 			{
// 				temp[i]+=a[i-j+MAX_M]*b[j];
// 			}
// 			else if((i-j)>=0&&(i-j<MAX_M))
// 			{
// 				temp[i]+=a[i-j]*b[j];
// 			}
// 		}
// 	}
	
// }
void DemoNoise()
{
	int a[20]={1,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0};
	int b[20]={0,0,0,1,1,0,0,0,0,0,1,0,0,0,0,1,0,1,0,0};
	float c,d;
	int temp1,temp2,i;
	for(i=0;i<MAX_M;i++)
	{
		temp1 = (a[19]^a[2])^a[0];
		a[0] = a[1];
		a[1] = a[2];
		a[2] = a[3];
		a[3] = a[4];
		a[4] = a[5];
		a[5] = a[6];
		a[6] = a[7];
		a[7] = a[8];
		a[8] = a[9];
		a[9] = a[10];
		a[10] = a[11];
		a[11] = a[12];
		a[12] = a[13];
		a[13] = a[14];
		a[14] = a[15];
		a[15] = a[16];
		a[16] = a[17];
		a[17] = a[18];
		a[18] = a[19];
		a[19] = temp1;
		c=a[19]*524288+a[18]*262144+a[17]*131072+a[16]*65536+a[15]*32768+a[14]*16384+a[13]*8192+a[12]*4096+a[11]*2048+a[10]+1024+a[9]*512+a[8]*256+a[7]*128+a[6]*64+a[5]*32+a[4]*16+a[3]*8+a[2]*4+a[1]*2+a[0];
		temp2 = (b[19]^b[2])^b[0];
		b[0] = b[1];
		b[1] = b[2];
		b[2] = b[3];
		b[3] = b[4];
		b[4] = b[5];
		b[5] = b[6];
		b[6] = b[7];
		b[7] = b[8];
		b[8] = b[9];
		b[9] = b[10];
		b[10] = b[11];
		b[11] = b[12];
		b[12] = b[13];
		b[13] = b[14];
		b[14] = b[15];
		b[15] = b[16];
		b[16] = b[17];
		b[17] = b[18];
		b[18] = b[19];
		b[19] =temp2;
		d=b[19]*524288+b[18]*262144+b[17]*131072+b[16]*65536+b[15]*32768+b[14]*16384+b[13]*8192+b[12]*4096+b[11]*2048+b[10]+1024+b[9]*512+b[8]*256+b[7]*128+b[6]*64+b[5]*32+b[4]*16+b[3]*8+b[2]*4+b[1]*2+b[0];
        z[i]=255*c*sin(d*2*180)/1047255;
		Junyun[i]=255*d/1047255;
	}

}

void DemoPrintTest(int Butt_val_0_0,u8 *frame, u32 width, u32 height, u32 stride, int pattern)
{
	u32 xcoi, ycoi;
	u32 iPixelAddr = 0;
	u8 wRed, wBlue, wGreen;
	u32 xInt;
	u32 pic_number=0;


	switch (pattern)
	{
	case DEMO_PATTERN_0:

		for(ycoi = 0; ycoi < 600; ycoi++)
		{
			for(xcoi = 0; xcoi < (800 * BYTES_PIXEL); xcoi+=BYTES_PIXEL)
			{
				frame[xcoi + iPixelAddr + 0] = gImage_pic_800_600[pic_number++];
				frame[xcoi + iPixelAddr + 1] = gImage_pic_800_600[pic_number++];
				frame[xcoi + iPixelAddr + 2] = gImage_pic_800_600[pic_number++];
			}
			iPixelAddr += stride;
		}
		/*
		 * Flush the framebuffer memory range to ensure changes are written to the
		 * actual memory, and therefore accessible by the VDMA.
		 */
		Xil_DCacheFlushRange((unsigned int) frame, DEMO_MAX_FRAME);
		break;
	case DEMO_PATTERN_1:         //Grid

		// for(ycoi = 0; ycoi < height; ycoi++)
		// {
		// 	for(xcoi = 0; xcoi < (width * BYTES_PIXEL); xcoi+=BYTES_PIXEL)
		// 	{
		// 		if (((xcoi/BYTES_PIXEL)&0x20)^(ycoi&0x20)) {
		// 			wRed = 255;
		// 			wGreen = 255;
		// 			wBlue = 255;
		// 		}
		// 		else{
		// 			wRed = 0;
		// 			wGreen = 0;
		// 			wBlue = 0;
		// 		}


		// 		frame[xcoi + iPixelAddr + 0] = wBlue;
		// 		frame[xcoi + iPixelAddr + 1] = wGreen;
		// 		frame[xcoi + iPixelAddr + 2] = wRed;
		// 	}
		// 	iPixelAddr += stride;
		// }
		// /*
		//  * Flush the framebuffer memory range to ensure changes are written to the
		//  * actual memory, and therefore accessible by the VDMA.
		//  */
		// Xil_DCacheFlushRange((unsigned int) frame, DEMO_MAX_FRAME);
		// break;
		for(ycoi = 0; ycoi < 600; ycoi++)
		{
			
			for(xcoi = 0; xcoi < (800 * BYTES_PIXEL); xcoi+=BYTES_PIXEL)
			{
				frame[xcoi + iPixelAddr + 0] = Butt_val_0_0*0.1*z[pic_number] + (1-Butt_val_0_0*0.1)*gImage_pic_800_600[pic_number++];
				frame[xcoi + iPixelAddr + 1] = Butt_val_0_0*0.1*z[pic_number] + (1-Butt_val_0_0*0.1)*gImage_pic_800_600[pic_number++];
				frame[xcoi + iPixelAddr + 2] = Butt_val_0_0*0.1*z[pic_number] + (1-Butt_val_0_0*0.1)*gImage_pic_800_600[pic_number++];
			}
			iPixelAddr += stride;
		}
		/*
		 * Flush the framebuffer memory range to ensure changes are written to the
		 * actual memory, and therefore accessible by the VDMA.
		 */
		Xil_DCacheFlushRange((unsigned int) frame, DEMO_MAX_FRAME);
		break;
	case DEMO_PATTERN_2://8 intervals color bar

		// for(ycoi = 0; ycoi < height; ycoi++)
		// {
		// 	for(xcoi = 0; xcoi < (width * BYTES_PIXEL); xcoi+=BYTES_PIXEL)
		// 	{

		// 		frame[xcoi + iPixelAddr + 0] = xcoi/BYTES_PIXEL;
		// 		frame[xcoi + iPixelAddr + 1] = xcoi/BYTES_PIXEL;
		// 		frame[xcoi + iPixelAddr + 2] = xcoi/BYTES_PIXEL;
		// 	}
		// 	iPixelAddr += stride;
		// }
		// /*
		//  * Flush the framebuffer memory range to ensure changes are written to the
		//  * actual memory, and therefore accessible by the VDMA.
		//  */
		// Xil_DCacheFlushRange((unsigned int) frame, DEMO_MAX_FRAME);
		// break;
		for(ycoi = 0; ycoi < 600; ycoi++)
		{
			//Butt_val_0_0 = XGpio_DiscreteRead(&Butt_0,1);
			for(xcoi = 0; xcoi < (800 * BYTES_PIXEL); xcoi+=BYTES_PIXEL)
			{
				frame[xcoi + iPixelAddr + 0] = z[pic_number++];
				frame[xcoi + iPixelAddr + 1] = z[pic_number++];
				frame[xcoi + iPixelAddr + 2] = z[pic_number++];
			}
			iPixelAddr += stride;
		}
		/*
		 * Flush the framebuffer memory range to ensure changes are written to the
		 * actual memory, and therefore accessible by the VDMA.
		 */
		Xil_DCacheFlushRange((unsigned int) frame, DEMO_MAX_FRAME);
		break;
	case DEMO_PATTERN_3: //8 intervals color bar

		// xInt = width*BYTES_PIXEL / 8; //each with width/8 pixels

		// for(ycoi = 0; ycoi < height; ycoi++)
		// {

		// 	/*
		// 	 * Just draw white in the last partial interval (when width is not divisible by 7)
		// 	 */

		// 	for(xcoi = 0; xcoi < (width*BYTES_PIXEL); xcoi+=BYTES_PIXEL)
		// 	{

		// 		if (xcoi < xInt) {                                   //White color
		// 			wRed = 255;
		// 			wGreen = 255;
		// 			wBlue = 255;
		// 		}

		// 		else if ((xcoi >= xInt) && (xcoi < xInt*2)){         //YELLOW color
		// 			wRed = 255;
		// 			wGreen = 255;
		// 			wBlue = 0;
		// 		}
		// 		else if ((xcoi >= xInt*2) && (xcoi < xInt*3)){        //CYAN color
		// 			wRed = 0;
		// 			wGreen = 255;
		// 			wBlue = 255;
		// 		}
		// 		else if ((xcoi >= xInt*3) && (xcoi < xInt*4)){        //GREEN color
		// 			wRed = 0;
		// 			wGreen = 255;
		// 			wBlue = 0;
		// 		}
		// 		else if ((xcoi >= xInt*4) && (xcoi < xInt*5)){        //MAGENTA color
		// 			wRed = 255;
		// 			wGreen = 0;
		// 			wBlue = 255;
		// 		}
		// 		else if ((xcoi >= xInt*5) && (xcoi < xInt*6)){        //RED color
		// 			wRed = 255;
		// 			wGreen = 0;
		// 			wBlue = 0;
		// 		}
		// 		else if ((xcoi >= xInt*6) && (xcoi < xInt*7)){        //BLUE color
		// 			wRed = 0;
		// 			wGreen = 0;
		// 			wBlue = 255;
		// 		}
		// 		else {                                                //BLACK color
		// 			wRed = 0;
		// 			wGreen = 0;
		// 			wBlue = 0;
		// 		}

		// 		frame[xcoi+iPixelAddr + 0] = wBlue;
		// 		frame[xcoi+iPixelAddr + 1] = wGreen;
		// 		frame[xcoi+iPixelAddr + 2] = wRed;
		// 		/*
		// 		 * This pattern is printed one vertical line at a time, so the address must be incremented
		// 		 * by the stride instead of just 1.
		// 		 */
		// 	}
		// 	iPixelAddr += stride;

		// }
		// /*
		//  * Flush the framebuffer memory range to ensure changes are written to the
		//  * actual memory, and therefore accessible by the VDMA.
		//  */
		// Xil_DCacheFlushRange((unsigned int) frame, DEMO_MAX_FRAME);
		// break;
		for(ycoi = 0; ycoi < 600; ycoi++)
		{
			//Butt_val_0_0 = XGpio_DiscreteRead(&Butt_0,1);
			for(xcoi = 0; xcoi < (800 * BYTES_PIXEL); xcoi+=BYTES_PIXEL)
			{
				frame[xcoi + iPixelAddr + 0] = Butt_val_0_0*0.1*Junyun[pic_number] + (1-Butt_val_0_0*0.1)*gImage_pic_800_600[pic_number++];
				frame[xcoi + iPixelAddr + 1] = Butt_val_0_0*0.1*Junyun[pic_number] + (1-Butt_val_0_0*0.1)*gImage_pic_800_600[pic_number++];
				frame[xcoi + iPixelAddr + 2] = Butt_val_0_0*0.1*Junyun[pic_number] + (1-Butt_val_0_0*0.1)*gImage_pic_800_600[pic_number++];
			}
			iPixelAddr += stride;
		}
		/*
		 * Flush the framebuffer memory range to ensure changes are written to the
		 * actual memory, and therefore accessible by the VDMA.
		 */
		Xil_DCacheFlushRange((unsigned int) frame, DEMO_MAX_FRAME);
		break;
	case DEMO_PATTERN_4://8 intervals color bar

		// for(ycoi = 0; ycoi < height; ycoi++)
		// {
		// 	for(xcoi = 0; xcoi < (width * BYTES_PIXEL); xcoi+=BYTES_PIXEL)
		// 	{

		// 		frame[xcoi + iPixelAddr + 0] = xcoi/BYTES_PIXEL;
		// 		frame[xcoi + iPixelAddr + 1] = xcoi/BYTES_PIXEL;
		// 		frame[xcoi + iPixelAddr + 2] = xcoi/BYTES_PIXEL;
		// 	}
		// 	iPixelAddr += stride;
		// }
		// /*
		//  * Flush the framebuffer memory range to ensure changes are written to the
		//  * actual memory, and therefore accessible by the VDMA.
		//  */
		// Xil_DCacheFlushRange((unsigned int) frame, DEMO_MAX_FRAME);
		// break;
		for(ycoi = 0; ycoi < 600; ycoi++)
		{
			//Butt_val_0_0 = XGpio_DiscreteRead(&Butt_0,1);
			for(xcoi = 0; xcoi < (800 * BYTES_PIXEL); xcoi+=BYTES_PIXEL)
			{
				frame[xcoi + iPixelAddr + 0] = Junyun[pic_number++];
				frame[xcoi + iPixelAddr + 1] = Junyun[pic_number++];
				frame[xcoi + iPixelAddr + 2] = Junyun[pic_number++];
			}
			iPixelAddr += stride;
		}
		/*
		 * Flush the framebuffer memory range to ensure changes are written to the
		 * actual memory, and therefore accessible by the VDMA.
		 */
		Xil_DCacheFlushRange((unsigned int) frame, DEMO_MAX_FRAME);
		break;
	default :
		xil_printf("Error: invalid pattern passed to DemoPrintTest");
	}
}


