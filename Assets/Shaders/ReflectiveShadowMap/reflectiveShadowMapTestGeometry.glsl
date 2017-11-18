#version 330 core

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

out vec2 geom_ScreenCoordinates;

void main () 
{
    geom_ScreenCoordinates = vec2 (1.0, 1.0);
    gl_Position = vec4 (geom_ScreenCoordinates, 0.0, 1.0 );
    EmitVertex();

    geom_ScreenCoordinates = vec2 (-1.0, 1.0);
    gl_Position = vec4 (geom_ScreenCoordinates, 0.0, 1.0 );
    EmitVertex();

    geom_ScreenCoordinates = vec2 (1.0, -1.0);
    gl_Position = vec4(geom_ScreenCoordinates, 0.0, 1.0 );
    EmitVertex();

    geom_ScreenCoordinates = vec2 (-1.0, -1.0);
    gl_Position = vec4(geom_ScreenCoordinates, 0.0, 1.0 );
    EmitVertex();

    EndPrimitive(); 
}