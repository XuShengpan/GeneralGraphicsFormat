#include "general_graphics_file_io.h"
#include <iostream>
#include <fstream>
#include "string_utility.h"

namespace general_graphics_file_ns {

    bool read_line(std::ifstream& infile, std::string& line)
    {
        if (infile.eof())
            return false;
        if (!std::getline(infile, line))
            return false;
        if (line.empty() || line == "\r" ) {
            return read_line(infile, line);
        }
        if (line[0] == '#') {
            return read_line(infile, line);
        }
        return true;
    }

    std::ostream& operator<<(std::ostream& o, const LineGeometryType& geo)
    {
        o << geo.first << " " << geo.second;
        return o;
    }

    std::ostream& operator<<(std::ostream& o, const PolylineGeometryType& geo)
    {
        o << geo.size() << " ";
        for (auto i : geo) {
            o << i << " ";
        }
        return o;
    }

    std::ifstream& operator >> (std::ifstream& in, LineGeometryType& geo)
    {
        in >> geo.first >> geo.second;
        return in;
    }

    std::ifstream& operator >> (std::ifstream& in, PolylineGeometryType& geo)
    {
        int n;
        in >> n;
        int j;
        for (int i = 0; i < n; ++i) {
            in >> j;
            geo.push_back(j);
        }
        return in;
    }

    template<typename REAL>
    void write_general_graphics_file(const GeneralGraphicContent<REAL>& content, const std::string& filepath)
    {
        //save coords
        std::ofstream ofile(filepath);
        ofile << "GeneralGraphicFormat" << std::endl;
        ofile << "point_list " <<get_coordniate_type_string(content.coordinate_type)<<" "<<content.coords.size() << std::endl;
        ofile.setf(std::ios::fixed);
        if (content.coordinate_type == CT_SpatialRectangleCoordinateSystem)
            ofile.precision(4);
        else
            ofile.precision(9);
        for (int i = 0; i < content.coords.size(); ++i) {
            ofile << i << " " << content.coords[i][0] << " " << content.coords[i][1] << " " << content.coords[i][2] << std::endl;
        }

        //save all points
        for (const auto iter : content.points_tables) {
            PointsTablePtr table = iter.second;
            if(table->size() == 0)
                continue;
            const auto table_name = iter.first;
            const auto feature_table = table->get_feature_table();
            const auto fields_info = feature_table->get_fields_info();
            ofile << "points " << table_name << "\n"
                << "fields_info " << fields_info.size() << std::endl;

            //save fields info
            for (int i = 0; i < fields_info.size(); ++i) {
                ofile << i << " " << fields_info[i].name << " " << get_field_type_string(fields_info[i].datatype) << std::endl;
            }

            const auto n = table->size();
            ofile << "data " << n << std::endl;
            for (int i = 0; i < n; ++i) {
                ofile << i << " ";
                //save features
                for (int j = 0; j < fields_info.size(); ++j) {
                    ofile << feature_table->get_feature_string(i, j) << " ";
                }
                //save geometries
                ofile << table->get_geometry(i) << " ";
                ofile << '{' << table->get_discription(i) << '}' << std::endl;
            }
        }

        //save all lines
        for (const auto iter : content.lines_tables) {
            LinesTablePtr table = iter.second;
            if (table->size() == 0)
                continue;
            const auto feature_table = table->get_feature_table();
            const auto fields_info = feature_table->get_fields_info();
            ofile << "lines " << table->get_table_name() << "\n"
                << "fields_info " << fields_info.size() << std::endl;

            //save fields info
            for (int i = 0; i < fields_info.size(); ++i) {
                ofile << i << " " << fields_info[i].name << " " << get_field_type_string(fields_info[i].datatype) << std::endl;
            }

            const auto n = table->size();
            ofile << "data " << n << std::endl;
            for (int i = 0; i < n; ++i) {
                ofile << i << " ";
                //save features
                for (int j = 0; j < fields_info.size(); ++j) {
                    ofile << feature_table->get_feature_string(i, j) << " ";
                }
                //save geometries
                ofile << table->get_geometry(i) << " ";
                ofile << '{' << table->get_discription(i) << '}' << std::endl;
            }
        }

        //save all polylines
        for (const auto iter : content.polylines_tables) {
            PolylinesTablePtr table = iter.second;
            if (table->size() == 0)
                continue;
            const auto feature_table = table->get_feature_table();
            const auto fields_info = feature_table->get_fields_info();
            ofile << "polylines " << table->get_table_name() << "\n"
                << "fields_info " << fields_info.size() << std::endl;

            //save fields info
            for (int i = 0; i < fields_info.size(); ++i) {
                ofile << i << " " << fields_info[i].name << " " << get_field_type_string(fields_info[i].datatype) << std::endl;
            }

            const auto n = table->size();
            ofile << "data " << n << std::endl;
            for (int i = 0; i < n; ++i) {
                ofile << i << " ";
                //save features
                for (int j = 0; j < fields_info.size(); ++j) {
                    ofile << feature_table->get_feature_string(i, j) << " ";
                }
                //save geometries
                ofile << table->get_geometry(i) << " ";
                ofile << '{' << table->get_discription(i) << '}' << std::endl;
            }
        }

        //save all polygons
        for (const auto iter : content.polygons_tables) {
            PolygonsTablePtr table = iter.second;
            if (table->size() == 0)
                continue;
            auto feature_table = table->get_feature_table();
            const auto fields_info = feature_table->get_fields_info();
            ofile << "polygons " << table->get_table_name() << "\n"
                << "fields_info " << fields_info.size() << std::endl;

            //save fields info
            for (int i = 0; i < fields_info.size(); ++i) {
                ofile << i << " " << fields_info[i].name << " " << get_field_type_string(fields_info[i].datatype) << std::endl;
            }

            const auto n = table->size();
            ofile << "data " << n << std::endl;
            for (int i = 0; i < n; ++i) {
                ofile << i << " ";
                //save features
                for (int j = 0; j < fields_info.size(); ++j) {
                    ofile << feature_table->get_feature_string(i, j) << " ";
                }
                //save geometries
                ofile << table->get_geometry(i) << " ";
                ofile << '{' << table->get_discription(i) << '}' << std::endl;
            }
        }

        ofile.clear();
        ofile.close();
    }

