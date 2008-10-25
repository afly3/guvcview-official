/*******************************************************************************#
#	    guvcview              http://guvcview.berlios.de                    #
#                                                                               #
#           Paulo Assis <pj.assis@gmail.com>                                    #
#										#
# This program is free software; you can redistribute it and/or modify         	#
# it under the terms of the GNU General Public License as published by   	#
# the Free Software Foundation; either version 2 of the License, or           	#
# (at your option) any later version.                                          	#
#                                                                              	#
# This program is distributed in the hope that it will be useful,              	#
# but WITHOUT ANY WARRANTY; without even the implied warranty of             	#
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the  		#
# GNU General Public License for more details.                                 	#
#                                                                              	#
# You should have received a copy of the GNU General Public License           	#
# along with this program; if not, write to the Free Software                  	#
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA	#
#                                                                              	#
********************************************************************************/

#include "colorspaces.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*------------------------------- Color space conversions --------------------*/
/* regular yuv (YUYV) to rgb24*/
void 
yuyv2rgb (BYTE *pyuv, BYTE *prgb, int width, int height){
	int l=0;
	int SizeYUV=height * width * 2; /* 2 bytes per pixel*/
	for(l=0;l<SizeYUV;l=l+4) { /*iterate every 4 bytes*/
		/* standart: r = y0 + 1.402 (v-128) */
		/* logitech: r = y0 + 1.370705 (v-128) */
		*prgb++=CLIP(pyuv[l] + 1.402 * (pyuv[l+3]-128));
		/* standart: g = y0 - 0.34414 (u-128) - 0.71414 (v-128)*/
		/* logitech: g = y0 - 0.337633 (u-128)- 0.698001 (v-128)*/
		*prgb++=CLIP(pyuv[l] - 0.34414 * (pyuv[l+1]-128) -0.71414*(pyuv[l+3]-128));
		/* standart: b = y0 + 1.772 (u-128) */
		/* logitech: b = y0 + 1.732446 (u-128) */
		*prgb++=CLIP(pyuv[l] + 1.772 *( pyuv[l+1]-128));
		/* standart: r1 =y1 + 1.402 (v-128) */
		/* logitech: r1 = y1 + 1.370705 (v-128) */
		*prgb++=CLIP(pyuv[l+2] + 1.402 * (pyuv[l+3]-128));
		/* standart: g1 = y1 - 0.34414 (u-128) - 0.71414 (v-128)*/
		/* logitech: g1 = y1 - 0.337633 (u-128)- 0.698001 (v-128)*/
		*prgb++=CLIP(pyuv[l+2] - 0.34414 * (pyuv[l+1]-128) -0.71414 * (pyuv[l+3]-128));
		/* standart: b1 = y1 + 1.772 (u-128) */
		/* logitech: b1 = y1 + 1.732446 (u-128) */
		*prgb++=CLIP(pyuv[l+2] + 1.772*(pyuv[l+1]-128));
	}
	
}


/* regular yuv (UYVY) to rgb24*/
void 
uyvy2rgb (BYTE *pyuv, BYTE *prgb, int width, int height){
	int l=0;
	int SizeYUV=height * width * 2; /* 2 bytes per pixel*/
	for(l=0;l<SizeYUV;l=l+4) { /*iterate every 4 bytes*/
		/* standart: r = y0 + 1.402 (v-128) */
		/* logitech: r = y0 + 1.370705 (v-128) */
		*prgb++=CLIP(pyuv[l+1] + 1.402 * (pyuv[l+2]-128));
		/* standart: g = y0 - 0.34414 (u-128) - 0.71414 (v-128)*/
		/* logitech: g = y0 - 0.337633 (u-128)- 0.698001 (v-128)*/
		*prgb++=CLIP(pyuv[l+1] - 0.34414 * (pyuv[l]-128) -0.71414*(pyuv[l+2]-128));
		/* standart: b = y0 + 1.772 (u-128) */
		/* logitech: b = y0 + 1.732446 (u-128) */
		*prgb++=CLIP(pyuv[l+1] + 1.772 *( pyuv[l]-128));
		/* standart: r1 =y1 + 1.402 (v-128) */
		/* logitech: r1 = y1 + 1.370705 (v-128) */
		*prgb++=CLIP(pyuv[l+3] + 1.402 * (pyuv[l+2]-128));
		/* standart: g1 = y1 - 0.34414 (u-128) - 0.71414 (v-128)*/
		/* logitech: g1 = y1 - 0.337633 (u-128)- 0.698001 (v-128)*/
		*prgb++=CLIP(pyuv[l+3] - 0.34414 * (pyuv[l]-128) -0.71414 * (pyuv[l+2]-128));
		/* standart: b1 = y1 + 1.772 (u-128) */
		/* logitech: b1 = y1 + 1.732446 (u-128) */
		*prgb++=CLIP(pyuv[l+3] + 1.772*(pyuv[l]-128));
	}
	
}

