#include "GraphicsIO.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>

using json = nlohmann::json;

namespace pcd_viewer_ns {

	// --- 辅助函数：保存 Geometry 数据到 JSON 对象 ---
	json geometry_to_json(const GeometryBasePtr& geom) {
		json j;
		// 1. 保存公共属性
		j["name"] = geom->name;
		j["type"] = static_cast<int>(geom->type); // 保存枚举值

		j["alpha_enable"] = geom->alpha_enable;

		// 保存颜色
		j["color"] = { geom->color[0], geom->color[1], geom->color[2], geom->color[3] };

		// 保存点集
		auto& j_points = j["points"];
		for (const auto& p : geom->points) {
			json j_point;
			j_point["x"] = p.x;
			j_point["y"] = p.y;
			j_point["z"] = p.z;
			j_points.push_back(j_point);
		}

		// 2. 根据类型保存特有属性
		switch (geom->type) {
		case GeometryType::Points: {

			auto p = static_cast<Points*>(geom.get());
			j["point_size"] = p->point_size;
			break;
		}
		case GeometryType::Lines: {
			auto l = static_cast<Lines*>(geom.get());
			j["line_width"] = l->line_width;
			j["dash"] = l->dash;
			break;
		}
		case GeometryType::Polyline: {
			auto pl = static_cast<Polyline*>(geom.get());
			j["line_width"] = pl->line_width;
			j["dash"] = pl->dash;
			break;
		}
		case GeometryType::Triangles: {
			auto t = static_cast<Triangles*>(geom.get());
			j["front_mode"] = static_cast<int>(t->mode.frontMode);
			j["back_mode"] = static_cast<int>(t->mode.backMode);
			break;
		}
		case GeometryType::Polygon: {
			auto poly = static_cast<Polygon*>(geom.get());
			j["front_mode"] = static_cast<int>(poly->mode.frontMode);
			j["back_mode"] = static_cast<int>(poly->mode.backMode);
			break;
		}
		}
		return j;
	}

	// 假设 JsonNode 是 nlohmann::json 的别名或类型
	using JsonNode = nlohmann::json;

	void write_transformation(JsonNode& node, const Transformation& T) {

		const auto& p = T.t;
		const auto& q = T.R;

		// 1. 写入平移 [x, y, z]
		node["translation"] = { p.x(), p.y(), p.z() };
		node["rotation"] = { q.x(), q.y(), q.z(), q.w() };
	}

	/**
	 * 从 JSON 节点读取 Transformation
	 */
	Transformation read_transformation(const JsonNode& node) {
		
		Eigen::Vector3d p(0, 0, 0);
		Eigen::Quaterniond q(1, 0, 0, 0);

		// 1. 读取平移
		// 假设 translation 是一个包含3个数字的数组
		if (node.contains("translation") && node["translation"].is_array()) {
			auto& t = node["translation"];
			if (t.size() >= 3) {
				p[0] = t[0].get<double>();
				p[1] = t[1].get<double>();
				p[2] = t[2].get<double>();
			}
		}

		// 2. 读取旋转
		// 假设 rotation 是一个包含4个数字的数组 [qx, qy, qz, qw]
		if (node.contains("rotation") && node["rotation"].is_array()) {
			auto& r = node["rotation"];
			if (r.size() >= 4) {
				double qx = r[0].get<double>();
				double qy = r[1].get<double>();
				double qz = r[2].get<double>();
				double qw = r[3].get<double>();

				// Eigen::Quaterniond 的构造函数参数顺序是 (w, x, y, z)
				q = Eigen::Quaterniond(qw, qx, qy, qz);

				// 可选：归一化四元数，防止数值误差导致缩放
				q.normalize();
			}
		}

		return Transformation(q, p);
	}

