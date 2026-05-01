# 核心原则    
PCDViewer 5.4.0 基于图元（Geomtry）和节点（Node）组织场景数据，通过节点构成树形结构，各节点可以有自己的名称（Name）和位姿（Transformation），以实现丰富的场景表示。

# 图元    
设计了五种图元，各图元有包括颜色、大小、绘制方式等属性。   
----------------------------------------------   
| 图元（Geomtry） |	属性（Attributes）       |   
----------------------------------------------   
| 点（Points）	  | uchar color[4];          |   
|                 | int point_size{ 1 };     |   
----------------------------------------------   
| 线段（Lines）	  | uchar color[4];          |   
|                 | int    line_width{ 1 };  |   
|                 | bool  dash{ false };     |   
----------------------------------------------   
| 三角形          | uchar color[4];          |   
|（Triangles）	  | FaceMode mode;           |   
----------------------------------------------   
| 多线段          | uchar color[4];          |   
|（Polyline）     | int   line_width{ 1 };   |   
|                 | bool  dash{ false };     |   
----------------------------------------------   
| 多线段          | uchar color[4];          |           
|（Polyline）	  | int   line_width{ 1 };   |   
|                 | bool  dash{ false };     |   
----------------------------------------------   
| 多边形          | uchar color[4];          |   
|（Polygon）	  | FaceMode mode;           |   
----------------------------------------------   

FaceMode 包括点、线框模型、填充   

# 节点   
一个节点可以包含`0~N`个图元和`0~n`个子节点。   

~
    struct Transformation
    {
        Eigen::Quaterniond R{ 1, 0, 0, 0 };
        Eigen::Vector3d t{ 0, 0, 0 };
    };

    struct GraphicsNode
    {
        std::string  name;
        Transformation T;  //pose in its parent node's coordination system.
        std::vector<GeometryBasePtr> geomtries;

        std::vector<std::shared_ptr<GraphicsNode> > children;
    };
~
