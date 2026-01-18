#include "GraphicsIO.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <iostream>

namespace pcd_viewer_ns {

    using namespace nlohmann;

    FaceMode readFaceMode(const json& geom)
    {
        FaceMode faceMode;
        if (geom.contains("FrontFaceMode")) {
            int v = geom["FrontFaceMode"];
            if (v == 0) {
                faceMode.frontMode = PolygonMode::Point;
            } else if (v == 1) {
                faceMode.frontMode = PolygonMode::Line;
            } else {
                faceMode.frontMode = PolygonMode::Fill;
            }
        }
        if (geom.contains("BackFaceMode")) {
            int v = geom["BackFaceMode"];
            if (v == 0) {
                faceMode.backMode = PolygonMode::Point;
            } else if (v == 1) {
                faceMode.backMode = PolygonMode::Line;
            } else {
                faceMode.backMode = PolygonMode::Fill;
            }
        }
        return faceMode;    
    }

    // 辅助函数：从 JSON 创建 GeometryBasePtr
    static GeometryBasePtr getGeometryFromJson(const std::string& geomType, const json& jGeom)
    {
        GeometryBasePtr ptr;
        if (geomType == "Points") {
            auto geom = std::make_shared<Points>();
            geom->point_size = jGeom.value("PointSize", 1);
            ptr = geom;
        }
        else if (geomType == "Lines") {
            auto geom = std::make_shared<Lines>();
            geom->line_width = jGeom.value("LineWidth", 1);
            geom->dash = jGeom.value("Dash", false);
            ptr = geom;
        }
        else if (geomType == "Polyline") {
            auto geom = std::make_shared<Polyline>();
            geom->line_width = jGeom.value("LineWidth", 1);
            geom->dash = jGeom.value("Dash", false);
            ptr = geom;
        }
        else if (geomType == "Triangles") {
            auto geom = std::make_shared<Triangles>();
            geom->mode = readFaceMode(jGeom);
            ptr = geom;
        }
        else if (geomType == "Polygon") {
            auto geom = std::make_shared<Polygon>();
            geom->mode = readFaceMode(jGeom);
            ptr = geom;
        }
        else {
            throw std::runtime_error("Unknown geometry type: " + geomType);
            return ptr;
        }

        ptr->name = jGeom.value("Name", "");
        auto color = jGeom.find("Color");
        if (color != jGeom.end() && color->is_array() && color->size() >= 3) {
            ptr->color[0] = (*color)[0].get<std::uint8_t>();
            ptr->color[1] = (*color)[1].get<std::uint8_t>();
            ptr->color[2] = (*color)[2].get<std::uint8_t>();
            ptr->color[3] = color->size() >= 4 ? (*color)[3].get<std::uint8_t>() : 255;
        } else {
            ptr->color[0] = ptr->color[1] = ptr->color[2] = ptr->color[3] = 255;
        }

        // 解析 Coordinates -> points (SmartArray2D<float,3>)
        const auto& coords = jGeom["Coordinates"];
        const size_t n = coords.size();
        ptr->points.resize(n);

        for (int i = 0; i < n; ++i) {
            const auto& pt = coords[i];
            ptr->points[i][0] = pt[0].get<float>();
            ptr->points[i][1] = pt[1].get<float>();
            ptr->points[i][2] = pt[2].get<float>();
        }

        return ptr;
    }

