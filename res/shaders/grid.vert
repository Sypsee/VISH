#version 330 core

uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjMatrix;
uniform vec3 u_CamPos;

out vec3 fragPos;

const vec3 m_Pos[4] = vec3[4](
    vec3(-1.0, 0.0, -1.0),
    vec3( 1.0, 0.0, -1.0),
    vec3( 1.0, 0.0,  1.0),
    vec3(-1.0, 0.0,  1.0)
);

const int m_Indices[6] = int[6](0, 2, 1, 2, 0, 3);

const float gridSize = 100.0;

void main()
{
    int index = m_Indices[gl_VertexID];
    fragPos = m_Pos[index] * gridSize;
    
    fragPos.x += u_CamPos.x;
    fragPos.z += u_CamPos.z;

    gl_Position =  u_ProjMatrix * u_ViewMatrix * vec4(fragPos, 1.0);
}