/* yuv (YUYV) to bgr with lines upsidedown */
/* used for bitmap files (DIB24)           */
void 
yuyv2bgr (BYTE *pyuv, BYTE *pbgr, int width, int height){

	int l=0;
	int k=0;
	BYTE *preverse;
	int bytesUsed;
	int SizeBGR=height * width * 3; /* 3 bytes per pixel*/
	/* BMP byte order is bgr and the lines start from last to first*/
	preverse=pbgr+SizeBGR;/*start at the end and decrement*/
	//printf("preverse addr:%d | pbgr addr:%d | diff:%d\n",preverse,pbgr,preverse-pbgr);
	for(l=0;l<height;l++) { /*iterate every 1 line*/
		preverse-=width*3;/*put pointer at begin of unprocessed line*/
		bytesUsed=l*width*2;
		for (k=0;k<((width)*2);k=k+4)/*iterate every 4 bytes in the line*/
		{                              
		/* standart: b = y0 + 1.772 (u-128) */
		/* logitech: b = y0 + 1.732446 (u-128) */
		*preverse++=CLIP(pyuv[k+bytesUsed] + 1.772 *( pyuv[k+1+bytesUsed]-128)); 
		/* standart: g = y0 - 0.34414 (u-128) - 0.71414 (v-128)*/
		/* logitech: g = y0 - 0.337633 (u-128)- 0.698001 (v-128)*/
		*preverse++=CLIP(pyuv[k+bytesUsed] - 0.34414 * (pyuv[k+1+bytesUsed]-128) -0.71414*(pyuv[k+3+bytesUsed]-128));
		/* standart: r = y0 + 1.402 (v-128) */
		/* logitech: r = y0 + 1.370705 (v-128) */
		*preverse++=CLIP(pyuv[k+bytesUsed] + 1.402 * (pyuv[k+3+bytesUsed]-128));                                                        
		/* standart: b1 = y1 + 1.772 (u-128) */
		/* logitech: b1 = y1 + 1.732446 (u-128) */
		*preverse++=CLIP(pyuv[k+2+bytesUsed] + 1.772*(pyuv[k+1+bytesUsed]-128));
		/* standart: g1 = y1 - 0.34414 (u-128) - 0.71414 (v-128)*/
		/* logitech: g1 = y1 - 0.337633 (u-128)- 0.698001 (v-128)*/
		*preverse++=CLIP(pyuv[k+2+bytesUsed] - 0.34414 * (pyuv[k+1+bytesUsed]-128) -0.71414 * (pyuv[k+3+bytesUsed]-128)); 
		/* standart: r1 =y1 + 1.402 (v-128) */
		/* logitech: r1 = y1 + 1.370705 (v-128) */
		*preverse++=CLIP(pyuv[k+2+bytesUsed] + 1.402 * (pyuv[k+3+bytesUsed]-128));
		}
		preverse-=width*3;/*get it back at the begin of processed line*/
	}
	//printf("preverse addr:%d | pbgr addr:%d | diff:%d\n",preverse,pbgr,preverse-pbgr);
	preverse=NULL;

}

