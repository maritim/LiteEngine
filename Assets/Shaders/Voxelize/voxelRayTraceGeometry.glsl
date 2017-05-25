#version 330 core

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

out vec2 geom_RayCoordinates;

void main () 
{
    geom_RayCoordinates = vec2 (1.0, 1.0);
    gl_Position = vec4 (geom_RayCoordinates, 0.0, 1.0 );
    EmitVertex();

    geom_RayCoordinates = vec2 (-1.0, 1.0);
    gl_Position = vec4 (geom_RayCoordinates, 0.0, 1.0 );
    EmitVertex();

    geom_RayCoordinates = vec2 (1.0, -1.0);
    gl_Position = vec4(geom_RayCoordinates, 0.0, 1.0 );
    EmitVertex();

    geom_RayCoordinates = vec2 (-1.0, -1.0);
    gl_Position = vec4(geom_RayCoordinates, 0.0, 1.0 );
    EmitVertex();

    EndPrimitive(); 
}