    bool read_fieldsinfo(std::ifstream& infile, FieldsInfo& info)
    {
        info.clear();
        std::string key;
        int n;
        infile >> key >> n;
        if (key != "fields_info") {
            std::cout << "Error: wrong format !" << std::endl;
            return false;
        }
        std::string line;
        int id;
        std::string name, data_type;
        for (int i = 0; i < n; ++i) {
            if (!read_line(infile, line))
                return false;
            std::stringstream ss(line);
            ss >> id >> name >> data_type;
            info.push_back(name, get_field_type_from_string(data_type));
        }
        if (info.size() != n) {
            std::cout << "Error: wong fields info, size contradictory !" << std::endl;
            return false;
        }
        return true;
    }

    void read_item_feature_data(std::ifstream& infile, const FieldsInfo& info, ItemFeatureData& feature)
    {
        int offset = 0;
        for (int i = 0; i < info.size(); ++i) {
            switch (info[i].datatype) {
            case INT8: {
                char c;
                infile >> c;
                feature.set_feature(offset, c);
                offset += 1;
                break;
            }
            case UINT8: {
                uchar c;
                infile >> c;
                feature.set_feature(offset, c);
                offset += 1;
                break;
            }
            case INT16: {
                short c;
                infile >> c;
                feature.set_feature(offset, c);
                offset += 2;
                break;
            }
            case UINT16: {
                unsigned short c;
                infile >> c;
                feature.set_feature(offset, c);
                offset += 2;
                break;
            }
            case INT32: {
                int c;
                infile >> c;
                feature.set_feature(offset, c);
                offset += 4;
                break;
            }
            case UINT32: {
                unsigned int c;
                infile >> c;
                feature.set_feature(offset, c);
                offset += 4;
                break;
            }
            case FLOAT32: {
                float c;
                infile >> c;
                feature.set_feature(offset, c);
                offset += 4;
                break;
            }
            case FLOAT64: {
                double c;
                infile >> c;
                feature.set_feature(offset, c);
                offset += 8;
                break;
            }
            default:
                break;
            }
        }
    }

    std::string read_description(std::ifstream& infile)
    {
        std::string line;
        std::getline(infile, line);

        std::string str;
        size_t i = line.find_first_of('{');
        size_t j = line.find_last_of('}');
        return std::string(line.begin() + i + 1, line.begin() + j);
    }