/* yuv (UYVY) to bgr with lines upsidedown */
/* used for bitmap files (DIB24)           */
void 
uyvy2bgr (BYTE *pyuv, BYTE *pbgr, int width, int height){

	int l=0;
	int k=0;
	BYTE *preverse;
	int bytesUsed;
	int SizeBGR=height * width * 3; /* 3 bytes per pixel*/
	/* BMP byte order is bgr and the lines start from last to first*/
	preverse=pbgr+SizeBGR;/*start at the end and decrement*/
	//printf("preverse addr:%d | pbgr addr:%d | diff:%d\n",preverse,pbgr,preverse-pbgr);
	for(l=0;l<height;l++) { /*iterate every 1 line*/
		preverse-=width*3;/*put pointer at begin of unprocessed line*/
		bytesUsed=l*width*2;
		for (k=0;k<((width)*2);k=k+4)/*iterate every 4 bytes in the line*/
		{                              
		/* standart: b = y0 + 1.772 (u-128) */
		/* logitech: b = y0 + 1.732446 (u-128) */
		*preverse++=CLIP(pyuv[k+1+bytesUsed] + 1.772 *( pyuv[k+bytesUsed]-128)); 
		/* standart: g = y0 - 0.34414 (u-128) - 0.71414 (v-128)*/
		/* logitech: g = y0 - 0.337633 (u-128)- 0.698001 (v-128)*/
		*preverse++=CLIP(pyuv[k+1+bytesUsed] - 0.34414 * (pyuv[k+bytesUsed]-128) -0.71414*(pyuv[k+2+bytesUsed]-128));
		/* standart: r = y0 + 1.402 (v-128) */
		/* logitech: r = y0 + 1.370705 (v-128) */
		*preverse++=CLIP(pyuv[k+1+bytesUsed] + 1.402 * (pyuv[k+2+bytesUsed]-128));                                                        
		/* standart: b1 = y1 + 1.772 (u-128) */
		/* logitech: b1 = y1 + 1.732446 (u-128) */
		*preverse++=CLIP(pyuv[k+3+bytesUsed] + 1.772*(pyuv[k+bytesUsed]-128));
		/* standart: g1 = y1 - 0.34414 (u-128) - 0.71414 (v-128)*/
		/* logitech: g1 = y1 - 0.337633 (u-128)- 0.698001 (v-128)*/
		*preverse++=CLIP(pyuv[k+3+bytesUsed] - 0.34414 * (pyuv[k+bytesUsed]-128) -0.71414 * (pyuv[k+2+bytesUsed]-128)); 
		/* standart: r1 =y1 + 1.402 (v-128) */
		/* logitech: r1 = y1 + 1.370705 (v-128) */
		*preverse++=CLIP(pyuv[k+3+bytesUsed] + 1.402 * (pyuv[k+2+bytesUsed]-128));
		}
		preverse-=width*3;/*get it back at the begin of processed line*/
	}
	//printf("preverse addr:%d | pbgr addr:%d | diff:%d\n",preverse,pbgr,preverse-pbgr);
	preverse=NULL;

}

int yuv420_to_yuyv (BYTE *framebuffer, BYTE *tmpbuffer, int width, int height) 
{
    BYTE *py;
    BYTE *pu;
    BYTE *pv;
    
    py=tmpbuffer;
    pu=py+(width*height);
    pv=pu+(width*height/4);
    
    int h=0;
    int w=0;
    
    int wy=0;
    int huv=0;
    int wuv=0;
    
    for(h=0;h<height;h+=2) 
    {
	wy=0;
	wuv=0;
	for(w=0;w<width*2;w+=4) 
	{
		/*y00*/
		framebuffer[h*width+w] = py[h*width+wy];
		/*u0*/
		framebuffer[h*width+(w+1)] = pu[huv*(width/2)+wuv];
		/*y01*/
		framebuffer[h*width+(w+2)] = py[h*width+(wy+1)];
		/*u0*/
		framebuffer[h*width+(w+3)] = pu[huv*(width/2)+wuv];
		
		/*y10*/
		framebuffer[(h+1)*width+w] = py[(h+1)*width+wy];
		/*u0*/
		framebuffer[(h+1)*width+(w+1)] = pu[huv*(width/2)+wuv];
		/*y11*/
		framebuffer[(h+1)*width+(w+2)] = py[(h+1)*width+(wy+1)];
		/*u0*/
		framebuffer[(h+1)*width+(w+3)] = pu[huv*(width/2)+wuv];
		wuv++;
		wy+=2;
	}
	huv++;
    }
    return (0);
}


/* raw bayer functions*/
#define R(x,y) pRGB24[0 + 3 * ((x) + width * (y))]
#define G(x,y) pRGB24[1 + 3 * ((x) + width * (y))]
#define B(x,y) pRGB24[2 + 3 * ((x) + width * (y))]

#define Bay(x,y) pBay[(x) + width * (y)]

static void bayer_copy_bg(BYTE *pBay, BYTE *pRGB24, int x, int y, int width)
{

	B(x + 1, y + 1) = B(x + 0, y + 0) = B(x + 0, y + 1) = B(x + 1, y + 0) = Bay(x + 0, y + 0);
	G(x + 0, y + 0) = G(x+1,x+1) = ((DWORD)Bay(x + 1, y + 0) + (DWORD)Bay(x+0,y+1))>>1;
	G(x + 1, y + 0) = Bay(x + 1, y + 0);
	G(x + 0, y + 1) = Bay(x + 0, y + 1);
	R(x + 0, y + 0) = R(x + 1, y + 0) = R(x + 1, y + 1) = R(x + 0, y + 1) = Bay(x + 1, y + 1);
}

