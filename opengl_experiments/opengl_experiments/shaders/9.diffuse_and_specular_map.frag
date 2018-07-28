#version 330 core
out vec4 FragColor;

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec2 TexCoords;
in vec3 FragPos;  
in vec3 Normal;  
  
uniform vec3 viewPos;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1; 
uniform Light light;

void main() {
    // ambient
    vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, TexCoords));
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * vec3(texture(texture_diffuse1, TexCoords)));
    
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = light.specular * (spec * vec3(texture(texture_specular1, TexCoords)));  
        
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
} 