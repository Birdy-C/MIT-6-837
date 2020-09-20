在之前的项目中加入OpenGL API来实现场景交互（相机相应鼠标事件）。
实现Phong model。

# assignment 3

这次还好吧比较常规……可能最麻烦的是配OpenGL？ 

OpenGL使用的是固定管线，如果有兴趣可以自己阅读一下[learnopengl](https://learnopengl.com/).

不过这个assignment里，最主要的不是OpenGL的实现。OpenGL只是提供一个交互，以及可以比较一下判断Phong Shading有没有写对。

## Log
- 环境光的材质和漫射光的材质一样。如果计算shader的时候要直接返回颜色的话，还要多传入一个环境光的参数。  

- OpenGL配置……glut.h 与stdlib.h中的exit()重定义问题之类的。
- 文件打不开可以先用fprintf(stderr, "%s \n",strerror(errno));检查路径。  
- 还有这个作业确实有点早……OpenGL居然有不自带画球方法的时候（惊了）  
- 还有就是法线方向了、一开始以为正方体显示的时候几个全黑的面是法线设置反了，后来想了想鼠标动的是相机位置所以对于背向水平入射光的方向是一直保持黑色的。  
- PhongMaterial::Shade这个函数不需要计入环境光，因为可能有多盏灯啊环境光算一次就够了！  
- 可以设置glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, zero);来单独测试每个光线。
- 一开始我写错Phong的种类了，写了原始的形式。normal会比Blinn-Torrance要暗……其他也没啥感觉……
- 最后不知道什么是高光修正？(**specular fix**）

## 模型

这一次的课件是从分析Ray Casting / Tracing 的优缺点出发，引出了Rendering Pipline。  

介绍了渲染管线的整个流程之后，重点介绍了Clipping和Scan Conversion的一些相关算法。 

Ray Casting / Tracing 的优缺点
![](https://raw.githubusercontent.com/birdy-C/pic/master/MIT-6-837/3/0.png) 

渲染管线介绍
![](https://raw.githubusercontent.com/birdy-C/pic/master/MIT-6-837/3/1.png) 
![](https://raw.githubusercontent.com/birdy-C/pic/master/MIT-6-837/3/2.png) 

线段的裁剪，比如Outcodes.
![](https://raw.githubusercontent.com/birdy-C/pic/master/MIT-6-837/3/3.png) 
 Polygon clipping，主要就是Walking rules。
基本的光栅化，比如bresenham's algorithm

以及一些反走样的介绍。
















