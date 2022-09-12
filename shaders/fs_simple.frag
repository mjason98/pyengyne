$input v_color0, v_texcoord0

#include <bgfx_shader.sh>

SAMPLER2D(s_texColor, 0);

void main()
{
    vec3 sampleC = texture2D(s_texColor, v_texcoord0).xyz;
    gl_FragColor = v_color0;
    gl_FragColor.xyz = sampleC;
}