static void bayer_bilinear_bg(BYTE *pBay, BYTE *pRGB24, int x, int y, int width)
{
	B(x + 0, y + 0) = Bay(x + 0, y + 0);
   
   	if (abs((DWORD)Bay(x + 0, y - 1) - (DWORD)Bay(x + 0, y +1)) > 
	       abs((DWORD)Bay(x - 1, y + 0) - (DWORD)Bay(x + 1, y + 0))) 
   	{
		G(x + 0, y + 0)= ((DWORD)Bay(x + 0, y - 1) + (DWORD)Bay(x + 0, y +1)) >> 1;
	} else {
		G(x + 0, y + 0)= ((DWORD)Bay(x - 1, y + 0) + (DWORD)Bay(x + 1, y + 0)) >> 1;
	}
	//~ G(x + 0, y + 0) = ((DWORD)Bay(x + 0, y - 1) + (DWORD)Bay(x + 0, y +1) +
			   //~ (DWORD)Bay(x - 1, y + 0) + (DWORD)Bay(x + 1, y + 0)) >> 2;
	R(x + 0, y + 0) = ((DWORD)Bay(x - 1, y - 1) + (DWORD)Bay(x + 1, y - 1)+
			   (DWORD)Bay(x - 1, y + 1) + (DWORD)Bay(x + 1, y + 1)) >> 2;
		
	B(x + 0, y + 1) = ((DWORD)Bay(x + 0, y + 0) + (DWORD)Bay(x + 0, y + 2)) >> 1;
	G(x + 0, y + 1) = Bay(x + 0, y + 1);
	R(x + 0, y + 1) = ((DWORD) Bay(x - 1, y + 1) +(DWORD)Bay(x + 1, y + 1)) >> 1;
			 
	B(x + 1, y + 0) = ((DWORD)Bay(x +0, y +0) + (DWORD)Bay(x + 2, y +0)) >> 1;
	G(x + 1, y + 0) = Bay(x + 1, y + 0);
	R(x + 1, y + 0) = ((DWORD)Bay(x + 1, y - 1) + (DWORD)Bay(x + 1, y + 1)) >> 1;

	B(x + 1, y + 1) = ((DWORD)Bay(x + 0, y + 0) + (DWORD)Bay(x + 0, y + 2) +
                           (DWORD)Bay(x + 2, y + 0) + (DWORD)Bay(x + 2, y + 2)) >>2;
   
   	if(abs((DWORD)Bay(x + 1, y + 0) - (DWORD)Bay(x + 1, y + 2)) > 
	      abs((DWORD)Bay(x + 0, y + 1) - (DWORD)Bay(x + 2, y + 1))) 
   	{
		G(x + 1, y + 1) = ((DWORD)Bay(x + 1, y + 0) + (DWORD)Bay(x + 1, y + 2)) >> 1;
	} else {
		G(x + 1, y + 1) = ((DWORD)Bay(x + 0, y + 1) + (DWORD)Bay(x + 2, y + 1)) >> 1;
	}
	//~ G(x + 1, y + 1) = ((DWORD)Bay(x + 1, y + 0) + (DWORD)Bay(x + 1, y + 2) +
			   //~ (DWORD)Bay(x + 0, y + 1) + (DWORD)Bay(x + 2, y + 1)) >> 2;
	R(x + 1, y + 1) = Bay(x + 1, y + 1);
}

static void bayer_copy_rg(BYTE *pBay, BYTE *pRGB24, int x, int y, int width)
{

	R(x + 1, y + 1) = R(x + 0, y + 0) = R(x + 0, y + 1) = R(x + 1, y + 0) = Bay(x + 0, y + 0);
	G(x + 0, y + 0) = G(x+1,x+1) = ((DWORD)Bay(x + 1, y + 0) + (DWORD)Bay(x+0,y+1))>>1;
	G(x + 1, y + 0) = Bay(x + 1, y + 0);
	G(x + 0, y + 1) = Bay(x + 0, y + 1);
	B(x + 0, y + 0) = B(x + 1, y + 0) = B(x + 1, y + 1) = B(x + 0, y + 1) = Bay(x + 1, y + 1);
}

