# GeneralGraphicsFormat
A general graphics format for hd-map.


# 设计原则：
1.	本文件面向高精地图矢量数据的表达，包括读写和存储；
2.	图形文件包括四大类，点（points）、线段（lines）、多线段（polylines）、多边形（polygons）；
3.	四大类要素通过点ID展示其几何结构（geometry），所用到的点用一个点列表(points_list)单独存储；
4.	点可以为空间直角坐标系（Spatial Rectangular Coordinate System，SRCS，分别给定XYZ）或WGS84坐标系（WGS84，分别给定经纬高，deg, deg, meter）；
5.	当坐标系为WGS84时，要素的显示将转换到以第一个点所对应的ENU坐标系下显示；
6.	要素以表（table）为单元单独存储，每个表中的要素具有相同的类型和数据结构，每个表有表名，同一类要素的表名不重复；
7.	每类要素可以有多个table；
8.	一个table包括fields_info和data两部分信息
    1) fields info定义与PCL相兼容
    2) data包含三部分内容，依次分别为属性（fields）,几何（geometry）和描述（description）
       - fields部分按fields_info的定义依次存储
       - geometry部分为geometry_size（包含的点个数）和 id_list，其中points和lines的geometry_size省略不写
       - description 为字符串，description可以为空，description用大括号括起来；
9.	信息以行为单位存储，每一行前面都有ID号，以方便人工读取，同时在读取中用于验证；
10.	字段间以空格分隔；
11.	允许出现空行；
12.	允许写注释，注释只能以整行形式存在，注释以 #开头；
13.	支持的数据类型  
![image](https://user-images.githubusercontent.com/40223484/194247274-532eda8d-c6ac-49dd-aef8-f591fd70d1fb.png)
14.	文件后缀：ghs；
15.	Magic Number: GeneralGraphicFormat


# 样例文件
```
GeneralGraphicFormat
#author: xu shengpan
#date: 2022/8/27
#an example file

point_list SRCS 14
0   0   0   0
1  10  0   0
2  20  0   0
3  50  0   0
4  100  0   0
5  0    20  0
6  0    50  0
7  0    100  0
8  0    0  30
9  0    0  50
10  0    0  100
11  100   100  100
12  50  50  50
13  80  80  0

points Vertices
fields_info   1
0  type   uint32
data  2
0   0    0     {origin}
1   0    12    {some point}

lines Axis
fields_info 1
0  label  uint32
data 3
0  1   0   4     {x_axis}
1  1   0   7     {y_axis}
2  1   0   10   {z_axis}

polygons a_polygon_example
fields_info   1
0   type   uint32
data 1
0 2 3 2 6 7  {a polygon with 3 points}
```
