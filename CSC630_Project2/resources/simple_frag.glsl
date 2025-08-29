#version 330 core

in vec3 fragNor;
out vec4 color;

uniform vec3 MatAmb;
uniform vec3 lightP; // Position of the light source

void main()
{
    vec3 normal = normalize(fragNor);
    
    // Diffuse component
    vec3 lightDir = normalize(lightP); // Direction from fragment to light
    float dc = max(dot(normal, lightDir), 0.0); // Diffuse coefficient
    
    // Specular component
    vec3 viewDir = normalize(-vec3(gl_FragCoord)); // Direction from fragment to viewer (camera)
    vec3 reflectDir = reflect(-lightDir, normal); // Reflection direction
    float sc = pow(max(dot(viewDir, reflectDir), 0.0), 32.0); // Specular coefficient

    color = vec4(MatAmb * (dc + sc), 1.0); // Final color with ambient, diffuse, and specular components
}
