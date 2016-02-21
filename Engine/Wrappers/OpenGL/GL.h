#ifndef GLWrapper_H
#define GLWrapper_H

#include <GL/glew.h>
#include <string>

class GL
{
public:
	// Draw Calls
	static void DrawElementsInstanced (GLenum mode, GLsizei count, GLenum type, const void* indices, GLsizei primcount);

	// Buffers
	static void BufferData (GLenum target, GLsizeiptr size, const GLvoid * data, GLenum usage);
	static void BufferSubData (GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid *data);

	// Attributes
	static void EnableVertexAttribArray (GLuint index);
	static void VertexAttribPointer (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer);
	static void VertexAttribDivisor (GLuint index, GLuint divisor);

	// Bind
	static void BindVertexArray (GLuint array);
	static void BindBuffer (GLenum target, GLuint buffer);

	// Depth Test
	static void DepthMask (GLboolean flag);

	// Blend
	static void BlendFunc (GLenum sfactor, GLenum dfactor);
	static void BlendFunci (GLuint buf, GLenum sfactor, GLenum dfactor);

	// Generators
	static void GenBuffers(GLsizei n,  GLuint * buffers);

	// Getters
	static void GetBooleanv(GLenum pname, GLboolean * params); 
	static void GetFixedv(GLenum pname, GLfixed * params); 
	static void GetFloatv(GLenum pname, GLfloat * params); 
	static void GetIntegerv(GLenum pname, GLint * params);

private:
	// Maybe use this only on development mode?
	static void ErrorCheck (const std::string& methodName);
};

#endif