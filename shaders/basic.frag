#version 430 core

in vec2 vUV;

uniform sampler2D uTexture;
uniform vec3 uColor;
uniform float uTime;

out vec4 FragColor;

void main() {
    //FragColor = vec4(0.3, 0.7, 1.0, 1.0);

    float pulse = 0.5 + 0.5 * sin(uTime);
    //FragColor = vec4(uColor * pulse, 1.0);
    //FragColor = vec4(uColor.r, uColor.g, uColor.b * pulse, 1.0);

    vec4 texColor = texture(uTexture, vUV);
    FragColor = texColor; // * vec4(uColor, 1.0); 

}
