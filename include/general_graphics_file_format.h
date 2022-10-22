#ifndef  XSP_GENERAL_GRAPHICS_FILE_FORMAT_H
#define XSP_GENERAL_GRAPHICS_FILE_FORMAT_H

#include <vector>
#include <string>
#include <unordered_map>
#include <iostream>
#include <memory>
#include <map>
#include <sstream>

namespace general_graphics_file_ns {

    enum FieldType {
        INT8 = 1,
        UINT8 = 2,
        INT16 = 3,
        UINT16 = 4,
        INT32 = 5,
        UINT32 = 6,
        FLOAT32 = 7,
        FLOAT64 = 8,
        UNKNOWN_TYPE
    };

    template<typename T>
    inline std::string get_string(T num)
    {
        std::ostringstream strm;
        strm.setf(std::ios::fixed);
        strm << num;
        return strm.str();
    }

    static inline int get_field_type_size(const FieldType& type)
    {
        switch (type) {
        case INT8:
        case UINT8:
            return 1;
        case INT16:
        case UINT16:
            return 2;
        case INT32:
        case UINT32:
        case FLOAT32:
            return 4;
        case FLOAT64:
            return 8;
        }
        return -1;
    }

    static std::string get_field_type_string(const FieldType& type)
    {
        switch (type) {
        case INT8:
            return "int8";
        case UINT8:
            return "uint8";
        case INT16:
            return "int16";
        case UINT16:
            return "uint16";
        case INT32:
            return "int32";
        case UINT32:
            return "uint32";
        case FLOAT32:
            return "float32";
        case FLOAT64:
            return "float64";
        }
        return "";
    }

    static inline FieldType get_field_type_from_string(const std::string& types_string)
    {
        if (types_string == "int8")
            return INT8;
        else if (types_string == "uint8")
            return UINT8;
        if (types_string == "int16")
            return INT16;
        else if (types_string == "uint16")
            return UINT16;
        if (types_string == "int32")
            return INT32;
        else if (types_string == "uint32")
            return UINT32;
        if (types_string == "float32")
            return FLOAT32;
        else if (types_string == "float64")
            return FLOAT64;
        else {
            std::cerr << "Error: unknown type" << std::endl;
            return UNKNOWN_TYPE;
        }
    }

    using uchar = unsigned char;

    struct Field
    {
        std::string name;
        FieldType datatype;
        int offset;
    };

    class FieldsInfo : public std::vector<Field>
    {
    public:
        FieldsInfo() {}

        void push_back(const std::string& name, FieldType data_type)
        {
            Field field;
            field.name = name;
            field.datatype = data_type;
            field.offset = _offset;
            std::vector<Field>::push_back(field);

            _offset += get_field_type_size(data_type);
        }

        int length() const
        {
            return _offset;
        }

    private:
        int _offset = 0;
    };

    template<typename REAL>
    struct PointType
    {
        union
        {
            REAL data[3];
            struct
            {
                REAL x;
                REAL y;
                REAL z;
            };
        };

        REAL operator[](int i) const
        {
            return data[i];
        }

        REAL& operator[](int i)
        {
            return data[i];
        }

        PointType():x(0),y(0),z(0) {}
        PointType(REAL x1, REAL y1, REAL z1): x(x1), y(y1), z(z1) {}
    };

    class ItemFeatureData
    {
    private:
        std::vector<uchar> _buffer;

    protected:
        const int _size = 0;  //size of bytes
        std::vector<uchar> _data;

    public:
        ItemFeatureData(int size_of_bytes) : _size(size_of_bytes)
        {
            if (_size > 0) {
                _data.resize(_size, 0);
            }
        }

        ~ItemFeatureData()
        {
        }

        template<typename T>
        T get_feature(int offset) const
        {
            T* ptr = (T*)(&(_data[0]) + offset);
            return *ptr;
        }

        template <typename T>
        void set_feature(int offset, T value)
        {
            T* ptr = (T*)(&(_data[0]) + offset);
            *ptr = value;
        }

        int size() const
        {
            return _size;
        }
    };

    class FeatureTable
    {
    protected:
        int _width;  //size of bytes of an item
        std::unordered_map<std::string, int> _field_index;
    
        FieldsInfo _fields_info;
        std::vector<ItemFeatureData> _features;

    public:
        FeatureTable(const FieldsInfo& fields): _fields_info(fields)
        {
            _width = fields.length();
        }

        ~FeatureTable()
        {
        }

        void reserve(size_t n)
        {
            _features.reserve(n);
        }

        bool push_back(const ItemFeatureData& feature_data)
        {
            if (feature_data.size() != _width) {
                std::cerr << "Error: wrong feature data type !" << std::endl;
                return false;
            }
            _features.push_back(feature_data);
            return true;
        }

        int width() const
        {
            return _width;
        }

        size_t length() const
        {
            return _features.size();
        }

        //if there is no the field, return -1.
        int get_field_index(const std::string& name) const
        {
            auto iter = _field_index.find(name);
            if (iter == _field_index.end())
                return -1;
            return iter->second;
        }

        const FieldsInfo& get_fields_info() const
        {
            return _fields_info;
        }

