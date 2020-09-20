这次的作业主要涉及分形算法[Chaos game版本]

# assignment 0
https://github.com/birdy-C/MIT-6-837/tree/master/Result/assignment0

这个assignment大概就是了解了一下分形相关的知识。之前对于分形的认知只有递归调用输出一种。（也就是wiki里的Shrinking and duplication）
https://en.wikipedia.org/wiki/Sierpinski_triangle
可以了解一下Chaos game之类的实现方法



这个作业里的算法和确定性的方法感觉有点类似ray tracing和path tracing的区别。


## 代码
主要逻辑（不是我写的）

```
   for "lots" of random points (x0, y0)
       for k=0 to num_iters 
           pick a random transform fi
           (xk+1, yk+1) = fi(xk, yk)
       display a dot at (xk, yk)
```

然后输入 
这是那个sierpinski_triangle的

```
3
0.33
0.500000 0.000000 0.000000 
0.000000 0.500000 0.000000 
0.000000 0.000000 1.000000 
0.33 
0.500000 0.000000 0.500000 
0.000000 0.500000 0.000000 
0.000000 0.000000 1.000000 
0.34 
0.500000 0.000000 0.000000 
0.000000 0.500000 0.500000 
0.000000 0.000000 1.000000 
```
所以大概思路就是


```
   for "lots" of random points (x0, y0)//x0 y0在[0,1]之间
       //生成齐次坐标下的矩阵N
       for k=0 to num_iters 
           pick a random transform fi
           (x_(k+1), y_(k+1)) = fi(x_k, y_k)//随机选择一个矩阵，然后用N*M继续下一次迭代
       display a dot at (x_num_iters, y_num_iters)//齐次坐标转到正常坐标
```

## More
### figure out the probabilities
如果是要一个概率可视化的话，在绘图的时候，现在的是把白格子变成黑色，可以试试减掉（或者乘以）一个0-1的小数。这样最后的结果越黑代表概率越高。  

### determine the bounding box
用边界点确定。感觉其实这个就是传统的Shrinking and duplication的方式。

Shrinking and duplication 
![Shrinking and duplication](https://upload.wikimedia.org/wikipedia/commons/thumb/c/c9/Sierpinski_triangle_evolution_square.svg/512px-Sierpinski_triangle_evolution_square.svg.png)

chaos game 
![chaos game](https://upload.wikimedia.org/wikipedia/commons/thumb/b/b8/Tri%C3%A2ngulo_de_Sierpinski.gif/200px-Tri%C3%A2ngulo_de_Sierpinski.gif)

这么一想的话chaos game和Shrinking and duplication的实现没有什么本质的区别。

- chaos game取随机数会有一定的开销，Shrinking and duplication在计算过程中会随着层数的增加运算量指数上升  

- Shrinking and duplication这种的图肯定比chaos game好看……但或许可以说chaos game 从某种程度上反应了概率的分布。



### change the color scheme
如果是不同的bounding box内颜色不一样的话，可以尝试一下对不同的选择（也就是不同的到达树的叶结点的路径）编码，然后赋予不同的颜色。

### anti-aliasing
在这里实现的时候，直接把float裁成int，可以根据具体float值影响周围的四个格子。
比如落在(100.5，100.5)，之前的做法是直接把它取整到(100,100),设为黑色。现在可以考虑把(100,100)(100,101)(101,100)(101,101)四个各种分别设置为(127,127,127)的灰色。如果一个格子被多个点影响，则取小的(影响大的)。  

### depth-first vs. breadth-first
这个我按Shrinking and duplication来理解。

BF画出来应该就是一级一级的，能够更好的观察过程。但是会比较占内存（因为要存下之前的所有结点）

## 结果
最后实现之后感觉fern的分形真的神奇，因为在看这个图的时候我没想到它的迭代规律那么简单。  

```
4
0.03
0.000000 0.000000 0.500000 
0.000000 0.200000 0.000000 
0.000000 0.000000 1.000000 
0.71
0.817377 0.065530 0.091311 
-0.065530 0.817377 0.232765 
0.000000 0.000000 1.000000 
0.13
0.108707 -0.372816 0.445646 
0.279612 0.144943 -0.059806 
0.000000 0.000000 1.000000 
0.13
-0.080250 0.385423 0.540125 
0.289067 0.107000 -0.004534 
0.000000 0.000000 1.000000 
```
虽然看起来非常的复杂，但其实并没有更多的变换矩阵。  

![](pic\0fern.tga)

