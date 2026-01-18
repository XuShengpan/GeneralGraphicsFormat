#ifndef  PCDVIEWER_GRAPHICS_IO_H
#define PCDVIEWER_GRAPHICS_IO_H

#include "Graphics.h"

namespace pcd_viewer_ns {

	bool read_graphics_data(const std::string& graphics_json_file, GraphicsData& data);
	void save_graphics_data(const GraphicsData& data, const std::string& graphics_json_file);

}

#endif // !PCDVIEWER_GEOMETRY_H
