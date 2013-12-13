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
            glm::vec3 n=glm::normalize(glm::cross(b-c,c-a));
            vertices[ia].norm=n;
            vertices[ib].norm=n;
            vertices[ic].norm=n;
            printf("Generated normal: %f %f %f\n",n.x,n.y,n.z);
        }
    }

    virtual void draw(uint32_t shader)
    {
        glUseProgram(shader);
        glBindVertexArray(VAO);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        glEnableVertexAttribArray(3);
        glDrawElements(GL_TRIANGLES,indices.size(),GL_UNSIGNED_INT,nullptr);
        glBindVertexArray(0);
        glUseProgram(0);
    }

    virtual void generate(){};

    void loadFromPLYFile(const char* filename);

    std::vector<SVertex> getVertices()
    {
        return vertices;
    }
protected:
    std::vector<SVertex> vertices;
    std::vector<uint32_t> indices;

    uint32_t VAO,VBO,IBO;

    void createBuffers()
    {
        glGenVertexArrays(1,&VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1,&VBO);
        glBindBuffer(GL_ARRAY_BUFFER,VBO);
        glBufferData(GL_ARRAY_BUFFER,vertices.size()*sizeof(SVertex),&vertices[0],GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        glEnableVertexAttribArray(3);

        //vec3 pos
        glVertexAttribPointer(0,3,GL_FLOAT,false,sizeof(SVertex),nullptr);
        //vec3 norm
        glVertexAttribPointer(1,3,GL_FLOAT,false,sizeof(SVertex),(void*)12);
        //vec2 uv
        glVertexAttribPointer(2,2,GL_FLOAT,false,sizeof(SVertex),(void*)24);
        //vec4 col
        glVertexAttribPointer(3,4,GL_FLOAT,false,sizeof(SVertex),(void*)40);

        glGenBuffers(1,&IBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices.size()*sizeof(uint32_t),&indices[0],GL_STATIC_DRAW);

        glBindVertexArray(0);
    };
};

#endif // CMESH_H_INCLUDED
