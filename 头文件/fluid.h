#pragma once 
#include <stdlib.h>
#include "GL/GLUT.H" 
#include "vec3d.h"

class Fluid
{		//��������һ��ˮ������
private:

	long length;		//ˮ��ĳ���
	long width;			//ˮ��Ŀ��
	long pointNum;
	long faceNum;

	Point3D* buffer[2];		//�����������������ж����������Ϣ
	int render;		//��ǰ������

	Vector3D* faceNormal;		//ÿ��������Ԫ����һ��������
	Vector3D* vertexNormal;		//����ÿ������ķ�����

	int* indices[3];	//�洢ÿ����������Ƭ�Ķ����ڻ������е�����
	float* texcoords[3][2];		//��Ƭ����������������ͼƬ�е����꣨��ʱ����ͼƬ�ɿ���1��1��
	float k1, k2, k3;	//����ʽϵ��
	int texture;

public:

	Fluid(long length, long width, float size, float time, float speed, int texture) {
		/// <summary>
		/// ��ʼ��һ��������
		/// </summary>
		/// <param name="length">�������򳤶�</param>
		/// <param name="width">����������</param>
		/// <param name="size">��Ԫ����Ĵ�С</param>
		/// <param name="time">ʱ�䵥Ԫ</param>
		/// <param name="speed">����</param>
		/// <param name="texture">����</param>

		this->texture = texture;

		this->length = length;
		this->width = width;
		this->pointNum = (width + 1) * (length + 1);
		this->faceNum = 2 * length * width;

		this->render = 0;
		buffer[0] = new Point3D[this->pointNum];
		buffer[1] = new Point3D[this->pointNum];

		vertexNormal = new Vector3D[this->pointNum];
		faceNormal = new Vector3D[this->faceNum];

		for (int i = 0; i < 3; i++)
			indices[i] = new int[this->faceNum];

		for (int i = 0; i < 6; i++)
			texcoords[i % 3][i / 3] = new float[this->faceNum];

		//������淽�̵Ķ���ʽϵ��
		float f1 = speed * speed * time * time / (size * size);
		k1 = 2.0F - 4.0F * f1;
		k2 = - 1.0f;
		k3 = f1;

		//��ʼ��ˮ�����������ÿ������ĸ߶�
		long idx = 0;
		for (long i = 0; i < this->width + 1; i++)
		{
			float w = size * i;
			for (long j = 0; j < this->length + 1; j++, idx++)
			{
				buffer[0][idx].setx(size * j);
				buffer[0][idx].sety(w);
				if (i == 0 || j == 0 || j == this->length || i == this->width) {
					buffer[0][idx].setz(0.0f);
				} else {
					int r = rand() % 5;
					if (r == 0) buffer[0][idx].setz(buffer[0][idx - 1].z + 0.5f * (rand() % 10 / 10.0f));
					else if(r == 1) buffer[0][idx].setz(buffer[0][idx - 1].z - 0.5f * (rand() % 10 / 10.0f));
					else if(r == 2) buffer[0][idx].setz(buffer[0][idx - this->length - 1].z + 0.5f * (rand() % 10 / 10.0f));
					else if(r == 3) buffer[0][idx].setz(buffer[0][idx - this->length - 1].z - 0.5f * (rand() % 10 / 10.0f));
					else buffer[0][idx].setz(0.0f);
				}
				buffer[1][idx] = buffer[0][idx];
			}
		}

		idx = 0;		//��ǰ��Ԫ������
		float wUnit = 1.0f / this->width;
		float lUnit = 1.0f / this->length;

		for (int i = 0; i < this->width; i++) {
			for (int j = 0; j < this->length; j++, idx++) {
				//��������Ƭ
				long t = i * (this->length + 1) + j;

				indices[0][idx] = t;
				indices[1][idx] = t + 1;
				indices[2][idx] = t + this->length + 1;

				texcoords[0][0][idx] = j * lUnit;
				texcoords[0][1][idx] = i * wUnit;
				texcoords[1][0][idx] = (j + 1) * lUnit;
				texcoords[1][1][idx] = i * wUnit;
				texcoords[2][0][idx] = j * lUnit;
				texcoords[2][1][idx] = (i + 1) * wUnit;
			}
		}

		for (int i = 1; i < this->width + 1; i++) {
			for (int j = 1; j < this->length + 1; j++, idx++) {
				//��������Ƭ
				long t = i * (this->length + 1) + j;

				indices[0][idx] = t;
				indices[1][idx] = t - 1;
				indices[2][idx] = t - this->length - 1;

				texcoords[0][0][idx] = j * lUnit;
				texcoords[0][1][idx] = i * wUnit;
				texcoords[1][0][idx] = (j - 1) * lUnit;
				texcoords[1][1][idx] = i * wUnit;
				texcoords[2][0][idx] = j * lUnit;
				texcoords[2][1][idx] = (i - 1) * wUnit;
			}
		}

		computeNormal();

	};

	~Fluid() {
		delete[] faceNormal;
		delete[] vertexNormal;
		delete[] buffer[1];
		delete[] buffer[0];
	};

