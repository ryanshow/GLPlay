#include "mesh_resource.h"

#include <fmt/format.h>
#include <json.hpp>

using json = nlohmann::json;

namespace GLPlay {

MeshResource::MeshResource() {
    renderable_ = new Renderable("default", "default");
}

void MeshResource::SetData(std::vector<unsigned char> & data) {
    std::string data_string(data.begin(), data.end());

    constexpr unsigned char QUAD               = 0b00000001;
    constexpr unsigned char FACE_MATERIAL      = 0b00000010;
    constexpr unsigned char FACE_UV            = 0b00000100;
    constexpr unsigned char FACE_VERTEX_UV     = 0b00001000;
    constexpr unsigned char FACE_NORMAL        = 0b00010000;
    constexpr unsigned char FACE_VERTEX_NORMAL = 0b00100000;
    constexpr unsigned char FACE_COLOR         = 0b01000000;
    constexpr unsigned char FACE_VERTEX_COLOR  = 0b10000000;

    auto parsed_data = json::parse(data_string);

    auto parsed_vertices = parsed_data["vertices"];
    auto parsed_colors = parsed_data["colors"];

    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    for (int i=0; i<parsed_vertices.size(); i+=3) {
        Vertex v = {
            {parsed_vertices[i], parsed_vertices[i+1], parsed_vertices[i+2]},
            {},
            {},
            {},
            {}
        };
        vertices.push_back(v);
    }

    auto faces = parsed_data["faces"];

    for(int i=0; i<faces.size(); i++) {
        unsigned char face_type = faces[i];
        fmt::print("Face type: {}, {}\n", i, face_type);
        int face_vertex_count = face_type & QUAD ? 4 : 3;

        std::vector<int> face_indices;

        // Vertices
        for (int j=0; j < face_vertex_count; ++j) {
            indices.push_back(faces[i+1+j]);
            face_indices.push_back(faces[i+1+j]);
        }
        i += face_vertex_count;

        // Material index
        if (face_type & FACE_MATERIAL) {
            fmt::print("FACE_MATERIAL\n");
            // TODO: Implement FACE_MATERIAL import
            ++i;
        }

        if (face_type & FACE_UV) {
            fmt::print("FACE_UV\n");
            // TODO: Implement FACE_UV import
            ++i;
        }

        if (face_type & FACE_VERTEX_UV) {
            fmt::print("FACE_VERTEX_UV\n");
            for (int j = 0; j < face_vertex_count; ++j) {
                // TODO: Implement FACE_VERTEX_UV import
            }
            i+=face_vertex_count;
        }

        if (face_type & FACE_NORMAL) {
            fmt::print("FACE_NORMAL\n");
            // TODO: Implement FACE_NORMAL import
            ++i;
        }

        if (face_type & FACE_VERTEX_NORMAL) {
            fmt::print("FACE_VERTEX_NORMAL\n");
            for (int j = 0; j < face_vertex_count; ++j) {
                // TODO: Implement FACE_VERTEX_NORMAL import
            }
            i+=face_vertex_count;
            fmt::print("i is now..{}\n", i);
        }

        if (face_type & FACE_COLOR) {
            fmt::print("FACE_COLOR\n");
            // TODO: Implement FACE_COLOR import
            ++i;
        }

        if (face_type & FACE_VERTEX_COLOR) {
            fmt::print("FACE_VERTEX_COLOR\n");
            for (int j = 0; j < face_vertex_count; ++j) {
                int color_index = faces[i+1+j];
                uint32_t hex_color = parsed_colors[color_index];
                glm::vec3 col((hex_color&0xFF0000) >> 16, (hex_color&0x00FF00) >> 8, (hex_color&0x0000FF));
                vertices[face_indices[j]].col = glm::vec4(col/255.0f, 1.0f);
            }
            i+=face_vertex_count;
        }
    }

    renderable_->AddMesh(vertices, indices);
}

}