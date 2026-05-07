precision mediump float;

varying vec2 v_uv, v_uv2;

uniform sampler2D u_texture_0; // BlendMap
uniform sampler2D u_texture_1; // Dirt
uniform sampler2D u_texture_2; // Rock
uniform sampler2D u_texture_3; // Grass

void main()
{
	vec4 c_blend = texture2D(u_texture_0, v_uv2);

    vec4 c_dirt  = texture2D(u_texture_1, v_uv);
    vec4 c_rock  = texture2D(u_texture_2, v_uv);
    vec4 c_grass = texture2D(u_texture_3, v_uv);

    vec4 c_final = c_rock * c_blend.r + c_grass * c_blend.g + c_dirt * c_blend.b;
    c_final.a = 1.0;

    gl_FragColor = c_final;
}