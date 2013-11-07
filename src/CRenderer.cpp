#include "CCamera.h"
#include "CRenderer.h"

CRenderer::CRenderer(glm::vec2 res)
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE,GL_FALSE);

    char info[128];
    sprintf(info,"Project %s Version %d.%d %s",PROJECT_NAME,PROJECT_VERSION_MINOR,PROJECT_VERSION_MAJOR,PROJECT_VERSION_STATE);

    // start GL context and O/S window using the GLFW helper library
    if (!glfwInit ())
    {
        fprintf (stderr, "ERROR: could not start GLFW3\n");
        exit(-1);
    }

    window=glfwCreateWindow(res.x,res.y,info,nullptr,nullptr);
    if (!window)
    {
        fprintf (stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        exit(-1);
    }

    glfwMakeContextCurrent (window);

    // start GLEW extension handler
    glewInit ();

    // get version info
    const GLubyte* renderer = glGetString (GL_RENDERER); // get renderer string
    const GLubyte* version = glGetString (GL_VERSION); // version as a string
    printf ("Renderer: %s\n", renderer);
    printf ("OpenGL version supported %s\n", version);

    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LEQUAL);

    glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_TEXTURE_2D);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    P=glm::perspective(67.f,res.x/res.y,0.1f,1000.0f);
}

CRenderer::~CRenderer()
{
    //dtor
}