	void computeNormal() {
		//��������������Ԫ�ķ�����
		long idx = 0;
		for (int i = 0; i < this->width; i++) {
			for (int j = 0; j < this->length; j++, idx++) {

				long t = i * (this->length + 1) + j;
				int i_0 = t;
				int i_1 = t + 1;
				int i_2 = t + this->length + 1;

				Vector3D z_o(buffer[0][i_1].x - buffer[0][i_0].x, buffer[0][i_1].y - buffer[0][i_0].y, buffer[0][i_1].z - buffer[0][i_0].z);
				Vector3D z_t(buffer[0][i_2].x - buffer[0][i_0].x, buffer[0][i_2].y - buffer[0][i_0].y, buffer[0][i_2].z - buffer[0][i_0].z);
				faceNormal[idx].cross(z_o, z_t).normalize();

			}
		}
		for (int i = 1; i < this->width + 1 ; i++) {
			for (int j = 1; j < this->length + 1; j++, idx++) {

				long t = i * (this->length + 1) + j;
				int i_0 = t;
				int i_1 = t + 1;
				int i_2 = t + this->length + 1;

				Vector3D z_o(buffer[0][i_1].x - buffer[0][i_0].x, buffer[0][i_1].y - buffer[0][i_0].y, buffer[0][i_1].z - buffer[0][i_0].z);
				Vector3D z_t(buffer[0][i_2].x - buffer[0][i_0].x, buffer[0][i_2].y - buffer[0][i_0].y, buffer[0][i_2].z - buffer[0][i_0].z);
				faceNormal[idx].cross(z_o, z_t).normalize();

			}
		}
		//�������ж���ķ�����
		long triangles = this->width * this->length;
		idx = 0;
		for (int i = 0; i < this->width + 1; i++) {
			for (int j = 0; j < this->length + 1; j++, idx++) {
				if (i == 0) {
					//��һ��
					if (j == 0) {
						vertexNormal[idx] = faceNormal[0];
					}
					else if (j == this->length) {
						vertexNormal[idx] = (faceNormal[idx - 1] + faceNormal[triangles + idx - 1]).normalize();
					}
					else {
						vertexNormal[idx] = (faceNormal[idx - 1] + faceNormal[triangles + idx - 1] + 
							faceNormal[idx]).normalize();
					}
				}
				else if (i < this->width) {
					//�м���
					long base = this->length;
					if (j == 0) {
						vertexNormal[idx] = (faceNormal[i * base] + faceNormal[(i - 1) * base] + 
							faceNormal[(i - 1) * base + triangles]).normalize();
					}
					else if (j == this->length) {
						vertexNormal[idx] = (faceNormal[(i + 1) * base - 1] + faceNormal[(i + 1) * base + triangles - 1] +
							faceNormal[i * base + triangles - 1]).normalize();
					}
					else {
						vertexNormal[idx] = (faceNormal[i * base + j - 1] + faceNormal[i * base + j + triangles - 1] +
							faceNormal[i * base + j] + faceNormal[(i - 1) * base + j + triangles - 1] +
							faceNormal[(i - 1) * base + j] + faceNormal[(i - 1) * base + j + triangles]).normalize();
					}
				}
				else {
					//�����һ��
					long base = this->length;
					if (j == 0) {
						vertexNormal[idx] = (faceNormal[(i - 1) * base] + faceNormal[(i - 1) * base + 1]).normalize();
					}
					else if (j == this->length) {
						vertexNormal[idx] = faceNormal[i * base - 1];
					}
					else {
						vertexNormal[idx] = (faceNormal[(i - 1) * base + j + triangles - 1] + faceNormal[(i - 1) * base + j] +
							faceNormal[(i - 1) * base + j + triangles]).normalize();
					}
				}
			}
		}
	}

	void Evaluate(void) {

		//����������淽�̣����ﲻ��������ϵ�����������߽��趨
		for (long i = 1; i < this->width; i++)
		{
			Point3D* now = buffer[render] + i * this->length;
			Point3D* pre = buffer[1 - render] + i * this->length;
			for (long j = 1; j < this->length; j++)
			{
				pre[j].z = k1 * now[j].z + k2 * pre[j].z +
					k3 * (now[j + 1].z + now[j - 1].z + (now + 1)[j].z
						+ (now - 1)[j].z);
			}
		}

		//����������
		render = 1 - render;

		computeNormal();

	}

	void draw()
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture);  
		glBegin(GL_TRIANGLES);

		for (int i = 0; i < this->faceNum; i++) {

			glTexCoord2f(texcoords[0][0][i], texcoords[0][1][i]);
			glNormal3f(vertexNormal[indices[0][i]].x, 
				vertexNormal[indices[0][i]].y,
				vertexNormal[indices[0][i]].z);
			glVertex3f(buffer[render][indices[0][i]].x,
				buffer[render][indices[0][i]].y,
				buffer[render][indices[0][i]].z);

			glTexCoord2f(texcoords[1][0][i], texcoords[1][1][i]);
			glNormal3f(vertexNormal[indices[1][i]].x,
				vertexNormal[indices[1][i]].y,
				vertexNormal[indices[1][i]].z);
			glVertex3f(buffer[render][indices[1][i]].x,
				buffer[render][indices[1][i]].y,
				buffer[render][indices[1][i]].z);

			glTexCoord2f(texcoords[2][0][i], texcoords[2][1][i]);
			glNormal3f(vertexNormal[indices[2][i]].x,
				vertexNormal[indices[2][i]].y,
				vertexNormal[indices[2][i]].z);
			glVertex3f(buffer[render][indices[2][i]].x,
				buffer[render][indices[2][i]].y,
				buffer[render][indices[2][i]].z);

		}

		glEnd();
		glDisable(GL_TEXTURE_2D);
	};
};