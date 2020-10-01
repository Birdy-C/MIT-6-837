在之前的项目中加入二次光照——现在Ray Caster就算升级为Ray Tracer了！  

这次实验完成之后模型里就有简单的阴影、反射和折射了。

# assignment 4

## Log
感觉这次的题特别多——

那个cutoff weight我最后没有把它加上，写的时候没有考虑到。应该是对于不重要的光线就不继续进行迭代计算了。

增加transparent_shadows。这个值相当于一个mask，之前是如果有遇到物体就设置为0，不遇到物体是1，然后乘以该光源的颜色。现在变成在0-1之间变换，由引向光源的射线穿透物体的透明属性决定。 
输出了一下在这次的数据中T的差不多在，0.8以内。
觉得算power太麻烦了直接定为线性衰减。T为1的时候，mask和该物体的透明颜色一样。然后看起来和图蛮像的……
![my result](https://raw.githubusercontent.com/birdy-C/pic/master/MIT-6-837/4/output4_06f_trans.png) 

比较丧的一点是，那个钻石的例子我到最后也没有调对，不知道哪里出了问题……或许等做后面的题目了就知道了……摊手
前一个例子，后一个是我的结果。
![example](https://raw.githubusercontent.com/birdy-C/pic/master/MIT-6-837/4/output4_14fr.png) 
![my result](https://raw.githubusercontent.com/birdy-C/pic/master/MIT-6-837/4/output4_14fw.png) 
一个问题是过亮，一个问题是偏红色。
如果在发出的射线不和任何物体相交的时候，返回0，那么这里的钻石不会过量，但是前面的一个例子能明显的感觉到不对。
然后偏红色那个问题，我脑补了一下觉得蛮对的……


## 模型

这里一边重新介绍了一下光栅化的方法。包括线和圆，以及确定线段围成的封闭图形。
颜色的插值使用重心坐标系。
可见性检测的方法——光线投射、画家算法、Z Buffer。Z Buffer 常取倒数。

提了软影，反走样，Glossy Reflection（有多条反射光线），运动模糊，景深。
提到了各种各样的加速结构。包围盒，空间网格化（固定网格，自适应网格（四/八叉树），BVH）。
最后提到了把变换合并。