	// --- 辅助函数：从 JSON 创建 Geometry 指针 ---
	GeometryBasePtr json_to_geometry(const json& j) {
		GeometryBasePtr ptr = nullptr;
		GeometryType type = static_cast<GeometryType>(j.at("type").get<int>());

		// 1. 根据类型创建对象
		switch (type) {
		case GeometryType::Points:
			ptr = std::make_shared<Points>();
			break;
		case GeometryType::Lines:
			ptr = std::make_shared<Lines>();
			break;
		case GeometryType::Polyline:
			ptr = std::make_shared<Polyline>();
			break;
		case GeometryType::Triangles:
			ptr = std::make_shared<Triangles>();
			break;
		case GeometryType::Polygon:
			ptr = std::make_shared<Polygon>();
			break;
		default:
			return nullptr;
		}

		// 2. 恢复公共属性
		ptr->name = j.value("name", "");
		{
			const auto& j_points = j["points"];
			ptr->points.reserve(j_points.size());

			for (const auto& j_point : j_points) {
				ptr->points.emplace_back(j_point["x"], j_point["y"], j_point["z"]);
			}
		}

		ptr->alpha_enable = j.value("alpha_enable", false);

		auto color_arr = j.at("color").get<std::vector<std::uint8_t>>();
		if (color_arr.size() == 4) {
			ptr->setColor(color_arr[0], color_arr[1], color_arr[2], color_arr[3]);
		}

		// 3. 恢复特有属性
		if (type == GeometryType::Points) {
			auto p = static_cast<Points*>(ptr.get());
			p->point_size = j.value("point_size", 1);
		} else if (type == GeometryType::Lines) {

			auto lines = static_cast<Lines*>(ptr.get());
			lines->line_width = j.value("line_width", 1);
			lines->dash = j.value("dash", false);
		} else if (type == GeometryType::Polyline) {
			auto poly = static_cast<Polyline*>(ptr.get());
			poly->line_width = j.value("line_width", 1);
			poly->dash = j.value("dash", false);
		}
		else if (type == GeometryType::Triangles || type == GeometryType::Polygon) {
			FaceMode mode;
			mode.frontMode = static_cast<PolygonMode>(j.value("front_mode", 2)); // 2 is Fill
			mode.backMode = static_cast<PolygonMode>(j.value("back_mode", 2));

			if (type == GeometryType::Triangles) {

				auto tris = static_cast<Triangles*>(ptr.get());

				tris->mode = mode;
			} else {

				auto plg = static_cast<Polygon*>(ptr.get());
				plg->mode = mode;
			}
		}

		return ptr;
	}

	// --- 核心函数：保存数据 ---
	void save_graphics_data(const GraphicsData& data, const std::string& graphics_json_file) {
		if (!data) return;

		// 递归函数：将节点树转换为 JSON
		std::function<json(const GraphicsNodePtr&)> node_to_json = [&](const GraphicsNodePtr& node) -> json {
			json j;
			j["name"] = node->name;

			// 保存几何体列表
			json geom_array = json::array();
			for (const auto& geom : node->geomtries) {
				geom_array.push_back(geometry_to_json(geom));
			}
			j["geometries"] = geom_array;

			auto& j_transformation = j["transformation"];
			write_transformation(j_transformation, node->T);

			// 递归保存子节点 (嵌套结构的关键)
			json children_array = json::array();
			for (const auto& child : node->children) {
				children_array.push_back(node_to_json(child));
			}
			j["children"] = children_array;

			return j;
		};

		json final_json = node_to_json(data);

		// 写入文件
		std::ofstream o(graphics_json_file);
		if (o.is_open()) {
			o << final_json.dump(4); // 4 表示缩进空格数，方便阅读
			o.close();
		}
		else {
			std::cerr << "Error: Could not open file for writing: " << graphics_json_file << std::endl;
		}
	}

	// --- 核心函数：读取数据 ---
	bool read_graphics_data(const std::string& graphics_json_file, GraphicsData& data) {
		std::ifstream i(graphics_json_file);
		if (!i.is_open()) {
			std::cerr << "Error: Could not open file: " << graphics_json_file << std::endl;
			return false;
		}

		try {
			json j;
			i >> j; // 解析文件流

			// 递归函数：将 JSON 转换为节点树
			std::function<GraphicsNodePtr(const json&)> json_to_node = [&](const json& node_j) -> GraphicsNodePtr {
				auto node = std::make_shared<GraphicsNode>();
				node->name = node_j.value("name", "unnamed");

				// 恢复几何体
				if (node_j.contains("geometries") && node_j["geometries"].is_array()) {
					for (const auto& geom_j : node_j["geometries"]) {
						auto geom_ptr = json_to_geometry(geom_j);
						if (geom_ptr) {
							node->geomtries.push_back(geom_ptr);
						}
					}
				}

				if (node_j.contains("transformation")) {
					node->T = read_transformation(node_j["transformation"]);
				}

				// 递归恢复子节点
				if (node_j.contains("children") && node_j["children"].is_array()) {
					for (const auto& child_j : node_j["children"]) {
						node->children.push_back(json_to_node(child_j));
					}
				}

				return node;
			};

			data = json_to_node(j);
			return true;

		}
		catch (const std::exception& e) {
			std::cerr << "JSON Parse Error: " << e.what() << std::endl;
			return false;
		}
	}

}