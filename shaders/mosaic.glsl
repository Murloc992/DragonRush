#VS
#version 420

layout (location=0) in vec3 pos;
layout (location=1) in vec3 norm;
layout (location=2) in vec2 uv;
layout (location=3) in vec4 col;

uniform mat4 MVP;

out vec4 color;
out vec2 st;

void main () {
    st=uv;
    color=col;
    //ORDER MATTERS
    gl_Position = MVP*vec4(pos,1.0);
}

#FS
#version 420

layout (binding = 0) uniform sampler2D tex;

in vec4 color;
in vec2 st;

out vec4 frag_colour;

void main () {
    float dp=1*(1./256);
    vec2 coord=vec2(dp*floor(st.s/dp),dp*floor(st.t/dp));
    frag_colour = texture(tex,coord)*color;
}
