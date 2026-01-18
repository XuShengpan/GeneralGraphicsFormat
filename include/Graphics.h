#ifndef  PCDVIEWER_GEOMETRY_H
#define PCDVIEWER_GEOMETRY_H

#include <memory>
#include <string>
#include <vector>

namespace pcd_viewer_ns {

    enum class GeometryType
    {
        Points,
        Lines,
        Triangles,
        Polyline,
        Polygon
    };

    enum class PolygonMode
    {
        Point,
        Line,
        Fill
    };

    struct FaceMode
    {
        PolygonMode frontMode{ PolygonMode::Fill};
        PolygonMode backMode{ PolygonMode::Fill };
    };

    struct Point3f
    {
        union
        {
            float data[3];
            float x;
            float y;
            float z;
        };
        float operator[](int i) const { return data[i]; }
        float& operator[](int i)
        {
            return data[i];
        }
    };

	struct GeometryBase
	{
        std::string name;
        std::vector<Point3f> points;
        std::uint8_t color[4]{ 255, 255, 255, 255 };

        void setColor(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a = 255)
        {
            color[0] = r;
            color[1] = g;
            color[2] = b;
            color[3] = a;
        }

        virtual GeometryType getType() = 0;
	};

    struct Points: GeometryBase
    {
        int point_size{ 1 };

        virtual GeometryType getType() { return GeometryType::Points; }
    };

    struct Lines : GeometryBase
    {
        int    line_width{ 1 };
        bool  dash{ false };

        virtual GeometryType getType() { return GeometryType::Lines; }
    };

    struct Polyline : GeometryBase
    {
        int    line_width{ 1 };
        bool  dash{ false };
        virtual GeometryType getType() { return GeometryType::Polyline; }
    };

    struct Triangles : GeometryBase
    {
        FaceMode mode;

        virtual GeometryType getType() { return GeometryType::Triangles; }
    };

    struct Polygon : GeometryBase
    {
        FaceMode mode;

        virtual GeometryType getType() { return GeometryType::Polygon; }
    };

    using GeometryBasePtr = std::shared_ptr<GeometryBase>;

    struct OBJ
    {
        std::string  name;
        std::vector<GeometryBasePtr> geomtries;
    };

    using OBJPtr = std::shared_ptr<OBJ>;

    struct Node
    {
        std::string  name;
        std::vector<OBJPtr> objs;
    };

    using NodePtr = std::shared_ptr<Node>;

    struct GraphicsData
    {
        std::vector<NodePtr> nodes;
    };
}

#endif // !PCDVIEWER_GEOMETRY_H
