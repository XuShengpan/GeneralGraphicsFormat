#include "general_graphics_file_format.h"
#include "general_graphics_file_io.h"

int main(int argc, char* argv[])
{
    if (argc == 1) {
        std::cout << "Usage: {example_file} {output_example_file}" << std::endl;
        return 0;
    }

    std::string example_file = argv[1], output_example_file = argv[2];

    general_graphics_file_ns::GeneralGraphicContent<float> example_data;
    general_graphics_file_ns::read_general_graphics_file(example_file, example_data);
    general_graphics_file_ns::write_general_graphics_file(example_data, output_example_file);

    return 0;
    
}