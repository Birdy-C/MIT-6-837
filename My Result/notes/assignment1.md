简单的球的Ray Casting。 

是一个大的项目的基础部分，所以起整体的框架起了很久。  

感觉结构清楚的代码看起来很舒服。  

# assignment 1

从这个项目来说涉及的知识点不多，Ray Casting和圆与射线相交的检测。  

但是课件上的点还是蛮多的……

这个作业主要花时间的是把C++快忘光的以及基本自己没怎么实现过的部分部分重新熟悉了一下，还有就是目测后面的代码会在这个的基础上进一步构造所以感觉会比较复杂。


## 代码

### Ray Casting  
```
For every pixel
	Construct a ray from the eye
	For every object in the scene
		Find intersection with the ray
		Keep if closest
	Shade depending on light and normal vector
```
## 一些模型
### 光线的显式定义

```math
P(t) = R_0 + t*R_d
```

### 平面的隐式定义
```math
H(P) = Ax+By+Cz  

=nP+D=0
```

#### 平面与光线求交  
![](https://raw.githubusercontent.com/birdy-C/pic/master/MIT-6-837/1/1.png)  

https://en.wikipedia.org/wiki/Line%E2%80%93plane_intersection



### 球面的隐式定义  
```math
H(P) = P*P-r^2 = 0
```
这里中点在坐标轴原点  

需要考虑到视点是否在球面内
#### 代数求交   
![](https://raw.githubusercontent.com/birdy-C/pic/master/MIT-6-837/1/2.png)

#### 几何求交以及两种方法比较

![](https://raw.githubusercontent.com/birdy-C/pic/master/MIT-6-837/1/3.png)

## 盒子  
![](https://raw.githubusercontent.com/birdy-C/pic/master/MIT-6-837/1/5.png)

## 多边形
先与平面求交，再求这个点是不是在多边形内。

点在多边形内常用检测  

https://en.wikipedia.org/wiki/Point_in_polygon

## 三角形
三角形也可以采用重心的格式。这里提出了一种重心坐标下的表示方法。  

看起来介绍的是下面这种复杂的算法
https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm  
https://lisabug.github.io/2015/05/01/Ray-tracer-triangle-intersection/

## 其他
显式曲面&隐式曲面  

前者可以方便的生成点，后者可以方便的检测一个点是不是在曲面上  

![](https://raw.githubusercontent.com/birdy-C/pic/master/MIT-6-837/1/4.png)


