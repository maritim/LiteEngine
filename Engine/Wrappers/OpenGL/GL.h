#ifndef GLWrapper_H
#define GLWrapper_H

#include <GL/glew.h>
#include <string>

class GL
{
public:
	
	/*
	 * Viewport
	*/

	static void Viewport(GLint x,  GLint y,  GLsizei width,  GLsizei height); 

	/*
	 * Frame Buffer
	*/

	static void Clear(GLbitfield  mask);
	static void ClearColor(GLclampf red,  GLclampf green,  GLclampf blue,  GLclampf alpha); 
	static void FramebufferTexture2D(GLenum target,  GLenum attachment,  GLenum textarget,  GLuint texture,  GLint level);
	static void DrawBuffer(GLenum buf);
	static void DrawBuffers(GLsizei n, const GLenum *bufs);
	static void ReadBuffer(GLenum mode);
	static void BindFramebuffer(GLenum target,  GLuint framebuffer);
	static void BlitFramebuffer(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, 
		GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);

	/*
	 * Culling
	*/

	static void CullFace(GLenum mode);

	/*
	 * Behaviour 
	*/

	static void Hint(GLenum target,  GLenum mode);

	/*
	 * Draw Calls
	*/

	static void DrawElements (GLenum mode, GLsizei count, GLenum type, const void* indices);
	static void DrawElementsInstanced (GLenum mode, GLsizei count, GLenum type, const void* indices, GLsizei primcount);

	// Buffers
	static void BufferData (GLenum target, GLsizeiptr size, const GLvoid * data, GLenum usage);
	static void BufferSubData (GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid *data);

	// Attributes
	static void EnableVertexAttribArray (GLuint index);
	static void VertexAttribPointer (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer);
	static void VertexAttribIPointer (GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid * pointer);	
	static void VertexAttribDivisor (GLuint index, GLuint divisor);

	// Bind
	static void BindVertexArray (GLuint array);
	static void BindBuffer (GLenum target, GLuint buffer);

	/*
	 * Depth Buffer
	*/

	static void DepthMask (GLboolean flag);
	static void DepthRange(GLclampd nearVal, GLclampd farVal);
	static void ClearDepth(GLclampd  depth);
	static void DepthFunc(GLenum func);

	/*
	 * Stencil Buffer
	*/

	static void StencilFunc(GLenum func, GLint ref, GLuint mask);
	static void StencilOp(GLenum sfail, GLenum dpfail, GLenum dppass);
	static void StencilMask(GLuint mask);

	// Blend
	static void BlendFunc (GLenum sfactor, GLenum dfactor);
	static void BlendFunci (GLuint buf, GLenum sfactor, GLenum dfactor);

	// Generators
	static void GenVertexArrays (GLsizei n, GLuint * arrays);
	static void GenBuffers(GLsizei n,  GLuint * buffers);
	static void GenTextures(GLsizei n,  GLuint * textures);
	static void GenFramebuffers(GLsizei n,  GLuint * framebuffers);

	// Textures
	static void BindTexture(GLenum target, GLuint texture);
	static void TexImage2D(GLenum target,  GLint level,  GLint internalformat,  GLsizei width,  
		GLsizei height,  GLint border,  GLenum format,  GLenum type,  const GLvoid * data); 
	static void TexEnvi(GLenum target,  GLenum pname,  GLint param);
	static void TexEnvf(GLenum target,  GLenum pname,  GLfloat param);
	static void TexParameteri(GLenum target,  GLenum pname,  GLint param);
	static void TexParameterf(GLenum target,  GLenum pname,  GLfloat param);
	static void GenerateMipmap(GLenum target);

	/*
	 * Shaders
	*/

	static GLuint CreateShader(GLenum shaderType);
	static void DeleteShader(GLuint shader);
	static void ShaderSource(GLuint shader, GLsizei count, const GLchar **string, const GLint *length);	
	static void CompileShader(GLuint shader);
	static void GetShaderInfoLog(GLuint  shader,  GLsizei  maxLength,  GLsizei * length,  GLchar * infoLog);

	static GLuint CreateProgram(void);
	static void DeleteProgram(GLuint program);
	static void UseProgram (GLuint program);
	static void LinkProgram(GLuint program);

	static void AttachShader(GLuint program, GLuint shader);
	static void DetachShader(GLuint program, GLuint shader);
	static GLint GetUniformLocation(GLuint program, const GLchar *name);

	/*
	 * Capabilities
	*/

	static void Enable (GLenum cap);
	static void Disable (GLenum cap);
	static void IsEnabled(GLenum cap, bool *val);

	/*
	 * Getters
	*/

	static void GetBooleanv(GLenum pname, GLboolean * params); 
	static void GetFixedv(GLenum pname, GLfixed * params); 
	static void GetFloatv(GLenum pname, GLfloat * params); 
	static void GetIntegerv(GLenum pname, GLint * params);

	/*
	 * Cleaning 
	*/

	static void DeleteVertexArrays (GLsizei n, const GLuint *arrays);
	static void DeleteBuffers(GLsizei n, const GLuint * buffers);
	static void DeleteFramebuffers(GLsizei n, const GLuint * framebuffers);
	static void DeleteTextures(GLsizei n, const GLuint * textures);

	static void Check ();

private:
	// Maybe use this only on development mode?
	static void ErrorCheck (const std::string& methodName);
};

#endif