static void bayer_bilinear_rg(BYTE *pBay, BYTE *pRGB24, int x, int y, int width)
{
	R(x + 0, y + 0) = Bay(x + 0, y + 0);
   
   	if (abs((DWORD)Bay(x + 0, y - 1) - (DWORD)Bay(x + 0, y +1)) > 
	       abs((DWORD)Bay(x - 1, y + 0) - (DWORD)Bay(x + 1, y + 0))) 
   	{
		G(x + 0, y + 0)= ((DWORD)Bay(x + 0, y - 1) + (DWORD)Bay(x + 0, y +1)) >> 1;
	} else {
		G(x + 0, y + 0)= ((DWORD)Bay(x - 1, y + 0) + (DWORD)Bay(x + 1, y + 0)) >> 1;
	}
	//~ G(x + 0, y + 0) = ((DWORD)Bay(x + 0, y - 1) + (DWORD)Bay(x + 0, y +1) +
			   //~ (DWORD)Bay(x - 1, y + 0) + (DWORD)Bay(x + 1, y + 0)) >> 2;
	B(x + 0, y + 0) = ((DWORD)Bay(x - 1, y - 1) + (DWORD)Bay(x + 1, y - 1)+
			   (DWORD)Bay(x - 1, y + 1) + (DWORD)Bay(x + 1, y + 1)) >> 2;
		
	R(x + 0, y + 1) = ((DWORD)Bay(x + 0, y + 0) + (DWORD)Bay(x + 0, y + 2)) >> 1;
	G(x + 0, y + 1) = Bay(x + 0, y + 1);
	B(x + 0, y + 1) = ((DWORD) Bay(x - 1, y + 1) +(DWORD)Bay(x + 1, y + 1)) >> 1;
			 
	R(x + 1, y + 0) = ((DWORD)Bay(x +0, y +0) + (DWORD)Bay(x + 2, y +0)) >> 1;
	G(x + 1, y + 0) = Bay(x + 1, y + 0);
	B(x + 1, y + 0) = ((DWORD)Bay(x + 1, y - 1) + (DWORD)Bay(x + 1, y + 1)) >> 1;

	R(x + 1, y + 1) = ((DWORD)Bay(x + 0, y + 0) + (DWORD)Bay(x + 0, y + 2) +
                           (DWORD)Bay(x + 2, y + 0)+ (DWORD)Bay(x + 2, y + 2)) >> 2;
   	if(abs((DWORD)Bay(x + 1, y + 0) - (DWORD)Bay(x + 1, y + 2)) > 
	      abs((DWORD)Bay(x + 0, y + 1) - (DWORD)Bay(x + 2, y + 1))) 
   	{
		G(x + 1, y + 1) = ((DWORD)Bay(x + 1, y + 0) + (DWORD)Bay(x + 1, y + 2)) >> 1;
	} else {
		G(x + 1, y + 1) = ((DWORD)Bay(x + 0, y + 1) + (DWORD)Bay(x + 2, y + 1)) >> 1;
	}
	//~ G(x + 1, y + 1) = ((DWORD)Bay(x + 1, y + 0) + (DWORD)Bay(x + 1, y + 2) +
			   //~ (DWORD)Bay(x + 0, y + 1) + (DWORD)Bay(x + 2, y + 1)) >> 2;
	B(x + 1, y + 1) = Bay(x + 1, y + 1);
}

static void bayer_copy_gb(BYTE *pBay, BYTE *pRGB24, int x, int y, int width)
{

	R(x + 1, y + 1) = R(x + 0, y + 0) = R(x + 0, y + 1) = R(x + 1, y + 0) = Bay(x + 0, y + 1);
	G(x + 0, y + 0) = Bay(x + 0, y + 0);
	G(x + 1, y + 1) = Bay(x + 1, y + 1);
	G(x + 0, y + 1) = G(x + 1, y + 0) = ((DWORD)Bay(x + 0, y + 0) + (DWORD)Bay(x + 1, y + 1)) >> 1;
	B(x + 0, y + 0) = B(x + 1, y + 0) = B(x + 1, y + 1) = B(x + 0, y + 1) = Bay(x + 1, y + 0);
}

