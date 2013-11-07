#include "CMesh.h"
#include <fstream>
#include <sstream>

using namespace std;

void CMesh::loadFromPLYFile(const char* filename)
{
    FILE* file=fopen(filename,"r");
    if (file==NULL)
    {
        cerr << "Cannot open " << filename << endl;
        exit(1);
    }

    string line;
    uint32_t verts;
    uint32_t faces;

    while (1)
    {
        char lineHeader[128];
        int res=fscanf(file,"%s",lineHeader);
        if(res==EOF)
            break;
        if (!strcmp(lineHeader,"element"))
        {
            fscanf(file,"%s",lineHeader);
            if(!strcmp(lineHeader,"vertex"))
            {
                fscanf(file,"%i\n",&verts);
                vertices.resize(verts);
                printf("Vertices: %i\n",verts);
            }
            else
            {
                fscanf(file,"%i\n",&faces);
                indices.resize(faces*3);
                printf("Faces: %i\n",faces);
            }
        }
        else if(!strcmp(lineHeader,"end_header"))
        {
            for(uint32_t i=0; i<verts; i++)
            {
                glm::vec3 pos,norm;
                glm::vec2 uv;
                fscanf(file,"%f %f %f %f %f %f %f %f\n",&pos.x,&pos.y,&pos.z,&norm.x,&norm.y,&norm.z,&uv.x,&uv.y);
                SVertex v=SVertex();
                v.pos=pos;
                v.norm=norm;
                v.uv1=uv;
                vertices[i]=v;
            }
            for(uint32_t i=0; i<faces*3; i+=3)
            {
                uint32_t ind[4];
                fscanf(file,"%d %d %d %d\n",&ind[0],&ind[1],&ind[2],&ind[3]);
                indices[i]=ind[1];
                indices[i+1]=ind[2];
                indices[i+2]=ind[3];
            }
        }
        else if(!strcmp(lineHeader,"ply") || !strcmp(lineHeader,"format") || !strcmp(lineHeader,"comment") || !strcmp(lineHeader,"property"))
        {
            /* ignoring this line */
        }
    }
    createBuffers();
}
