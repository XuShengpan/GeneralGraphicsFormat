# 核心原则
基于Node-OBJ-Geometry组织数据。Node是一个绘制场景，包含一组待绘制物体（OBJ），每一个待绘制物体由若干图元（Geometry）组成，图元是基本的绘制图形，包括点（Points）、线段（Lines）、多线段（Polyline）、三角形（Triangles）、平面多边形（Polygon）。
图元包含顶点、颜色等属性。

# 样例文件
例如，绘制一个坐标系，里面有一个三角形
```
{
    "Nodes": [
        {
            "Name": "Test Graphics",
            "OBJs": [
                {
                    "Geometries": [
                        {
                            "Lines": {
                                "Color": [
                                    255,
                                    0,
                                    0,
                                    255
                                ],
                                "Coordinates": [
                                    [
                                        -1.0,
                                        0.0,
                                        0.0
                                    ],
                                    [
                                        10.0,
                                        0.0,
                                        0.0
                                    ]
                                ],
                                "Dash": false,
                                "LineWidth": 3,
                                "Name": "X-Axis"
                            }
                        },
                        {
                            "Lines": {
                                "Color": [
                                    0,
                                    255,
                                    0,
                                    255
                                ],
                                "Coordinates": [
                                    [
                                        0.0,
                                        -1.0,
                                        0.0
                                    ],
                                    [
                                        0.0,
                                        10.0,
                                        0.0
                                    ]
                                ],
                                "Dash": false,
                                "LineWidth": 3,
                                "Name": "Y-Axis"
                            }
                        },
                        {
                            "Lines": {
                                "Color": [
                                    0,
                                    0,
                                    255,
                                    255
                                ],
                                "Coordinates": [
                                    [
                                        0.0,
                                        0.0,
                                        -1.0
                                    ],
                                    [
                                        0.0,
                                        0.0,
                                        10.0
                                    ]
                                ],
                                "Dash": false,
                                "LineWidth": 3,
                                "Name": "Z-Axis"
                            }
                        }
                    ],
                    "Name": "Coordinate System"
                },
                {
                    "Geometries": [
                        {
                            "Triangles": {
                                "BackFaceMode": 1,
                                "Color": [
                                    255,
                                    255,
                                    255,
                                    20
                                ],
                                "Coordinates": [
                                    [
                                        -0.5,
                                        0.0,
                                        -0.30000001192092896
                                    ],
                                    [
                                        2.0,
                                        -2.0,
                                        0.0
                                    ],
                                    [
                                        6.0,
                                        5.0,
                                        1.0
                                    ]
                                ],
                                "FrontFaceMode": 2,
                                "Name": "Triangle"
                            }
                        },
                        {
                            "Lines": {
                                "Color": [
                                    0,
                                    255,
                                    255,
                                    255
                                ],
                                "Coordinates": [
                                    [
                                        2.579272508621216,
                                        1.0251351594924927,
                                        -0.25970226526260376
                                    ],
                                    [
                                        2.0243656635284424,
                                        0.8491891026496887,
                                        3.19154691696167
                                    ]
                                ],
                                "Dash": true,
                                "LineWidth": 1,
                                "Name": "Triangle Normal"
                            }
                        }
                    ],
                    "Name": "TriangleObj"
                }
            ]
        }
    ]
}
```