static void bayer_bilinear_gb(BYTE *pBay, BYTE *pRGB24, int x, int y, int width)
{
	B(x + 0, y + 0) = ((DWORD)Bay(x - 1, y + 0) + (DWORD)Bay(x + 1, y + 0)) >> 1;
	G(x + 0, y + 0) = Bay(x + 0, y + 0);
	R(x + 0, y + 0) = ((DWORD)Bay(x + 0, y + 1) + (DWORD)Bay(x + 0, y - 1)) >> 1;
		
	B(x + 0, y + 1) = ((DWORD)Bay(x + 1, y + 0) + (DWORD)Bay(x - 1, y + 0)
			 + (DWORD)Bay(x + 1, y + 2) + (DWORD)Bay(x - 1, y + 2)) >> 2;
   	if (abs((DWORD)Bay(x + 0, y + 0) - (DWORD)Bay(x + 0, y + 2)) >
	       abs((DWORD)Bay(x - 1, y + 1) + (DWORD)Bay(x + 1, y + 1))) 
   	{
		G(x + 0, y + 1) = ((DWORD)Bay(x + 0, y + 0) + (DWORD)Bay(x + 0, y + 2)) >> 1;
	} else {
		G(x + 0, y + 1) = ((DWORD)Bay(x - 1, y + 1) + (DWORD)Bay(x + 1, y + 1)) >> 1;
	}
	//~ G(x + 0, y + 1) = ((DWORD)Bay(x + 0, y + 0) + (DWORD)Bay(x + 0, y + 2)
			 //~ + (DWORD)Bay(x - 1, y + 1) + (DWORD)Bay(x + 1, y + 1)) >> 2;
	R(x + 0, y + 1) = Bay(x + 0, y + 1);
			 
	B(x + 1, y + 0) = Bay(x + 1, y + 0);
   	if (abs((DWORD)Bay(x + 0, y + 0) - (DWORD)Bay(x + 2, y + 0)) >
	       abs((DWORD)Bay(x + 1, y - 1) + (DWORD)Bay(x + 1, y + 1))) 
   	{
		G(x + 1, y + 0) = ((DWORD)Bay(x + 0, y + 0) + (DWORD)Bay(x + 2, y + 0)) >> 1;
	} else {
		G(x + 1, y + 0) = ((DWORD)Bay(x + 1, y - 1) + (DWORD)Bay(x + 1, y + 1)) >> 1;
	}
	//~ G(x + 1, y + 0) = ((DWORD)Bay(x + 0, y + 0) + (DWORD)Bay(x + 2, y + 0)
			 //~ + (DWORD)Bay(x + 1, y - 1) + (DWORD)Bay(x + 1, y + 1)) >> 2;
	R(x + 1, y + 0) = ((DWORD)Bay(x + 0, y + 1) + (DWORD)Bay(x + 2, y + 1)
			 + (DWORD)Bay(x + 0, y - 1) + (DWORD)Bay(x + 2, y - 1)) >> 2;

	B(x + 1, y + 1) = ((DWORD)Bay(x + 1, y + 0) + (DWORD)Bay(x + 1, y + 2)) >> 1;
	G(x + 1, y + 1) = Bay(x + 1, y + 1);
	R(x + 1, y + 1) = ((DWORD)Bay(x + 0, y + 1) + (DWORD)Bay(x + 2, y + 1)) >> 1;
}

static void bayer_copy_gr(BYTE *pBay, BYTE *pRGB24, int x, int y, int width)
{

	R(x + 1, y + 1) = B(x + 0, y + 0) = B(x + 0, y + 1) = B(x + 1, y + 0) = Bay(x + 1, y + 0);
	G(x + 0, y + 0) = Bay(x + 0, y + 0);
	G(x + 1, y + 1) = Bay(x + 1, y + 1);
	G(x + 0, y + 1) = G(x + 1, y + 0) = ((DWORD)Bay(x + 0, y + 0) + (DWORD)Bay(x + 1, y + 1)) >> 1;
	B(x + 0, y + 0) = R(x + 1, y + 0) = R(x + 1, y + 1) = R(x + 0, y + 1) = Bay(x + 0, y + 1);
}

