#pragma once 
#include <stdlib.h>
#include "GL/GLUT.H"  

class Point3D
{
public:
	float x, y, z;	//三维坐标

	Point3D& set(float r, float s, float t)
	{
		x = r;
		y = s;
		z = t;
		return (*this);
	}
};

class Fluid
{		//此类生成一个水波区域
private:

	long length;		//水面的长度
	long width;	//水面的宽度
	long pointNum;
	long faceNum;

	Point3D* buffer[2];		//缓冲区，其中有所有定点的坐标信息
	int render;		//当前缓冲区

	int* indices[3];	//存储每个三角形面片的顶点在缓冲区中的索引
	float* texcoords[3][2];		//在纹理图片中的坐标（此时纹理图片可看做1×1）
	float k1, k2, k3;	//多项式系数
	int texture;

public:

	Fluid(long length, long width, float size, float time, float speed, int texture) {
		/// <summary>
		/// 初始化一个流体类
		/// </summary>
		/// <param name="length">流体区域长度</param>
		/// <param name="width">流体区域宽度</param>
		/// <param name="size">单元流体的大小</param>
		/// <param name="time">时间参数</param>
		/// <param name="speed">波速</param>
		/// <param name="texture">纹理</param>
		/// 
		this->texture = texture;

		this->length = length;
		this->width = width;
		this->pointNum = width * length;
		this->faceNum = 2 * (length - 1) * (width - 1);

		this->render = 0;
		buffer[0] = new Point3D[this->pointNum];
		buffer[1] = new Point3D[this->pointNum];

		for (int i = 0; i < 3; i++)
			indices[i] = new int[this->faceNum];

		for (int i = 0; i < 6; i++)
			texcoords[i % 3][i / 3] = new float[this->faceNum];

		//流体表面方程的多项式系数
		float f1 = speed * speed * time * time / (size * size);
		float f2 = 1.0F / 2;
		k1 = (4.0F - 8.0F * f1) * f2;
		k2 = - 2 * f2;
		k3 = 2.0F * f1 * f2;

		long a = 0;
		for (long j = 0; j < this->width; j++)
		{
			float y = size * j;
			for (long i = 0; i < this->length; i++)
			{
				if (i == 0 || j == 0 || i == this->length - 1 || j == this->width - 1) 
					//边缘高度取0
					buffer[0][a].set(size * i, y, 0.0F);
				else {
					//随机赋值
					int r = rand() % 5;
					if (r == 0) buffer[0][a].set(size * i, y, buffer[0][a - 1].z + 0.3f * (rand() % 10 / 10.0f));
					else if(r == 1) buffer[0][a].set(size * i, y, buffer[0][a - 1].z - 0.3f * (rand() % 10 / 10.0f));
					else if(r == 2) buffer[0][a].set(size * i, y, buffer[0][a - this->length].z + 0.3f * (rand() % 10 / 10.0f));
					else if(r == 3) buffer[0][a].set(size * i, y, buffer[0][a - this->length].z - 0.3f * (rand() % 10 / 10.0f));
					else buffer[0][a].set(size * i, y, 0.0f);
				}
				buffer[1][a] = buffer[0][a];
				a++;
			}
		}

		a = 0;		//当前点的索引

		float wUnit = 1.0f / this->width;
		float lUnit = 1.0f / this->length;

		for (int i = 0; i < this->length - 1; i++) {
			for (int j = 0; j < this->width - 1; j++, a++) {
				//下三角面片
				long t = i * this->length + j;
				indices[0][a] = t;
				indices[1][a] = t + 1;
				indices[2][a] = t + this->length;

				texcoords[0][0][a] = i * lUnit;
				texcoords[0][1][a] = j * wUnit;
				texcoords[1][0][a] = (i + 1) * lUnit;
				texcoords[1][1][a] = j * wUnit;
				texcoords[2][0][a] = i * lUnit;
				texcoords[2][1][a] = (j + 1) * wUnit;
			}
		}

		for (int i = 1; i < this->length; i++) {
			for (int j = 1; j < this->width; j++, a++) {
				//上三角面片
				long t = i * this->length + j;
				indices[0][a] = t;
				indices[1][a] = t - 1;
				indices[2][a] = t - this->length;

				texcoords[0][0][a] = i * lUnit;
				texcoords[0][1][a] = j * wUnit;
				texcoords[1][0][a] = (i - 1) * lUnit;
				texcoords[1][1][a] = j * wUnit;
				texcoords[2][0][a] = i * lUnit;
				texcoords[2][1][a] = (j - 1) * wUnit;
			}
		}
	};
	~Fluid() {
		delete[] buffer[1];
		delete[] buffer[0];
	};

	void Evaluate(void) {
		//调用流体表面方程
		for (long j = 1; j < this->length - 1; j++)
		{
			//当前顶点位移
			const Point3D* crnt = buffer[render] + j * this->width;
			//前一顶点位移
			Point3D* prev = buffer[1 - render] + j * this->width;
			for (long i = 1; i < this->width - 1; i++)
			{
				prev[i].z = k1 * crnt[i].z + k2 * prev[i].z +
					k3 * (crnt[i + 1].z + crnt[i - 1].z +
						crnt[i + this->width].z + crnt[i - this->width].z);
			}
		}

		//交换缓冲区
		render = 1 - render;
	}

	void draw()
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture);  
		glBegin(GL_TRIANGLES);	//绘制每个三角形面片

		for (int i = 0; i < this->faceNum; i++) {

			glTexCoord2f(texcoords[0][0][i], texcoords[0][1][i]);
			glVertex3f(buffer[render][indices[0][i]].x,
				buffer[render][indices[0][i]].y,
				buffer[render][indices[0][i]].z);

			glTexCoord2f(texcoords[1][0][i], texcoords[1][1][i]);
			glVertex3f(buffer[render][indices[1][i]].x,
				buffer[render][indices[1][i]].y,
				buffer[render][indices[1][i]].z);

			glTexCoord2f(texcoords[2][0][i], texcoords[2][1][i]);
			glVertex3f(buffer[render][indices[2][i]].x,
				buffer[render][indices[2][i]].y,
				buffer[render][indices[2][i]].z);
		}

		glEnd();
		glDisable(GL_TEXTURE_2D);
	};
};