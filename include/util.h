#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

#define GL_LOG_FILE "gl.log"

static inline bool restart_gl_log ()
{
    FILE* file = fopen (GL_LOG_FILE, "w+");
    if (!file)
    {
        fprintf (stderr, "ERROR: could not open %s log file for writing\n", GL_LOG_FILE);
        return false;
    }
    time_t now = time (NULL);
    char* date = ctime (&now);
    fprintf (file, "%s log. local time %s\n", GL_LOG_FILE, date);
    fclose (file);
    return true;
}

static inline bool gl_log (const char* message, const char* filename, int line)
{
    FILE* file = fopen (GL_LOG_FILE, "a+");
    if (!file)
    {
        fprintf (stderr, "ERROR: could not open %s for writing\n", GL_LOG_FILE);
        return false;
    }
    fprintf (file, "%s:%i %s\n", filename, line, message);
    fclose (file);
    return true;
}

glm::vec3 cartesianToSpherical(glm::vec3 c)
{
    glm::vec3 ret(0,0,0);

    /// Radius
    ret.x=glm::length(c);

    /// Elevation
    ret.y=glm::asin(c.y/ret.x);

    /// Polar
    ret.z=glm::atan(c.x/c.z);
    if(c.z<0)
        ret.z+=glm::pi<float>();

    return ret;
}

glm::vec3 sphericalToCartesian(glm::vec3 s)
{
    glm::vec3 ret(0,0,0);

    float a=s.x * glm::cos(s.y);

    ret.x = a * glm::sin(s.z);
    ret.y = s.x * glm::sin(s.y);
    ret.z = a * glm::cos(s.z);

    return ret;
}

void printVec3(const char * name,glm::vec3 a)
{
    printf("VEC3 %s: %f %f %f\n",name,a.x,a.y,a.z);
}

void printVec4(const char * name,glm::vec4 a)
{
    printf("VEC3 %s: %f %f %f %f\n",name,a.x,a.y,a.z,a.w);
}

#endif // UTIL_H_INCLUDED