static void bayer_bilinear_gr(BYTE *pBay, BYTE *pRGB24, int x, int y, int width)
{
	R(x + 0, y + 0) = ((DWORD)Bay(x - 1, y + 0) + (DWORD)Bay(x + 1, y + 0)) >> 1;
	G(x + 0, y + 0) = Bay(x + 0, y + 0);
	B(x + 0, y + 0) = ((DWORD)Bay(x + 0, y + 1) + (DWORD)Bay(x + 0, y - 1)) >> 1;
		
	R(x + 0, y + 1) = ((DWORD)Bay(x + 1, y + 0) + (DWORD)Bay(x - 1, y + 0)
			 + (DWORD)Bay(x + 1, y + 2) + (DWORD)Bay(x - 1, y + 2)) >> 2;
   	
   	if (abs((DWORD)Bay(x + 0, y + 0) - (DWORD)Bay(x + 0, y + 2)) >
	       abs((DWORD)Bay(x - 1, y + 1) + (DWORD)Bay(x + 1, y + 1))) 
   	{
		G(x + 0, y + 1) = ((DWORD)Bay(x + 0, y + 0) + (DWORD)Bay(x + 0, y + 2)) >> 1;
	} else {
		G(x + 0, y + 1) = ((DWORD)Bay(x - 1, y + 1) + (DWORD)Bay(x + 1, y + 1)) >> 1;
	}
	//~ G(x + 0, y + 1) = ((DWORD)Bay(x + 0, y + 0) + (DWORD)Bay(x + 0, y + 2)
			 //~ + (DWORD)Bay(x - 1, y + 1) + (DWORD)Bay(x + 1, y + 1)) >> 2;
	B(x + 0, y + 1) = Bay(x + 0, y + 1);
			 
	R(x + 1, y + 0) = Bay(x + 1, y + 0);
   
   	if (abs((DWORD)Bay(x + 0, y + 0) - (DWORD)Bay(x + 2, y + 0)) >
	       abs((DWORD)Bay(x + 1, y - 1) + (DWORD)Bay(x + 1, y + 1))) 
   	{
		G(x + 1, y + 0) = ((DWORD)Bay(x + 0, y + 0) + (DWORD)Bay(x + 2, y + 0)) >> 1;
	} else {
		G(x + 1, y + 0) = ((DWORD)Bay(x + 1, y - 1) + (DWORD)Bay(x + 1, y + 1)) >> 1;
	}
	//~ G(x + 1, y + 0) = ((DWORD)Bay(x + 0, y + 0) + (DWORD)Bay(x + 2, y + 0)
			 //~ + (DWORD)Bay(x + 1, y - 1) + (DWORD)Bay(x + 1, y + 1)) >> 2;
	B(x + 1, y + 0) = ((DWORD)Bay(x + 0, y + 1) + (DWORD)Bay(x + 2, y + 1)
			 + (DWORD)Bay(x + 0, y - 1) + (DWORD)Bay(x + 2, y - 1)) >> 2;

	R(x + 1, y + 1) = ((DWORD)Bay(x + 1, y + 0) + (DWORD)Bay(x + 1, y + 2)) >> 1;
	G(x + 1, y + 1) = Bay(x + 1, y + 1);
	B(x + 1, y + 1) = ((DWORD)Bay(x + 0, y + 1) + (DWORD)Bay(x + 2, y + 1)) >> 1;
}



void 
bayer_to_rgb24(BYTE *pBay, BYTE *pRGB24, int width, int height, int pix_order)
{
	int i, j;
   	switch (pix_order) {
	      case 0: /* gbgbgb... | rgrgrg... */
		for (i = 0; i < width; i += 2) {
			for (j = 0; j < height; j += 2) {
				if (i == 0 || j == 0 || i == (width - 2) || j == (height - 2))
					bayer_copy_gb(pBay, pRGB24, i, j, width);
				else
					bayer_bilinear_gb(pBay, pRGB24, i, j, width);
			}
		}
	        break;
	      case 1: /* grgrgr... | bgbgbg... */
		for (i = 0; i < width; i += 2) {
			for (j = 0; j < height; j += 2) {
				if (i == 0 || j == 0 || i == (width - 2) || j == (height - 2))
					bayer_copy_gr(pBay, pRGB24, i, j, width);
				else
					bayer_bilinear_gr(pBay, pRGB24, i, j, width);
			}
		}
	        break;
	      case 2: /* bgbgbg... | grgrgr */
		 for (i = 0; i < width; i += 2) {
			for (j = 0; j < height; j += 2) {
				if (i == 0 || j == 0 || i == (width - 2) || j == (height - 2))
					bayer_copy_bg(pBay, pRGB24, i, j, width);
				else
					bayer_bilinear_bg(pBay, pRGB24, i, j, width);
			}
		}
	        break;
	      case 3: /* rgrgrg... ! gbgbgb... */
		 for (i = 0; i < width; i += 2) {
			for (j = 0; j < height; j += 2) {
				if (i == 0 || j == 0 || i == (width - 2) || j == (height - 2))
					bayer_copy_rg(pBay, pRGB24, i, j, width);
				else
					bayer_bilinear_rg(pBay, pRGB24, i, j, width);
			}
		}
	        break;
	      default: /* default is gb*/
		 for (i = 0; i < width; i += 2) {
			for (j = 0; j < height; j += 2) {
				if (i == 0 || j == 0 || i == width - 2 || j == height - 2)
					bayer_copy_gb(pBay, pRGB24, i, j, width);
				else
					bayer_bilinear_gb(pBay, pRGB24, i, j, width);
			}
		}
	        break;
	}
}


