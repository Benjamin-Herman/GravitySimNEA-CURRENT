#version 330 core

out vec4 FragColor;

uniform vec2 resolution; // screen size (e.g., 800x600)

void main()
{
    // Convert fragment position to normalized coordinates [0,1]
    vec2 uv = gl_FragCoord.xy / resolution;

    // Define circle center and radius (in normalized coordinates)
    vec2 center = vec2(0.5, 0.5);
    float radius = 0.4;

    // Distance from center
    float dist = distance(uv, center);

    if (dist > radius)
        discard; // Outside the circle

    // Color changes based on position
    vec3 color = vec3(uv.x, uv.y, 1.0 - dist / radius);

    FragColor = vec4(color, 1.0);
}
