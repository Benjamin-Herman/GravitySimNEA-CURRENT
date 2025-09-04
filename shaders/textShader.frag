#version 330 core

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D text;
uniform vec3 textColor;

void main()
{
    float alpha = texture(text, TexCoords).r; // red channel contains glyph
    FragColor = vec4(textColor, alpha);       // multiply by text color
}