void
rgb2yuyv(BYTE *prgb, BYTE *pyuv, int width, int height) {

   int i=0;
   for(i=0;i<(width*height*3);i=i+6) {
       	/* y */ 
       	*pyuv++ =CLIP(0.299 * (prgb[i] - 128) + 0.587 * (prgb[i+1] - 128) + 0.114 * (prgb[i+2] - 128) + 128);
 	/* u */
        *pyuv++ =CLIP(((- 0.147 * (prgb[i] - 128) - 0.289 * (prgb[i+1] - 128) + 0.436 * (prgb[i+2] - 128) + 128) +
		      (- 0.147 * (prgb[i+3] - 128) - 0.289 * (prgb[i+4] - 128) + 0.436 * (prgb[i+5] - 128) + 128))/2);
        /* y1 */ 
       	*pyuv++ =CLIP(0.299 * (prgb[i+3] - 128) + 0.587 * (prgb[i+4] - 128) + 0.114 * (prgb[i+5] - 128) + 128); 
        /* v*/
	*pyuv++ =CLIP(((0.615 * (prgb[i] - 128) - 0.515 * (prgb[i+1] - 128) - 0.100 * (prgb[i+2] - 128) + 128) +
		       (0.615 * (prgb[3] - 128) - 0.515 * (prgb[i+4] - 128) - 0.100 * (prgb[i+5] - 128) + 128))/2);
   }
}

/*-------------------------------- YUV Filters -------------------------------*/
/* Flip YUYV frame horizontal*/
void 
yuyv_mirror (BYTE *frame, int width, int height){
	
	int h=0;
	int w=0;
	int sizeline = width*2; /* 2 bytes per pixel*/ 
	BYTE *pframe;
	pframe=frame;
	BYTE line[sizeline-1];/*line buffer*/
    	for (h=0; h < height; h++) { /*line iterator*/
        	for(w=sizeline-1; w > 0; w = w - 4) { /* pixel iterator */
            	line[w-1]=*pframe++;
	    		line[w-2]=*pframe++;
	    		line[w-3]=*pframe++;
	    		line[w]=*pframe++;
		}
        	memcpy(frame+(h*sizeline), line, sizeline); /*copy reversed line to frame buffer*/           
    }
	
}

/* Flip UYVY frame horizontal*/
void 
uyvy_mirror (BYTE *frame, int width, int height){
	
	int h=0;
	int w=0;
	int sizeline = width*2; /* 2 bytes per pixel*/ 
	BYTE *pframe;
	pframe=frame;
	BYTE line[sizeline-1];/*line buffer*/
    	for (h=0; h < height; h++) { /*line iterator*/
        	for(w=sizeline-1; w > 0; w = w - 4) { /* pixel iterator */
            	line[w-3]=*pframe++;
	    		line[w]=*pframe++;
	    		line[w-1]=*pframe++;
	    		line[w-2]=*pframe++;
		}
        	memcpy(frame+(h*sizeline), line, sizeline); /*copy reversed line to frame buffer*/           
    }
	
}

void 
yuyv_negative(BYTE* frame, int width, int height)
{
	int size=width*height*2;
	int i=0;
    for(i=0; i < size; i++)
        frame[i] = ~frame[i];     
}

void 
yuyv_upturn(BYTE* frame, int width, int height)
{   
	int h=0;
	int sizeline = width*2; /* 2 bytes per pixel*/ 
	BYTE *pframe;
	pframe=frame;
	BYTE line1[sizeline-1];/*line1 buffer*/
	BYTE line2[sizeline-1];/*line2 buffer*/
    for (h=0; h < height/2; h++) { /*line iterator*/	
       	memcpy(line1,frame+h*sizeline,sizeline);
		memcpy(line2,frame+(height-1-h)*sizeline,sizeline);
			
		memcpy(frame+h*sizeline, line2, sizeline);
		memcpy(frame+(height-1-h)*sizeline, line1, sizeline);
	}      
}


void
yuyv_monochrome(BYTE* frame, int width, int height) 
{
    int size=width*height*2;
	int i=0;

	for(i=0; i < size; i = i + 4) { /* keep Y - luma */
        frame[i+1]=0x80;/*U - median (half the max value)=128*/
        frame[i+3]=0x80;/*V - median (half the max value)=128*/        
    }
}

void
uyvy_monochrome(BYTE* frame, int width, int height) 
{
    int size=width*height*2;
	int i=0;

	for(i=0; i < size; i = i + 4) { /* keep Y - luma */
        frame[i]=0x80;/*U - median (half the max value)=128*/
        frame[i+2]=0x80;/*V - median (half the max value)=128*/        
    }
}