    // 辅助函数：从 GeometryBasePtr 生成 JSON 对象
    static void geometryToJson(const GeometryBasePtr& geom, json& jGeomList)
    {
        std::string typeStr;
        switch (geom->getType()) {
        case GeometryType::Points: typeStr = "Points"; break;
        case GeometryType::Lines: typeStr = "Lines"; break;
        case GeometryType::Polyline: typeStr = "Polyline"; break;
        case GeometryType::Triangles: typeStr = "Triangles"; break;
        case GeometryType::Polygon: typeStr = "Polygon"; break;
        default: throw std::runtime_error("Unknown geometry type in save");
        }

        json jGeom;
        jGeom["Name"] = geom->name;
        jGeom["Color"] = { geom->color[0], geom->color[1], geom->color[2], geom->color[3] };

        // Coordinates
        json coords = json::array();

        for (size_t i = 0; i < geom->points.size(); ++i) {
            const auto pt = geom->points[i];
            coords.push_back({ pt[0], pt[1], pt[2] });
        }
        jGeom["Coordinates"] = coords;

        // 特定属性
        if (auto p = std::dynamic_pointer_cast<Points>(geom)) {
            jGeom["PointSize"] = p->point_size;
        }
        else if (auto p = std::dynamic_pointer_cast<Lines>(geom)) {
            jGeom["LineWidth"] = p->line_width;
            jGeom["Dash"] = p->dash;
        }
        else if (auto p = std::dynamic_pointer_cast<Polyline>(geom)) {
            jGeom["LineWidth"] = p->line_width;
            jGeom["Dash"] = p->dash;
        }
        else if (auto p = std::dynamic_pointer_cast<Triangles>(geom)) {
            jGeom["FrontFaceMode"] = (int)p->mode.frontMode;
            jGeom["BackFaceMode"] = (int)p->mode.backMode;
        }
        else if (auto p = std::dynamic_pointer_cast<Polygon>(geom)) {
            jGeom["FrontFaceMode"] = (int)p->mode.frontMode;
            jGeom["BackFaceMode"] = (int)p->mode.backMode;
        }

        jGeomList[typeStr] = jGeom;
    }

    bool read_graphics_data(const std::string& graphics_json_file, GraphicsData& data)
    {
        std::ifstream file(graphics_json_file);
        if (!file.is_open()) {
            return false;
        }

        auto& nodes = data.nodes;
        nodes.clear();

        try {
            json j;
            file >> j;

            if (!j.contains("Nodes") || !j["Nodes"].is_array()) {
                throw std::runtime_error("Missing 'Nodes' array in JSON");
            }

            for (const auto& jNode : j["Nodes"]) {
                if (!jNode.contains("OBJs") || !jNode["OBJs"].is_array()) {
                    continue; // skip invalid node
                }

                auto node = std::make_shared<Node>();
                node->name = jNode.value("Name", ""); // 可选 Name 字段

                for (const auto& jObj : jNode["OBJs"]) {
                    auto obj = std::make_shared<OBJ>();
                    obj->name = jObj.value("Name", "");

                    if (!jObj.contains("Geometries") || !jObj["Geometries"].is_array()) {
                        continue;
                    }

                    for (const auto& jGeomEntry : jObj["Geometries"]) {

                        if (!jGeomEntry.is_object() || jGeomEntry.size() != 1) {
                            throw std::runtime_error("Each geometry entry must be a single-key object");
                        }

                        auto it = jGeomEntry.begin();
                        std::string geomType = it.key();
                        const json& jGeom = it.value();

                        auto geom = getGeometryFromJson(geomType, jGeom);
                        obj->geomtries.push_back(geom);
                    }

                    node->objs.push_back(obj);
                }

                nodes.push_back(node);
            }

            return true;
        }
        catch (const std::exception& e) {
            std::cerr << "Error loading graphics file: " << e.what() << std::endl;
            return false;
        }
    }

    void save_graphics_data(const GraphicsData& data, const std::string& graphics_json_file)
    {
        json j;
        auto jNodes = json::array();

        const auto& nodes = data.nodes;

        for (const auto& node : nodes) {
            json jNode;
            if (!node->name.empty()) {
                jNode["Name"] = node->name;
            }

            json jObjs = json::array();
            for (const auto& obj : node->objs) {
                json jObj;
                jObj["Name"] = obj->name;

                json jGeoms = json::array();
                for (const auto& geom : obj->geomtries) {
                    json jGeomEntry;
                    geometryToJson(geom, jGeomEntry);
                    jGeoms.push_back(jGeomEntry);
                }

                jObj["Geometries"] = jGeoms;
                jObjs.push_back(jObj);
            }

            jNode["OBJs"] = jObjs;
            jNodes.push_back(jNode);
        }

        j["Nodes"] = jNodes;

        std::ofstream file(graphics_json_file);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open file for writing: " + graphics_json_file);
        }
        file << j.dump(4); // pretty print with indent=4
    }

}
