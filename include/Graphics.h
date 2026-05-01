#ifndef  PCDVIEWER_GEOMETRY_H
#define PCDVIEWER_GEOMETRY_H

#include <memory>
#include <string>
#include <vector>
#include <Eigen/Geometry>

namespace pcd_viewer_ns {

	struct Point3 {

		union {
			struct { float x, y, z; };
			float data[3];
		};
		float& operator[] (size_t i) { return data[i]; }
		const float operator[] (size_t i) const { return data[i]; }

		Point3() {}
		Point3(float x_, float y_, float z_)
		{
			data[0] = x_;
			data[1] = y_;
			data[2] = z_;
		}

		Point3(const Eigen::Vector3d& p)
		{
			data[0] = p[0];
			data[1] = p[1];
			data[2] = p[2];
		}
	};

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

	struct GeometryBase
	{
        GeometryBase(GeometryType type_): type(type_) {}

        std::string name;
        std::vector<Point3> points;
        const GeometryType type;

        bool alpha_enable{ false };

        //r, g, b, a
        std::uint8_t color[4]{ 255, 255, 255, 255 };

        void setColor(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a = 255)
        {
            color[0] = r;
            color[1] = g;
            color[2] = b;
            color[3] = a;
        }

        GeometryType getType() const { return type; }
	};

    struct Points: GeometryBase
    {
        int point_size{ 1 };

        Points(): GeometryBase(GeometryType::Points) {}

    };

    struct Lines : GeometryBase
    {
        int    line_width{ 1 };
        bool  dash{ false };

        Lines() : GeometryBase(GeometryType::Lines) {}
    };

    struct Polyline : GeometryBase
    {
        int    line_width{ 1 };
        bool  dash{ false };
        
        Polyline(): GeometryBase(GeometryType::Polyline) {}
    };

    struct Triangles : GeometryBase
    {
        FaceMode mode;

        Triangles() : GeometryBase(GeometryType::Triangles) {}
    };

    struct Polygon : GeometryBase
    {
        FaceMode mode;

        Polygon() : GeometryBase(GeometryType::Polygon) {}
    };

    using GeometryBasePtr = std::shared_ptr<GeometryBase>;

    struct Transformation
    {
        Eigen::Quaterniond R{ 1, 0, 0, 0 };
        Eigen::Vector3d t{ 0, 0, 0 };

        Transformation() {}
        Transformation(const Eigen::Quaterniond& q_, const Eigen::Vector3d& t_)
        {
            R = q_;
            t = t_;
        }
    };

    struct GraphicsNode
    {
        std::string  name;
        Transformation T;  //transformation (pose) in its parent node's coordination system.
        std::vector<GeometryBasePtr> geomtries;

        std::vector<std::shared_ptr<GraphicsNode> > children;
    };

    using GraphicsNodePtr = std::shared_ptr<GraphicsNode>;

    using GraphicsData = GraphicsNodePtr;
}

#endif // !PCDVIEWER_GEOMETRY_H
