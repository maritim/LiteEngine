#include "GL.h"

#include "Core/Console/Console.h"

void GL::DrawElementsInstanced (GLenum mode, GLsizei count, GLenum type, const void* indices, GLsizei primcount)
{
	glDrawElementsInstanced (mode, count, type, indices, primcount);

	ErrorCheck ("glDrawElementsInstanced");
}

// Buffers

void GL::BufferData (GLenum target, GLsizeiptr size, const GLvoid * data, GLenum usage)
{
	glBufferData (target, size, data, usage);

	ErrorCheck ("glBufferData");
}

void GL::BufferSubData (GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid * data)
{
	glBufferSubData (target, offset, size, data);

	ErrorCheck ("glBufferSubData");
}

// Attributes

void GL::EnableVertexAttribArray (GLuint index)
{
	glEnableVertexAttribArray (index);

	ErrorCheck ("glEnableVertexAttribArray");
}

void GL::VertexAttribPointer (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer)
{
	glVertexAttribPointer (index, size, type, normalized, stride, pointer);

	ErrorCheck ("glVertexAttribPointer");
}

void GL::VertexAttribDivisor(GLuint index, GLuint divisor)
{
	glVertexAttribDivisor (index, divisor);

	ErrorCheck ("glVertexAttribDivisor");
}

// Bindings

void GL::BindVertexArray (GLuint array)
{
	glBindVertexArray (array);

	ErrorCheck ("glBindVertexArray");
}

void GL::BindBuffer(GLenum target, GLuint buffer)
{
	glBindBuffer(target, buffer);

	ErrorCheck ("glBindBuffer");
}

// Depth Test

void GL::DepthMask (GLboolean flag)
{
	glDepthMask (flag);

	ErrorCheck ("glDepthMask");
}

// Blending

void GL::BlendFunc (GLenum sfactor, GLenum dfactor)
{
	glBlendFunc (sfactor, dfactor);

	ErrorCheck ("glBlendFunc");
}

void GL::BlendFunci (GLuint buf, GLenum sfactor, GLenum dfactor)
{
	glBlendFunci (buf, sfactor, dfactor);

	ErrorCheck ("glBlendFunci");
}

// Generators

void GL::GenBuffers(GLsizei n,  GLuint * buffers)
{
	glGenBuffers (n, buffers);

	ErrorCheck ("glGenBuffers");
}

// Getters
void GL::GetBooleanv(GLenum pname, GLboolean * params)
{
	glGetBooleanv (pname, params);

	ErrorCheck ("glGetBooleanv");
}

void GL::GetFixedv(GLenum pname, GLfixed * params)
{
	glGetFixedv (pname, params);

	ErrorCheck ("glGetFixedv");
}

void GL::GetFloatv(GLenum pname, GLfloat * params)
{
	glGetFloatv (pname, params); 

	ErrorCheck ("glGetFloatv");
}

void GL::GetIntegerv(GLenum pname, GLint * params)
{
	glGetIntegerv(pname, params);

	ErrorCheck ("glGetIntegerv");
}

void GL::ErrorCheck (const std::string& methodName)
{
	GLenum error;
	while ((error = glGetError()) != GL_NO_ERROR) {
		std::string errorString = "On " + methodName + ": ";

		switch (error) {
			case GL_INVALID_ENUM:
				errorString += "GL_INVALID_ENUM (An unacceptable value is specified for an enumerated argument. The offending command is ignored and has no other side effect than to set the error flag.)";
				break;
			case GL_INVALID_VALUE:
				errorString += "GL_INVALID_VALUE (A numeric argument is out of range. The offending command is ignored and has no other side effect than to set the error flag.)";
				break;
			case GL_INVALID_OPERATION:
				errorString += "GL_INVALID_OPERATION (The specified operation is not allowed in the current state. The offending command is ignored and has no other side effect than to set the error flag.)";
				break;
			case GL_INVALID_FRAMEBUFFER_OPERATION:
				errorString += "GL_INVALID_FRAMEBUFFER_OPERATION (The framebuffer object is not complete. The offending command is ignored and has no other side effect than to set the error flag.)";
				break;
			case GL_OUT_OF_MEMORY:
				errorString += "GL_OUT_OF_MEMORY (There is not enough memory left to execute the command. The state of the GL is undefined, except for the state of the error flags, after this error is recorded.)";
				break;
			case GL_STACK_UNDERFLOW:
				errorString += "GL_STACK_UNDERFLOW (An attempt has been made to perform an operation that would cause an internal stack to underflow.)";
				break;
			case GL_STACK_OVERFLOW:
				errorString += "GL_STACK_OVERFLOW (An attempt has been made to perform an operation that would cause an internal stack to overflow.)";
				break;
		}

		Console::LogError (errorString);
	}
}