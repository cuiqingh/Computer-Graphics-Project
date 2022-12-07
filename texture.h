#pragma once 
#define GLUT_DISABLE_ATEXIT_HACK      
#include "GL/GLUT.H"      
#include <stdio.h> 
 
//��ȡ����ͼƬ���ݣ�BMP��
static unsigned char *LoadBmpFile(char *filename, BITMAPINFOHEADER *bmpInfoHeader)
{    

	FILE* filePtr = fopen(filename, "rb");	//��BMP�ļ�
	if (filePtr == NULL) {		//�ļ��򿪴���
		printf("Texture file open error.\n");
		return NULL;
	}

	BITMAPFILEHEADER bmpFileHeader;
	fread(&bmpFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);	// ����BMP�ļ�ͷ
	if (bmpFileHeader.bfType != 0x4D42) {	// ��֤�Ƿ�Ϊbitmap�ļ�
		
		fprintf(stderr, "The file is not a bitmap file.\n");
		return NULL;
	}

	fread(bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);		// ����BMP��Ϣͷ
	fseek(filePtr, bmpFileHeader.bfOffBits, SEEK_SET);		// ���ļ�ָ������BMP������     
	unsigned char* bmpImage = new unsigned char[bmpInfoHeader->biSizeImage];	//���λͼ����
	if (!bmpImage) {		//�ڴ洴��ʧ��
		fprintf(stderr, "Error in LoadBitmapFile: memory error\n");
		return NULL;
	}
	fread(bmpImage, 1, bmpInfoHeader->biSizeImage, filePtr);

	unsigned char temp;
	for (int idx = 0; idx < bmpInfoHeader->biSizeImage; idx += 3) {	   //��BGRת��ΪRGB
		temp = bmpImage[idx];
		bmpImage[idx] = bmpImage[idx + 2];
		bmpImage[idx + 2] = temp;
	}

	fclose(filePtr);
	return bmpImage;
}

//��������
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
			0,				//����Ԫ��Σ����ﲻ��mipmap��ȡ0
			GL_RGB,			//����Ԫ�����ݸ�ʽ
			bmpInfoHeader.biWidth,		//������
			bmpInfoHeader.biHeight,		//����߶�
			0,			//ָ������Ԫ�ı߿���������߿�ȡֵΪ1���������߿�ȡֵΪ0   
			GL_RGB,     //����ͼƬ�����ݸ�ʽ
			GL_UNSIGNED_BYTE,		//ÿ����ɫ����������
			bmpData);    //����ͼƬ����
	}

	
}
