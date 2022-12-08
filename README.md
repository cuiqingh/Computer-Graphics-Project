# Computer-Graphics-Project
计算机图形学大作业
### 最终效果如下
<img src="https://github.com/cuiqingh/Computer-Graphics-Project/blob/main/img/%E6%9C%80%E5%90%8E%E6%95%88%E6%9E%9C.gif" width="30%">

---------

### 目录
* <a href="#water">水面模拟</a>
* <a href="#texture">纹理映射</a>
* <a href="#light">实现光照</a>
* <a href="#run">程序运行</a>

----------

### <a name="water">水面模拟</a>
参考[[图形学] 实时流体模拟中的数学方法](https://blog.csdn.net/zju_fish1996/article/details/52293782)一文，二维波方程可以表示为:

$\frac{\partial ^{2} z}{\partial t^{2}} = c^{2}\left ( \frac{\partial ^{2} z}{\partial x^{2}}+\frac{\partial ^{2} z}{\partial y^{2}} \right ) - \mu \frac{\partial z}{\partial t}$

设有一个矩形区域（水面），定义 $z(i,j,k)$ 为该区域 $(i,j)$ 坐标在 $k$ 时间的位移（即高度）。已知当前时刻 $z(i,j,k)$ 和前一时刻 $z(i,j,k-1)$ 位移，可以用
如下公式求下一时刻位移:

$z(i,j,k+1)=\frac{4-8c^{2}t^{2}/d^{2}}{\mu t +2}z(i,j,k)+\frac{\mu t-2}{\mu t+2}z(i,j,k-1)+\frac{2c^{2}t^{2}/d^{2}}{\mu t+2} \left [ z(i+1,j,k)+z(i-1,j,k)+z(i,j+1.k)+z(i,j-1,k) \right ] $

本项目中不考虑阻力系数，并将待绘制水面看做一个个小三角面元，如下图

<img src="https://github.com/cuiqingh/Computer-Graphics-Project/blob/main/img/%E7%BD%91%E6%A0%BC.png" width="20%">

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
