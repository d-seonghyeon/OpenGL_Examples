#version 330 core
in vec3 normal;
in vec3 position;
in vec2 texCoord;
out vec4 fragColor;


uniform vec3 viewPos;

struct Light{
    vec3 position;
    vec3 direction;
    vec2 cutoff;
    vec3 attenuation;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

};
uniform Light light;

struct Material{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
uniform Material material;


void main(){

    vec3 texColor = texture(material.diffuse, texCoord).xyz;
    vec3 ambient = texColor * light.ambient; //ambientStrength만큼 전체 밝기를 높여줌

    float dist = length(light.position - position);
    vec3 distPoly = vec3(1.0,dist,dist*dist); //1.0,d,d*d
    float attenuation = 1.0/dot(distPoly,light.attenuation);
    //1.0 * Kc +
    //d * Kl +
    //d*d * Kq

    vec3 lightDir = (light.position-position)/dist;

    float theta = dot(lightDir, normalize(-light.direction));
    float intensity = clamp((theta - light.cutoff[1])/(light.cutoff[0]-light.cutoff[1]),0.0f,1.0f);
    vec3 result = ambient;

    if (intensity > 0) {
        vec3 pixelNorm = normalize(normal);
        float diff = max(dot(pixelNorm, lightDir), 0.0);
        vec3 diffuse = diff * texColor * light.diffuse;

        vec3 specColor = texture(material.specular, texCoord).xyz;
        vec3 viewDir = normalize(viewPos - position);
        vec3 reflectDir = reflect(-lightDir, pixelNorm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = spec * specColor * light.specular;

        result += (diffuse + specular) * intensity;
    }

    result *= attenuation;
    fragColor = vec4(result,1.0);
}

//version 330 core를 사용 중이신데, 이 버전부터는 texture2D 함수가 Deprecated(더 이상 권장되지 않음) 되었습니다. 대신 texture 함수를 사용해야 합니다.

//수정 전: vec3 specColor = texture2D(material.specular, texCoord).xyz;

//수정 후: vec3 specColor = texture(material.specular, texCoord).rgb; (xyz 대신 rgb를 쓰는 것이 관례입니다.)