    template<typename GeometryType>
    typename GraphicsTable<GeometryType>::Ptr read_graphics_data(std::ifstream& infile)
    {
        typename GraphicsTable<GeometryType>::Ptr table;
        FieldsInfo info;
        if (!read_fieldsinfo(infile, info)) {
            return table;
        }

        std::string key;
        int n = 0;
        std::string line;
        if (!read_line(infile, line)) {
            return table;
        }
        {
            std::stringstream ss(line);
            ss >> key >> n;
            if (key != "data") {
                std::cout << "Error: wrong format !" << std::endl;
                return table;
            }
            if (n <= 0) {
                return table;
            }
        }
        table.reset(new GraphicsTable<GeometryType>(info));

        table->reserve(n);

        std::string str;
        int id;
        for (int i = 0; i < n; ++i) {
            infile >> id;
            ItemFeatureData item_feature(info.length());
            read_item_feature_data(infile, info, item_feature);

            GeometryType v;
            infile >> v;

            auto str = read_description(infile);
            table->push_back(item_feature, v, str);
        }

        return table;
    }

    template<typename REAL>
    bool read_general_graphics_file(const std::string& filepath, GeneralGraphicContent<REAL>& content)
    {
        //read coords
        std::ifstream infile(filepath);
        std::string line;

        if(!std::getline(infile, line)) {
            if (line != "GeneralGraphicFormat") {
                std::cout << "Not a general graphic format file !" << std::endl;
                return false;
            }
        }

        if (!read_line(infile, line)) {
            std::cout << "Read the first line failed !" << std::endl;
            return false;
        }

        {
            //read point_list
            std::string strs[3];
            std::stringstream ss(line);
            ss >> strs[0] >> strs[1] >> strs[2];
            if (strs[0] != "point_list") {
                std::cout << "Wrong format !" << std::endl;
                return false;
            }
            if (!get_coordinate_system_from_string(strs[1], content.coordinate_type)) {
                std::cout << "Wrong format !" << std::endl;
                return false;
            }

            std::uint32_t n = get_number<std::uint32_t>(strs[2]);
            if (n == 0) {
                std::cout << "Empty file !" << std::endl;
                return 0;
            }
            content.coords.reserve(n);
            std::uint32_t i;
            REAL x, y, z;
            for (std::uint32_t k = 0; k < n; ++k) {
                infile >> i >> x >> y >> z;
                content.coords.emplace_back(x, y, z);
            }
        }

        while (read_line(infile, line)) {

            std::string geometry_type, table_name;
            std::stringstream ss(line);
            ss >> geometry_type >> table_name;
            if (geometry_type == "points") {
                std::cout << "Reading points: " << table_name << std::endl;
                auto table = read_graphics_data<PointGeometryType>(infile);
                if (table.get() == nullptr) {
                    return false;
                }
                table->set_table_name(table_name);
                content.points_tables[table_name] = table;
            } else if (geometry_type == "lines") {
                std::cout << "Reading lines: " << table_name << std::endl;
                auto table = read_graphics_data<LineGeometryType>(infile);
                if (table.get() == nullptr) {
                    return false;
                }
                table->set_table_name(table_name);
                content.lines_tables[table_name] = table;
            } else if (geometry_type == "polylines") {
                std::cout << "Reading polylines: " << table_name << std::endl;
                auto table = read_graphics_data<PolylineGeometryType>(infile);
                if (table.get() == nullptr) {
                    return false;
                }
                table->set_table_name(table_name);
                content.polylines_tables[table_name] = table;
            } else if (geometry_type == "polygons") {
                std::cout << "Reading polygons: " << table_name << std::endl;
                auto table = read_graphics_data<PolygonGeometryType>(infile);
                if (table.get() == nullptr) {
                    return false;
                }
                table->set_table_name(table_name);
                content.polygons_tables[table_name] = table;
            } else {
                std::cout << "Error: wrong format !" << std::endl;
                return false;
            }

        }
        return true;

    }


    template void write_general_graphics_file<float>(const GeneralGraphicContent<float>& content, const std::string& filepath);
    template void write_general_graphics_file<double>(const GeneralGraphicContent<double>& content, const std::string& filepath);

    template bool read_general_graphics_file<float>(const std::string& filepath, GeneralGraphicContent<float>& content);
    template bool read_general_graphics_file<double>(const std::string& filepath, GeneralGraphicContent<double>& content);
}
