#include <iostream>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include "GraphicsIO.h"

int main()
{
	Eigen::Vector3d orign(0, 0, 0);
	float length = 10.0;

	Eigen::Vector3d ptx0(-1, 0, 0);
	Eigen::Vector3d pty0(0, -1, 0);
	Eigen::Vector3d ptz0(0, 0, -1);

	Eigen::Vector3d ptx1(length, 0, 0);
	Eigen::Vector3d pty1(0, length, 0);
	Eigen::Vector3d ptz1(0, 0, length);

	Eigen::Vector3d pa(-0.5, 0, -0.3);
	Eigen::Vector3d pb(2, -2, 0);
	Eigen::Vector3d pc(6, 5, 1);
	Eigen::Vector3d ab = pb - pa;
	Eigen::Vector3d ac = pc - pa;
	Eigen::Vector3d n = ab.cross(ac).normalized();

	Eigen::Vector3d pn0 = (pa + pb + pc) / 3.0;
	Eigen::Vector3d pn1 = pn0 - n * 0.5;
	Eigen::Vector3d pn2 = pn0 + n * 3.0;

	std::shared_ptr< pcd_viewer_ns::Lines> x_axis(new pcd_viewer_ns::Lines());
	x_axis->setColor(255, 0, 0);
	x_axis->line_width = 3;
	x_axis->name = "X-Axis";
	x_axis->dash = false;
	x_axis->points.resize(2);
	x_axis->points[0][0] = ptx0[0];
	x_axis->points[0][1] = ptx0[1];
	x_axis->points[0][2] = ptx0[2];
	x_axis->points[1][0] = ptx1[0];
	x_axis->points[1][1] = ptx1[1];
	x_axis->points[1][2] = ptx1[2];

	std::shared_ptr< pcd_viewer_ns::Lines> y_axis(new pcd_viewer_ns::Lines());
	y_axis->setColor(0, 255, 0);
	y_axis->line_width = 3;
	y_axis->name = "Y-Axis";
	y_axis->dash = false;
	y_axis->points.resize(2);
	y_axis->points[0][0] = pty0[0];
	y_axis->points[0][1] = pty0[1];
	y_axis->points[0][2] = pty0[2];
	y_axis->points[1][0] = pty1[0];
	y_axis->points[1][1] = pty1[1];
	y_axis->points[1][2] = pty1[2];

	std::shared_ptr< pcd_viewer_ns::Lines> z_axis(new pcd_viewer_ns::Lines());
	z_axis->setColor(0, 0, 255);
	z_axis->line_width = 3;
	z_axis->name = "Z-Axis";
	z_axis->dash = false;
	z_axis->points.resize(2);
	z_axis->points[0][0] = ptz0[0];
	z_axis->points[0][1] = ptz0[1];
	z_axis->points[0][2] = ptz0[2];
	z_axis->points[1][0] = ptz1[0];
	z_axis->points[1][1] = ptz1[1];
	z_axis->points[1][2] = ptz1[2];

	std::shared_ptr<pcd_viewer_ns::Triangles> triangle(new pcd_viewer_ns::Triangles());
	triangle->setColor(255, 255, 255, 20);
	triangle->name = "Triangle";
	triangle->mode.frontMode = pcd_viewer_ns::PolygonMode::Fill;
	triangle->mode.backMode = pcd_viewer_ns::PolygonMode::Line;
	triangle->points.resize(3);
	triangle->points[0][0] = pa[0];
	triangle->points[0][1] = pa[1];
	triangle->points[0][2] = pa[2];

	triangle->points[1][0] = pb[0];
	triangle->points[1][1] = pb[1];
	triangle->points[1][2] = pb[2];

	triangle->points[2][0] = pc[0];
	triangle->points[2][1] = pc[1];
	triangle->points[2][2] = pc[2];

	std::shared_ptr<pcd_viewer_ns::Lines> normal(new pcd_viewer_ns::Lines);
	normal->setColor(0, 255, 255);
	normal->dash = true;
	normal->line_width = 1;
	normal->name = "Triangle Normal";
	normal->points.resize(2);
	normal->points[0][0] = pn1[0];
	normal->points[0][1] = pn1[1];
	normal->points[0][2] = pn1[2];

	normal->points[1][0] = pn2[0];
	normal->points[1][1] = pn2[1];
	normal->points[1][2] = pn2[2];

	pcd_viewer_ns::GraphicsData data;
	pcd_viewer_ns::NodePtr root(new pcd_viewer_ns::Node());
	root->name = "Test Graphics";

	pcd_viewer_ns::OBJPtr   csObj(new pcd_viewer_ns::OBJ());
	csObj->name = "Coordinate System";
	csObj->geomtries.push_back(x_axis);
	csObj->geomtries.push_back(y_axis);
	csObj->geomtries.push_back(z_axis);

	root->objs.push_back(csObj);

	pcd_viewer_ns::OBJPtr triObj(new pcd_viewer_ns::OBJ());
	triObj->name = "TriangleObj";
	triObj->geomtries.push_back(triangle);
	triObj->geomtries.push_back(normal);
	root->objs.push_back(triObj);

	data.nodes.push_back(root);

	pcd_viewer_ns::save_graphics_data(data, "graphics_demo.ghs");

	return 0;

}