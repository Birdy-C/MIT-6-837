---
title: assignment2 [Transformations & Additional Primitives]
date: 2018-05-07 11:42:05
categories:   
	- [Tech, MIT-6-837]  
tags:  
	- CG  
---
加入平面和三角形，加入放射变换，加入透视相机。  
对于放射变换的法线可视化。  
以及漫反射光照。


<!-- more -->

# assignment 2

我觉得涉及的知识点都不难但是一个线代不好的人硬生生写的快炸毛了……  
一开始觉得变换都写好了应该不难。但是后面，变换写错之后总是跑到屏幕外面……面对空荡荡的图不知道如何debug……  
然后调出来上一个assignment的很多个问题。  
  
## BUG日志

1. 鱼眼镜头的话插值是对角度进行插值，而正常的透视镜头需要对投影平面上的点。 
2. 透视相机的生成我可能大概或许还是写错了……  
一开始觉得非常的不可理喻（因为那个角度很奇怪），后来发现可能就是比较方便测量吧。
3. raytracer -input scene2_05_inside_sphere.txt -size 200 200 -output output2_05_no_back.tga  
这个测试数据后面生成的应该是有环境光的那个漫反射的颜色的吧，根据之前对$c_{ambient} $的定义来看  
4. 然后还是上面那个图，要么是深度的定义出问题了要么是透视相机的问题，最后的深度图不是非常符合。
5. 平面的法线正负无法确定，用隐式定义的话根本看不出法线啊喂……  
6. 然后是三角形正反面的确定，这里是按照顺时针为正。  
7. 最后是求变换后的矩阵的法向量，如果用齐次坐标的话好似无法确定方向？求出来的法线好几次出现相反的情况。然后最后结果是直接舍弃了第四维……不知道算其他的东西的时候会不会出错

综上所述一堆bug……  

### 实现内容

实现了一下射线和平面求交以及射线和三角形求交，用的是上次那篇笔记里的方法。
一个很重要的是Normal Transformation，法线在仿射空间的变换和正常的不一样。这个例程确实解决了我对Transformation的理解上的一些坎。  
![](https://raw.githubusercontent.com/birdy-C/pic/master/MIT-6-837/2/2.png)  

用齐次坐标表示向量和点之后在计算上有神奇的统一性。  

## 模型
课件的模型除了仿射变换之外，还涉及了大量的光照模型（而在这次的程序里还只实现到漫反射光）  
已经讲到了BRDF，讲到镜面反射和折射的原理。
### 变换
![](https://raw.githubusercontent.com/birdy-C/pic/master/MIT-6-837/2/1.png)  

### 镜面反射
分为Phong和Blinn-Torrance。  
![](https://raw.githubusercontent.com/birdy-C/pic/master/MIT-6-837/2/3.png)  
![](https://raw.githubusercontent.com/birdy-C/pic/master/MIT-6-837/2/4.png)  

### 其他
Anisotropic BRDFs，Off-specular & Retro-reflection

### 增加阴影
这里是用光线跟踪计算的
![](https://raw.githubusercontent.com/birdy-C/pic/master/MIT-6-837/2/5.png)  
缺陷：入射光肯定不是只有一条。