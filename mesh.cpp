#include <string>
#include <vector>

#include "mesh.h"

const int max_vertex_count = 10000;
const int max_index_count = 5000;

Mesh::Mesh() {
    // 5 - vertex : pos 3, tex 2
    _data_stride = sizeof(GLfloat)*5;

    _vertex_count = 0;
    _vertexlist = (unsigned char *)malloc(_data_stride*max_vertex_count);
    _indexlist = (GLuint *)malloc(sizeof(GLuint)*max_index_count);
}

Mesh::~Mesh() {
    free(_vertexlist);
    free(_indexlist);
}

void Mesh::load_OBJ(const std::string& path)
{
    std::vector<PolyOBJ> temp_poly;
    std::vector< glm::vec3 > temp_vertices;
    std::vector< glm::vec2 > temp_uvs;

    FILE * file = fopen(path.c_str(), "r");
    if ( file == NULL ) {
        printf("Impossible to open the file !\n");
        return;
    }

    while( 1 ){

        char lineHeader[128];
        // read the first word of the line
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
            break; // EOF = End Of File. Quit the loop.

        // else : parse lineHeader

        if ( strcmp( lineHeader, "v" ) == 0 ){
            glm::vec3 vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
            temp_vertices.push_back(vertex);
        }
        else if ( strcmp( lineHeader, "vt" ) == 0 ){
            glm::vec2 uv;
            fscanf(file, "%f %f\n", &uv.x, &uv.y );
            temp_uvs.push_back(uv);
        }
        // else if ( strcmp( lineHeader, "vn" ) == 0 ){
        //     glm::vec3 normal;
        //     fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
        //     temp_normals.push_back(normal);
        // }
        else if ( strcmp( lineHeader, "f" ) == 0 ){
            
            unsigned int vertexIndex[3], uvIndex[3];
            int matches = fscanf(file, "%d/%d %d/%d %d/%d\n", &vertexIndex[0], &uvIndex[0], &vertexIndex[1], &uvIndex[1], &vertexIndex[2], &uvIndex[2] );
            if (matches != 6){
                printf("File can't be read by our simple parser : ( Try exporting with other options\n");
                return;
            }

            PolyOBJ poly;

            poly.v[0] = vertexIndex[0];
            poly.v[1] = vertexIndex[1];
            poly.v[2] = vertexIndex[2];

            poly.t[0] = uvIndex[0];
            poly.t[1] = uvIndex[1];
            poly.t[2] = uvIndex[2];

            temp_poly.push_back(poly);
            
            
            // vertexIndices.push_back(vertexIndex[0]);
            // vertexIndices.push_back(vertexIndex[1]);
            // vertexIndices.push_back(vertexIndex[2]);
            // uvIndices    .push_back(uvIndex[0]);
            // uvIndices    .push_back(uvIndex[1]);
            // uvIndices    .push_back(uvIndex[2]);
            // normalIndices.push_back(normalIndex[0]);
            // normalIndices.push_back(normalIndex[1]);
            // normalIndices.push_back(normalIndex[2]);
        }


    }

    
    int ind = 0;
    int tri = 0;

    GLfloat *vlist = nullptr;

    // For each vertex of each triangle
    for( unsigned int i=0; i<temp_poly.size(); i++ ){

        //индекс позиции вершины - это vertexIndices[i] :
        int vertexIndex1 = temp_poly[i].v[0];
        int vertexIndex2 = temp_poly[i].v[1];
        int vertexIndex3 = temp_poly[i].v[2];

        int uvIndex1 = temp_poly[i].t[0];
        int uvIndex2 = temp_poly[i].t[1];
        int uvIndex3 = temp_poly[i].t[2];

        glm::vec3 p = temp_vertices[ vertexIndex1-1 ];
        glm::vec2 t = temp_uvs[ uvIndex1-1 ];

        // compute memory position of next free vertex
        vlist = (GLfloat *)(_vertexlist + ind*_data_stride);

        vlist[0] = p.x;
        vlist[1] = p.y;
        vlist[2] = p.z;
        vlist[3] = t.x;
        vlist[4] = t.y;

        ind++;

        p = temp_vertices[ vertexIndex2-1 ];
        t = temp_uvs[ uvIndex2-1 ];

        // compute memory position of next free vertex
        vlist = (GLfloat *)(_vertexlist + ind*_data_stride);

        vlist[0] = p.x;
        vlist[1] = p.y;
        vlist[2] = p.z;
        vlist[3] = t.x;
        vlist[4] = t.y;

        ind++;

        p = temp_vertices[ vertexIndex3-1 ];
        t = temp_uvs[ uvIndex3-1 ];

        // compute memory position of next free vertex
        vlist = (GLfloat *)(_vertexlist + ind*_data_stride);

        vlist[0] = p.x;
        vlist[1] = p.y;
        vlist[2] = p.z;
        vlist[3] = t.x;
        vlist[4] = t.y;

        ind++;

        _indexlist[tri+0] = ind - 3;
        _indexlist[tri+1] = ind - 2;
        _indexlist[tri+2] = ind - 1;

        tri += 3;

        _vertex_count += 3;
        _index_count += 3;
    }

}
