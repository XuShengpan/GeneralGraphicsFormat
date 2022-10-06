# GeneralGraphicsFormat
A general graphics format for hd-map.

设计原则：
1.	本文件面向高精地图矢量数据的表达，包括读写和存储；
2.	图形文件包括四大类，点（points）、线段（lines）、多线段（polylines）、多边形（polygons）；
3.	四大类要素通过点ID展示其几何结构（geometry），所用到的点用一个点列表(points_list)单独存储；
4.	点可以为空间直角坐标系（Spatial Rectangular Coordinate System，SRCS，分别给定XYZ）或WGS84坐标系（分别给定经纬高，deg, deg, meter）；
5.	当坐标系为WGS84时，要素的显示将转换到以第一个点所对应的ENU坐标系下显示；
6.	要素以表（table）为单元单独存储，每个表中的要素具有相同的类型和数据结构，每个表有表名，同一类要素的表名不重复；
7.	每类要素可以有多个table；
8.	一个table包括fields_info和data两部分信息
1)	fields info定义与PCL相兼容
2)	data包含三部分内容，依次分别为属性（fields）,几何（geometry）和描述（description）
    - fields部分按fields_info的定义依次存储
    - geometry部分为geometry_size（包含的点个数）和 id_list，其中points和lines的geometry_size省略不写
    - description 为字符串，description可以为空，description用大括号括起来；
9.	信息以行为单位存储，每一行前面都有ID号，以方便人工读取，同时在读取中用于验证；
10.	字段间以空格分隔；
11.	允许出现空行；
12.	允许写注释，注释只能以整行形式存在，注释以 #开头；
13.	支持的数据类型
    ![image](https://user-images.githubusercontent.com/40223484/194245636-81c3be6c-0530-4f09-9dcd-3b4377242f54.png)

14.	文件后缀：ghs；
15.	Magic Number: GeneralGraphicFormat

