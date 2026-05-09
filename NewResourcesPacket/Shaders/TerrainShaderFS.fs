precision mediump float;

varying vec2 v_uv, v_uv2;
varying vec3 v_worldPos;

uniform sampler2D u_texture_0; // BlendMap
uniform sampler2D u_texture_1; // Dirt
uniform sampler2D u_texture_2; // Rock
uniform sampler2D u_texture_3; // Grass

uniform vec3 u_cameraPos;
uniform vec3 u_fogColor;
uniform float u_smallRadius;
uniform float u_largeRadius;

void main()
{
    vec4 c_blend = texture2D(u_texture_0, v_uv2);
    vec4 c_dirt  = texture2D(u_texture_1, v_uv);
    vec4 c_rock  = texture2D(u_texture_2, v_uv);
    vec4 c_grass = texture2D(u_texture_3, v_uv);

    vec4 c_final = c_rock * c_blend.r + c_grass * c_blend.g + c_dirt * c_blend.b;
    c_final.a = 1.0;

    float d = distance(u_cameraPos, v_worldPos);
    
    float alpha = clamp((d - u_smallRadius) / (u_largeRadius - u_smallRadius), 0.0, 1.0);
    
    vec3 foggedColor = mix(c_final.rgb, u_fogColor, alpha);
    
    gl_FragColor = vec4(foggedColor, c_final.a);
}