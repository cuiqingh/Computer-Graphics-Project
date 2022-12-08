# Computer-Graphics-Project
计算机图形学大作业
### 最终效果如下
<img src="https://github.com/cuiqingh/Computer-Graphics-Project/blob/main/img/%E6%9C%80%E5%90%8E%E6%95%88%E6%9E%9C.gif" width="30%">

---------

### 目录
* <a href="#water">水面模拟</a>
* <a href="#texture">纹理映射</a>
* <a href="#light">添加光照</a>
* <a href="#run">程序运行</a>

----------

### <a name="water">水面模拟</a>
参考[[图形学] 实时流体模拟中的数学方法](https://blog.csdn.net/zju_fish1996/article/details/52293782)一文，二维波方程可以表示为:

$\frac{\partial ^{2} z}{\partial t^{2}} = c^{2}\left ( \frac{\partial ^{2} z}{\partial x^{2}}+\frac{\partial ^{2} z}{\partial y^{2}} \right ) - \mu \frac{\partial z}{\partial t}$

设有一个矩形区域（水面），定义 $z(i,j,k)$ 为该区域 $(i,j)$ 坐标在 $k$ 时间的位移（即高度）。已知当前时刻 $z(i,j,k)$ 和前一时刻 $z(i,j,k-1)$ 位移，可以用
如下公式求下一时刻位移:

$z(i,j,k+1)=\frac{4-8c^{2}t^{2}/d^{2}}{\mu t +2}z(i,j,k)+\frac{\mu t-2}{\mu t+2}z(i,j,k-1)+\frac{2c^{2}t^{2}/d^{2}}{\mu t+2} \left [ z(i+1,j,k)+z(i-1,j,k)+z(i,j+1.k)+z(i,j-1,k) \right ] $

本项目中不考虑阻力系数，并将待绘制水面看做一个个小三角面元，如下图

<img src="https://github.com/cuiqingh/Computer-Graphics-Project/blob/main/img/%E7%BD%91%E6%A0%BC.png" width="25%">

区域的长度（横向）为 _this->length_，宽度（纵向）为 _this->width_,
然后设置两个缓冲区(buffer[0]、buffer[1])，每个缓冲区分别保存当前时刻和上一时刻的所有顶点坐标，大小为 (this->width + 1) \* (this->length + 1)

代码部分如下:
```C++
//初始化波动方程参数
float f1 = speed * speed * time * time / (size * size);
this->k1 = 2.0f - 4.0f * f1; 
this->k2 = - 1.0f;
this->k3 = f1;

//根据此刻和上一时刻的数据求出下一次刷新所有定点的位置
for (long i = 1; i < this->width; i++)
{
  Point3D* now = buffer[render] + i * this->length;     //now为当前时刻顶点信息，pre为上一时刻
  Point3D* pre = buffer[1 - render] + i * this->length;
  for (long j = 1; j < this->length; j++)
  {
    pre[j].z = k1 * now[j].z + k2 * pre[j].z +         //使用波动方程计算下一时刻坐标信息
      k3 * (now[j + 1].z + now[j - 1].z + (now + 1)[j].z
        + (now - 1)[j].z);
  }
}
//交换缓冲区
render = 1 - render;
```

在主程序中，以上计算将每隔一段时间进行一次，以达到动态效果，运行结果如下图:

<img src="https://github.com/cuiqingh/Computer-Graphics-Project/blob/main/img/%E7%BD%91%E6%A0%BC%E7%BA%BF%E5%9B%BE.gif" width="30%">

### <a name="texture">纹理映射</a>
这一部分主要将纹理贴图应用到水面的网格上，
其中读取bmp图片的函数(teture.h -> LoadBmpFile)是自定义的，也可以直接调用其他库中的函数，
opengl加载纹理主要用到下面几个函数:

```C++
glEnable(GL_TEXTURE_2D);            //启动纹理
glGenTextures(1, texture);                //加载一个纹理对象到texture
glBindTexture(GL_TEXTURE_2D, texture);    //创建或使用一个已命名的纹理，这里是绑定一个2D纹理到texture
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);   //设置纹理的一些特性，比如这里设置纹理过滤方式为线性插值
void WINAPI glTexImage2D(           //指定二维纹理图像，即将图像信息绑定到该纹理上，此处纹理对应的是上面最近的绑定纹理
         GLenum  target,
         GLint   level,
         GLint   internalformat,
         GLsizei width,
         GLsizei height,
         GLint   border,
         GLint   format,
         GLenum  type,
   const GLvoid  *pixels
);
glTexCoord2f(GLfloat s,GLfloat t);    //后面跟glVertex3f, 将纹理图上的坐标到网格上的坐标做一个映射
glDisable(GL_TEXTURE_2D);           //关闭纹理
```

代码部分主要实现将纹理图正确映射到网格区域中,在Fluid类中设置 indices[3][faceNum] 和 texcoords[3][2][faceNum],前者保存从面到点的索引，后者存放
从网格到纹理图片的坐标映射，算法部分如下:
```C++
long idx = 0;
float wUnit = 1.0f / this->width;           //纹理图片的大小固定为1×1
float lUnit = 1.0f / this->length;

for (int i = 0; i < this->width; i++) {
  for (int j = 0; j < this->length; j++, idx++) {
    //下三角面片
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
    //上三角面片
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
```
texcoords下标对应三角面元的位置如下图:

<img src="https://github.com/cuiqingh/Computer-Graphics-Project/blob/main/img/%E4%B8%8B%E4%B8%89%E8%A7%92.png" width="20%">
<img src="https://github.com/cuiqingh/Computer-Graphics-Project/blob/main/img/%E4%B8%8A%E4%B8%89%E8%A7%92.png" width="20%">

增加纹理后的效果如下:

<img src="https://github.com/cuiqingh/Computer-Graphics-Project/blob/main/img/%E6%B7%BB%E5%8A%A0%E7%BA%B9%E7%90%86.gif" width="30%">

然后调节波动方程的参数以美化绘制结果，并在水面四周添加五个背景纹理，做成虚拟的天空盒，
同时改写键盘回调函数，能通过按键控制视角的移动，这里主要使用glLookAt函数，需要注意每个参数的效果:
```C++
//a和d控制视角左右移动，w和s控制视角上下移动，1和2控制上下高度
void keyBoard(unsigned char key, int x, int y) {
	switch (key)
	{
	case 'a':
	case 'A':
		angleXy += 2.0; break;
	case 'd':
	case 'D':
		angleXy -= 2.0; break;
	case 'w':
	case 'W':
		angleXz += 2.0; break;
	case 's':
	case 'S':
		angleXz -= 2.0; break;
	case '1':
		z += (float)sin(PI * angleXz / 180.0f) * 1.0;
		break;
	case '2':
		z -= (float)sin(PI * angleXz / 180.0f) * 1.0;
	}
	
	glutPostRedisplay();
}

gluLookAt(x, y, z, float(x + 100 * cos(PI * angleXz / 180.0f)),
    float(y + 100 * sin(PI * angleXy / 180.0f)),
    float(z + 100 * sin(PI * angleXz / 180.0f)), 0.0f, 0.0f, 1.0f);
```

效果如下:

<img src="https://github.com/cuiqingh/Computer-Graphics-Project/blob/main/img/%E5%A4%A9%E7%A9%BA%E7%9B%92.gif" width="30%">

(这里可以看到水面还是有明显的边界感，因为此前的版本纹理映射算法有些小bug，在之后版本已经调整好了)

### <a name="light">添加光照</a>