        std::string get_feature_string(int i_item, int i_feature) const
        {
            const auto& field = _fields_info[i_feature];
            switch (field.datatype) {
            case INT8: {
                char v = _features[i_item].get_feature<char>(field.offset);
                return get_string(v);
            }
            case UINT8: {
                uchar v = _features[i_item].get_feature<uchar>(field.offset);
                return get_string(v);
            }
            case INT16: {
                short v = _features[i_item].get_feature<short>(field.offset);
                return get_string(v);
            }
            case UINT16: {
                unsigned short v = _features[i_item].get_feature<unsigned short>(field.offset);
                return get_string(v);
            }
            case INT32: {
                int v = _features[i_item].get_feature<int>(field.offset);
                return get_string(v);
            }
            case UINT32: {
                unsigned int v = _features[i_item].get_feature<unsigned int>(field.offset);
                return get_string(v);
            }
            case FLOAT32: {
                float v = _features[i_item].get_feature<float>(field.offset);
                return get_string(v);
            }
            case FLOAT64: {
                double v = _features[i_item].get_feature<double>(field.offset);
                return get_string(v);
            }
            }
            return "";
        }

        const ItemFeatureData& operator[] (int i) const
        {
            return _features[i];
        }

        ItemFeatureData& operator[] (int i)
        {
            return _features[i];
        }

        const ItemFeatureData& get_feature_data(int i) const
        {
            return _features[i];
        }

        ItemFeatureData& get_feature_data(int i)
        {
            return _features[i];
        }
    };

    template <typename GEOMETRYTYPE>
    class GraphicsTable
    {
    protected:
        std::string _table_name;
        std::shared_ptr<FeatureTable> _features;
        std::vector<GEOMETRYTYPE> _geometries;
        std::vector<std::string> _descriptions;

    public:
        GraphicsTable(const FieldsInfo& info) : _features(new FeatureTable(info))
        {
        }

        ~GraphicsTable()
        {
            //std::cout << "~GraphicsTable: " << _table_name << std::endl;
        }

        using GeometryType = GEOMETRYTYPE;

        const std::shared_ptr<FeatureTable> get_feature_table() const
        {
            return _features;
        }

        bool push_back(const ItemFeatureData& feature, const GeometryType& geometry, std::string description = "")
        {
            if (!_features->push_back(feature))
                return false;
            _geometries.push_back(geometry);
            _descriptions.push_back(description);
            return true;
        }

        void reserve(int size)
        {
            _features->reserve(size);
            _geometries.reserve(size);
            _descriptions.reserve(size);
        }

        size_t size() const
        {
            return _features->length();
        }

        const ItemFeatureData& get_feature_data(int i) const
        {
            return _features->operator[](i);
        }

        ItemFeatureData& get_feature_data(int i)
        {
            return _features->operator[](i);
        }

        const GeometryType& get_geometry(int i) const
        {
            return _geometries[i];
        }

        GeometryType& get_geometry(int i)
        {
            return _geometries[i];
        }

        const std::string& get_discription(int i) const
        {
            return _descriptions[i];
        }

        void set_discription(int i,const std::string& desc)
        {
            _descriptions[i] = desc;
        }

        void set_table_name(const std::string& name)
        {
            _table_name = name;
        }

        std::string get_table_name() const
        {
            return _table_name;
        }

        const std::vector<GeometryType>& get_geometries() const
        {
            return _geometries;
        }

        using Ptr = std::shared_ptr<GraphicsTable<GeometryType> >;
    };

    using PointGeometryType = int;
    using LineGeometryType = std::pair<int, int>;
    using PolylineGeometryType = std::vector<int>;
    using PolygonGeometryType = std::vector<int>;

    using PointsTable = GraphicsTable<PointGeometryType>;
    using LinesTable = GraphicsTable<LineGeometryType>;
    using PolylinesTable = GraphicsTable<PolylineGeometryType>;
    using PolygonsTable = GraphicsTable<PolygonGeometryType>;

    using PointsTablePtr = PointsTable::Ptr;
    using LinesTablePtr = LinesTable::Ptr;
    using PolylinesTablePtr = PolylinesTable::Ptr;
    using PolygonsTablePtr = PolygonsTable::Ptr;

    enum CoordniateType
    {
        CT_SpatialRectangleCoordinateSystem,
        CT_WGS84
    };

    static std::string get_coordniate_type_string(CoordniateType type)
    {
        if (CT_SpatialRectangleCoordinateSystem == type)
            return "SRCS";
        else if (CT_WGS84 == type)
            return "WGS84";
        else
            return "";
    }

    static bool get_coordinate_system_from_string(const std::string& str, CoordniateType& type)
    {
        if (str == "SRCS") {
            type = CoordniateType::CT_SpatialRectangleCoordinateSystem;
            return true;
        } else if (str == "WG84") {
            type = CoordniateType::CT_WGS84;
            return true;
        }
        return false;
    }

    template<typename REAL>
    struct GeneralGraphicContentData
    {
        std::vector<PointType<REAL> > coords;
        CoordniateType coordinate_type = CT_SpatialRectangleCoordinateSystem;

        std::map<std::string, PointsTablePtr> points_tables;
        std::map<std::string, LinesTablePtr> lines_tables;
        std::map<std::string, PolylinesTablePtr> polylines_tables;
        std::map<std::string, PolygonsTablePtr> polygons_tables;
    };

    template<typename REAL>
    class GeneralGraphicContent: public GeneralGraphicContentData<REAL>
    {
    public:
        GeneralGraphicContent()
        {
        }

        ~GeneralGraphicContent()
        {
        }

        size_t register_point(const PointType<REAL>& p)
        {
            size_t i = this->coords.size();
            this->coords.push_back(p);
            return i;
        }

        void set_coordinate_type(CoordniateType type)
        {
            this->coordinate_type = type;
        }

    };

}

#endif
