#include "CShader.h"
#include "CQuad.h"
#include "CRenderer.h"
#include "CCamera.h"

#include <SOIL.h>


int main ()
{
    CRenderer* renderer=new CRenderer(glm::vec2(1280,720));

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

    glm::mat4 M,MVP;

    M=glm::mat4(1);

    double fpstracker=glfwGetTime();
    int fps=0;

    CMesh* mesh=new CMesh();

    mesh->loadFromPLYFile("feisar.ply");

    uint32_t ftex=SOIL_load_OGL_texture("diffuse.bmp",SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);
    uint32_t stex=SOIL_load_OGL_texture("specular.bmp",SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);

    CMesh* sphere=new CMesh();
    sphere->loadFromPLYFile("sphere.ply");

    CCamera* cam=new CCamera(renderer,glm::vec3(0,2,5),glm::vec3(0,2,0),glm::vec3(0,1,0),true);
    while (!glfwWindowShouldClose (renderer->getWindow()) && !glfwGetKey(renderer->getWindow(),GLFW_KEY_ESCAPE)==GL_TRUE)
    {
        static double previous_seconds = glfwGetTime ();
        double current_seconds = glfwGetTime ();
        double elapsed_seconds = current_seconds - previous_seconds;
        previous_seconds = current_seconds;

        cam->update(elapsed_seconds);

        // wipe the drawing surface clear
        glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,tex2d);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,spec);
        for(int32_t z=-6; z<6; z++)
            for(int32_t x=-6; x<6; x++)
            {
                M=glm::mat4(1.0f);
                M=glm::translate(M,glm::vec3(x*2,0,z*2));
                //MVP=P*V*M;
                quad.shader->setUniformMat4("M",M);
                quad.shader->setUniformMat4("V",cam->getView());
                quad.shader->setUniformMat4("P",renderer->getPerspective());
                quad.draw();
            }
        M=glm::mat4(1.0f);
        quad.shader->setUniformMat4("M",M);
        quad.shader->setUniformMat4("V",cam->getView());
        quad.shader->setUniformMat4("P",renderer->getPerspective());
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,ftex);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,stex);
        mesh->draw(quad.shader->id);

        M=glm::mat4(1.0f);
        M=glm::translate(M,glm::vec3(0,1,0));
        quad.shader->setUniformMat4("M",M);
        quad.shader->setUniformMat4("V",cam->getView());
        quad.shader->setUniformMat4("P",renderer->getPerspective());
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,0);
        sphere->draw(quad.shader->id);

        M=glm::mat4(1.0f);
        M=glm::translate(M,glm::vec3(0,4,4));
        quad.shader->setUniformMat4("M",M);
        quad.shader->setUniformMat4("V",cam->getView());
        quad.shader->setUniformMat4("P",renderer->getPerspective());
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,0);
        sphere->draw(quad.shader->id);

        M=glm::mat4(1.0f);
        M=glm::translate(M,glm::vec3(0,4,-4));
        quad.shader->setUniformMat4("M",M);
        quad.shader->setUniformMat4("V",cam->getView());
        quad.shader->setUniformMat4("P",renderer->getPerspective());
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D,0);
        sphere->draw(quad.shader->id);
        glfwPollEvents();
        // put the stuff we've been drawing onto the display
        glfwSwapBuffers (renderer->getWindow());
        fps++;
        if(glfwGetTime()-fpstracker>=1.0)
        {
            printf("FPS: %i\n",fps);
            fps=0;
            fpstracker=glfwGetTime();
        }
    }

    // close GL context and any other GLFW resources
    glfwTerminate();
    return 0;
}
