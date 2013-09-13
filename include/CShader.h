#ifndef CSHADER_H
#define CSHADER_H
#include <string>
enum
{
    SHADER_NORMAL,
    SHADER_GEOM,
    SHADER_COMP,

    SHADER_TYPE_COUNT
};

class CShader
{
    private:
        std::string VS,FS,GS;
        void read(const char* filename);
        void build();
        void showinfo(uint32_t id,const std::string &type,const std::string &name);
    public:
        uint32_t id;
        CShader(const char* filename);
        virtual ~CShader();

        void use();
        void setUniformMat4(const char* uniform,const glm::mat4 value);
    protected:
};

#endif // CSHADER_H
