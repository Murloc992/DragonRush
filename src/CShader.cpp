#include <fstream>
#include "CShader.h"

CShader::CShader(const char* filename)
{
    read(filename);
    build();
}

CShader::~CShader()
{
    //dtor
}

void CShader::read(const char* filename)
{
    //buffers and shader file string.
    std::string buf,line;

    //the file stream to read the shader from
    std::ifstream fs(filename,std::ios::in);

    //read entire file and append newlines
    while(!fs.eof())
    {
        std::getline(fs,buf);
        if(buf.length()>0)
        {
            line+=buf;
            line+="\n";
        }
    }

    //detect shader tags.
    int vsi=line.find("#VS");
    int fsi=line.find("#FS");
    int gsi=line.find("#GS");

    printf("Search indexes: %i %i %i Length: %i\n",vsi,fsi,gsi,line.length());

    //if VS and FS are present, this is a normal shader
    if(vsi>=0&&fsi>=0&&gsi<0)
        printf("Normal shader loaded.\n");
    //if GS is also present, this is a geometry shader
    else if(vsi>=0&&fsi>=0&&gsi>=0)
        printf("Geometry shader loaded.\n");
    //if no VS/FS present this is not a valid shader
    else if(vsi<0||fsi<0)
    {
        printf("Not a valid shader.");
        return;
    }
    // 3 character tag offset;
    unsigned int to=3;
    VS+=line.substr(vsi+to,fsi-to);
    FS+=line.substr(fsi+to,gsi>0?gsi-fsi-to:line.length()-fsi-to);
    if(gsi>0)
        GS+=line.substr(gsi+to,line.length()-gsi);

    printf("<<<VERTEX SHADER START>>>\n%s\n<<<VERTEX SHADER END>>>\n",VS.c_str());
    printf("<<<FRAGMENT SHADER START>>>\n%s\n<<<FRAGMENT SHADER END>>>\n",FS.c_str());
    if(gsi>0)
        printf("<<<GEOMETRY SHADER START>>>\n%s\n<<<GEOMETRY SHADER END>>>\n",GS.c_str());
}

void CShader::setUniformMat4(const char* uniform,const glm::mat4 value)
{
    glUseProgram(id);
    glUniformMatrix4fv(glGetUniformLocation(id,uniform),1,false,glm::value_ptr(value));
    glUseProgram(0);
}

void CShader::build()
{
    int32_t vs,fs,gs,cs;
    vs=fs=gs=666;

    const char* vss=VS.c_str();
    vs = glCreateShader (GL_VERTEX_SHADER);
    glShaderSource (vs, 1, &vss, NULL);
    glCompileShader (vs);
    glGetShaderiv(vs,GL_COMPILE_STATUS,&cs);
    if(!cs)
    {
        showinfo(vs,"NPROG","vertex");
        return;
    }

    const char* fss=FS.c_str();
    fs = glCreateShader (GL_FRAGMENT_SHADER);
    glShaderSource (fs, 1, &fss, NULL);
    glCompileShader (fs);
    glGetShaderiv(fs,GL_COMPILE_STATUS,&cs);
    if(!cs)
    {
        showinfo(fs,"NPROG","fragment");
        return;
    }

    if(GS.length()>0)
    {
        const char* gss=GS.c_str();
        gs = glCreateShader (GL_FRAGMENT_SHADER);
        glShaderSource (gs, 1, &gss, NULL);
        glCompileShader (gs);
    }

    id = glCreateProgram ();
    glAttachShader (id, fs);
    glAttachShader (id, vs);
    if(gs!=666)
        glAttachShader(id, gs);
    glLinkProgram (id);

    glGetProgramiv(id,GL_LINK_STATUS,&cs);
    if(!cs)
    {
        showinfo(id,"PROG","linking");
        return;
    }
}

void CShader::use()
{
    glUseProgram(id);
}

void CShader::showinfo(uint32_t id,const std::string &type,const std::string &name)
{
    int32_t length = 0;

    if ( type=="PROG" )
        glGetProgramiv ( id, GL_INFO_LOG_LENGTH, &length );
    else
        glGetShaderiv ( id, GL_INFO_LOG_LENGTH, &length );

    if ( length > 1 )
    {
        GLchar *log = new GLchar[length];

        if ( type == "PROG" )
            glGetProgramInfoLog ( id, length, &length, log );
        else
            glGetShaderInfoLog ( id, length, &length, log );

        printf ( "GLSL ERROR (%s:%s)\n", type.c_str(), name.c_str() );
        puts ( log );

        delete[] log;
    }
}
