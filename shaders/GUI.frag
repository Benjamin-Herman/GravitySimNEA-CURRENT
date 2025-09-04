#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
uniform sampler2D textAtlas;
uniform vec3 textColor;

void main() {
    float alpha = texture(textAtlas, TexCoords).r; // use red channel for glyph
    FragColor = vec4(textColor, alpha);
}
