#include "CShader.h"
#include "CQuad.h"

#include <SOIL.h>


int main ()
{
    // start GL context and O/S window using the GLFW helper library
    if (!glfwInit ())
    {
        fprintf (stderr, "ERROR: could not start GLFW3\n");
        return 1;
    }
    GLFWwindow* window = glfwCreateWindow (800, 600, "Hello Triangle", NULL, NULL);
    if (!window)
    {
        fprintf (stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent (window);

    // start GLEW extension handler
    glewInit ();

    // get version info
    const GLubyte* renderer = glGetString (GL_RENDERER); // get renderer string
    const GLubyte* version = glGetString (GL_VERSION); // version as a string
    printf ("Renderer: %s\n", renderer);
    printf ("OpenGL version supported %s\n", version);

    // tell GL to only draw onto a pixel if the shape is closer to the viewer
    glEnable(GL_TEXTURE_2D);
    glEnable (GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc (GL_LEQUAL); // depth-testing interprets a smaller value as "closer"
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    CQuad quad=CQuad();
    quad.generate();

    uint32_t tex2d=SOIL_load_OGL_texture("diag.png",SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,tex2d);
    // work out maximum an-isotropy
    GLfloat max_aniso = 0.0f;
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max_aniso);
    // set the maximum!
    glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, max_aniso);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    uint32_t spec=SOIL_load_OGL_texture("img_spec.png",SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D,spec);
    // work out maximum an-isotropy
    max_aniso = 0.0f;
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max_aniso);
    // set the maximum!
    glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, max_aniso);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    if(tex2d==0||spec==0)
        return -1;

    glm::mat4 M,V,P,MVP;

    M=glm::mat4(1);
    V=glm::lookAt(glm::vec3(1,2,1),
                  glm::vec3(0,0,0),
                  glm::vec3(0,1,0));
    P=glm::perspective(67.f,4.f/3.f,0.1f,1000.f);

    CShader* mosaic=new CShader("shaders/phong.glsl");

    quad.shader=mosaic;

    float zpos=0.1;
    float xpos=0.1;

    double fpstracker=glfwGetTime();
    int fps=0;

    while (!glfwWindowShouldClose (window) && !glfwGetKey(window,GLFW_KEY_ESCAPE)==GL_TRUE)
    {
        static double previous_seconds = glfwGetTime ();
        double current_seconds = glfwGetTime ();
        double elapsed_seconds = current_seconds - previous_seconds;
        previous_seconds = current_seconds;

        if(glfwGetKey(window,GLFW_KEY_W)==GLFW_PRESS)
        {
            zpos-=1.f*elapsed_seconds;
            xpos-=1.f*elapsed_seconds;
        }
        else if(glfwGetKey(window,GLFW_KEY_S)==GLFW_PRESS)
        {
            zpos+=1.f*elapsed_seconds;
            xpos+=1.f*elapsed_seconds;
        }
        else if(glfwGetKey(window,GLFW_KEY_A)==GLFW_PRESS)
        {
            zpos+=1.f*elapsed_seconds;
            xpos-=1.f*elapsed_seconds;
        }
        else if(glfwGetKey(window,GLFW_KEY_D)==GLFW_PRESS)
        {
            zpos-=1.f*elapsed_seconds;
            xpos+=1.f*elapsed_seconds;
        }

        V=glm::lookAt(glm::vec3(xpos,5,zpos),
                  glm::vec3(xpos-0.5,0,zpos-0.5),
                  glm::vec3(0,1,0));
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,tex2d);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,spec);
        // wipe the drawing surface clear
        glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        for(int32_t z=-3; z<3; z++)
            for(int32_t x=-3; x<3; x++)
            {
                M=glm::mat4(1.0f);
                M=glm::translate(M,glm::vec3(x*2,0,z*2));
                //MVP=P*V*M;
                quad.shader->setUniformMat4("M",M);
                quad.shader->setUniformMat4("V",V);
                quad.shader->setUniformMat4("P",P);
                quad.draw();
            }
        // put the stuff we've been drawing onto the display
        glfwSwapBuffers (window);
        fps++;
        if(glfwGetTime()-fpstracker>=1.0)
        {
            printf("FPS: %i\n",fps);
            fps=0;
            fpstracker=glfwGetTime();
        }
        // update other events like input handling
        glfwPollEvents ();
    }

    // close GL context and any other GLFW resources
    glfwTerminate();
    return 0;
}
