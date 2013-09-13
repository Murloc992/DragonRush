#ifndef CMESH_H_INCLUDED
#define CMESH_H_INCLUDED

struct SVertex
{
public:
    SVertex():pos(0),norm(0),uv1(0),uv2(0),col(1)
    {
    }

    SVertex setPos(float x, float y,float z)
    {
        pos=glm::vec3(x,y,z);
        return *this;
    }

    SVertex setNorm(float x, float y,float z)
    {
        norm=glm::vec3(x,y,z);
        return *this;
    }

    SVertex setUV1(float s,float t)
    {
        uv1=glm::vec2(s,t);
        return *this;
    }

    SVertex setUV2(float s,float t)
    {
        uv2=glm::vec2(s,t);
        return *this;
    }

    SVertex setCol(float r, float g, float b, float a)
    {
        col=glm::vec4(r,g,b,a);
        return *this;
    }

    //geometry vectors
    glm::vec3 pos,norm;

    //texture vectors
    glm::vec2 uv1,uv2;

    //color vector
    glm::vec4 col;
};

enum
{
};

class CMesh
{
private:
public:
    CMesh(){};
    ~CMesh(){};
    void calculateNormals()
    {
        for(uint32_t i=0; i<indices.size(); i+=3)
        {
            uint32_t ia=indices[i];
            uint32_t ib=indices[i+1];
            uint32_t ic=indices[i+2];
            glm::vec3 a=vertices[ia].pos;
            glm::vec3 b=vertices[ib].pos;
            glm::vec3 c=vertices[ic].pos;
            glm::vec3 n=glm::normalize(glm::cross(c-a,b-a));
            vertices[ia].norm=n;
            vertices[ib].norm=n;
            vertices[ic].norm=n;
            printf("Generated normal: %f %f %f\n",n.x,n.y,n.z);
        }
    }
protected:
    std::vector<SVertex> vertices;
    std::vector<uint32_t> indices;

    uint32_t VAO,VBO,IBO;
};

#endif // CMESH_H_INCLUDED
