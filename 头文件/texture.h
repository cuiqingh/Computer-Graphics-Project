#pragma once 
#define GLUT_DISABLE_ATEXIT_HACK      
#include "GL/GLUT.H"      
#include <stdio.h> 
 
//读取纹理图片数据（BMP）
static unsigned char *LoadBmpFile(char *filename, BITMAPINFOHEADER *bmpInfoHeader)
{    

	FILE* filePtr = fopen(filename, "rb");	//打开BMP文件
	if (filePtr == NULL) {		//文件打开错误
		printf("Texture file open error.\n");
		return NULL;
	}

	BITMAPFILEHEADER bmpFileHeader;
	fread(&bmpFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);	// 读入BMP文件头
	if (bmpFileHeader.bfType != 0x4D42) {	// 验证是否为bitmap文件
		
		fprintf(stderr, "The file is not a bitmap file.\n");
		return NULL;
	}

	fread(bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);		// 读入BMP信息头
	fseek(filePtr, bmpFileHeader.bfOffBits, SEEK_SET);		// 将文件指针移至BMP数据区     
	unsigned char* bmpImage = new unsigned char[bmpInfoHeader->biSizeImage];	//存放位图数据
	if (!bmpImage) {		//内存创建失败
		fprintf(stderr, "Error in LoadBitmapFile: memory error\n");
		return NULL;
	}
	fread(bmpImage, 1, bmpInfoHeader->biSizeImage, filePtr);

	unsigned char temp;
	for (int idx = 0; idx < bmpInfoHeader->biSizeImage; idx += 3) {	   //将BGR转换为RGB
		temp = bmpImage[idx];
		bmpImage[idx] = bmpImage[idx + 2];
		bmpImage[idx + 2] = temp;
	}

	fclose(filePtr);
	return bmpImage;
}

//加载纹理
void loadTex(GLuint* texture)
{
	char filenames[6][20];
	sprintf(filenames[0], "wave.bmp");
	sprintf(filenames[1], "top.bmp");
	sprintf(filenames[2], "left.bmp");
	sprintf(filenames[3], "front.bmp");
	sprintf(filenames[4], "right.bmp");
	sprintf(filenames[5], "back.bmp");
	for (int i = 0; i < 6; i++) {

		BITMAPINFOHEADER bmpInfoHeader;
		unsigned char* bmpData = LoadBmpFile(filenames[i], &bmpInfoHeader);
		glBindTexture(GL_TEXTURE_2D, texture[i]);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D,
			0,				//纹理单元层次，这里不是mipmap，取0
			GL_RGB,			//纹理单元中数据格式
			bmpInfoHeader.biWidth,		//纹理宽度
			bmpInfoHeader.biHeight,		//纹理高度
			0,			//指定纹理单元的边框，如果包含边框取值为1，不包含边框取值为0   
			GL_RGB,     //加载图片的数据格式
			GL_UNSIGNED_BYTE,		//每个颜色的数据类型
			bmpData);    //纹理图片数据
	}

	
}
