
接下来这两个实验是要是用网格去加速光线跟踪的算法，这个实验里面实现网格化，下一个实验实现加速。



# assignment 5

- Bounding boxes for primitives
- Sphere voxelization
- Regular grid data structure
- Fast ray-grid intersection
- Flatten the transformation hierarchy

## Log

拖得太久了再次看代码仿佛不是我儿子……

- 我一开始的结果好像比他的颜色要深一点，而且背后两个面是黑色的。后来处理是设置了一下Grid的Material。
  - `itsMaterial = new PhongMaterial(Vec3f(1), Vec3f(0), 0);`
  - 并且在绘制之前调用
  - `glSetMaterial();`
  - 但是最后得到的结果有几个图颜色不对，我怀疑题目有问题。

- 3DDDA卡了很久。可以参考下面这个链接。
  - https://www.shadertoy.com/view/4dfGzs
  - 虽然我大概能理解在干什么，但是具体实现的时候出现了非常多问题。一开始写作业的时候卡在这一步然后就没有把整个作业写下去orz
- BoundingBox的transform

## 课程笔记

Texture Mapping， 主要就是和纹理相关的内容。如何在透视下保持纹理映射正确、过程式纹理、常见的纹理映射方法（投射、mapping），使用Displacement Mapping、Bump Mapping、Environment Mapping来得到更加逼真的纹理凹凸效果。

Linear Interpolation into Screen Space 也是很重要的一个点，虽然因为作业实现的是raytracing所以不需要具体实现。

Shadows，介绍了ray tracing中的软阴影，Projective Texture Shadow，Shadow Maps以及Shadow Volumes。












