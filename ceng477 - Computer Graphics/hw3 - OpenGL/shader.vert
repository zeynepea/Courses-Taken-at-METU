#version 410

layout(location = 0) in vec3 position;

// Data from CPU
uniform mat4 MVP; // ModelViewProjection Matrix
uniform mat4 MV; // ModelView idMVPMatrix
uniform vec4 cameraPosition;
uniform vec3 light_pos;
uniform float heightFactor;

uniform mat4 normalMatrix;


// Texture-related data
uniform sampler2D textureMap;
uniform sampler2D heightMap;
uniform int widthTexture;
uniform int heightTexture;



// Output to Fragment Shader
out vec2 textureCoordinate; // For texture-color
out vec3 vertexNormal; // For Lighting computation
out vec3 ToLightVector; // Vector from Vertex to Light;
out vec3 ToCameraVector; // Vector from Vertex to Camera;



void computeHeight(inout vec3 adj){

    vec2 coordinate;
    coordinate.x = abs(adj.x - widthTexture) / widthTexture;
    coordinate.y = abs(adj.z - heightTexture) / heightTexture;

    vec4 height4 = texture(heightMap, coordinate);
    adj.y = heightFactor * (height4.x );
}

bool boundryCheck(inout vec3 pos) {
    if(pos.x>=0.0f && pos.x<=widthTexture && pos.z >= 0.0f && pos.z <= heightTexture ){
        computeHeight(pos);
        return true;
    }
    else{
        return false;
    }
}

vec3 calculateNormalVector(vec3 pos) {

    vec3 adj1 = pos + vec3(1.0f, 0.0f, 0.0f); 
    vec3 adj2 = pos + vec3(0.0f, 0.0f, 1.0f);  
    vec3 adj3 = pos + vec3(-1.0f, 0.0f, 1.0f); 
    vec3 adj4 = pos + vec3(-1.0f, 0.0f, 0.0f); 
    vec3 adj5 = pos + vec3(0.0f, 0.0f, -1.0f); 
    vec3 adj6 = pos + vec3(1.0f, 0.0f, -1.0f);

    vec3 normal = vec3(0.0, 0.0, 0.0);

    if (boundryCheck(adj1) && boundryCheck(adj2)) {
        normal += normalize(cross(adj2 - pos, adj1 - pos));
    }

    if (boundryCheck(adj2) && boundryCheck(adj3)) {
        normal += normalize(cross(adj3 - pos, adj2 - pos));
    }

    if (boundryCheck(adj3) && boundryCheck(adj4)) {
        normal += normalize(cross(adj4 - pos, adj3 - pos));
    }

    if (boundryCheck(adj4) && boundryCheck(adj5)) {
        normal += normalize(cross(adj5 - pos, adj4 - pos));
    }

    if (boundryCheck(adj5) && boundryCheck(adj6)) {
        normal += normalize(cross(adj6 - pos, adj5 - pos));
    }

    if (boundryCheck(adj6) && boundryCheck(adj1)) {
        normal += normalize(cross(adj1 - pos, adj6 - pos));
    }

    return normalize(normal);
}

void main()
{
    vec3 pos = position;

    float textureCoordinate_x = 1 - (float(position.x) / (widthTexture + 1));
    float textureCoordinate_y = 1 - (float(position.z) / (heightTexture + 1));
    textureCoordinate = vec2(textureCoordinate_x, textureCoordinate_y);

    vec4 height4 = texture(heightMap, textureCoordinate);
    pos.y = heightFactor * (height4.x);

    vec3 norm;
    norm = calculateNormalVector(pos);

    vertexNormal = normalize(vec3(normalMatrix * vec4(norm, 0)));
    ToCameraVector = normalize(vec3(MV * (vec4(vec3(cameraPosition) - pos, 0))));
    ToLightVector = normalize(vec3(MV * vec4(light_pos - pos, 0)));

    gl_Position = MVP * vec4(pos, 1.0);

}
