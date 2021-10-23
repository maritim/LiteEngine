#ifndef GLWrapper_H
#define GLWrapper_H

#include <GL/glew.h>
#include <string>

#define GL_DEPRECATED_PERMIT

class GL
{
public:
	
#ifdef GL_DEPRECATED_PERMIT
	
	/*
	 * Not intended to be used
	*/

	static void Begin(GLenum  mode);
	static void End();

#endif

	/*
	 * Viewport
	*/

	static void Viewport(GLint x,  GLint y,  GLsizei width,  GLsizei height); 

	/*
	 * Scissor
	*/

	static void Scissor(GLint x, GLint y, GLsizei width, GLsizei height);

	/*
	 * Frame Buffer
	*/

	static void Clear(GLbitfield  mask);
	static void ClearColor(GLclampf red,  GLclampf green,  GLclampf blue,  GLclampf alpha); 
	static void ColorMask(GLboolean red,  GLboolean green,  GLboolean blue,  GLboolean alpha);
	static void FramebufferTexture (GLenum target, GLenum attachment, GLuint texture, GLint level);
	static void FramebufferTexture2D(GLenum target,  GLenum attachment,  GLenum textarget,  GLuint texture,  GLint level);
	static void DrawBuffer(GLenum buf);
	static void DrawBuffers(GLsizei n, const GLenum *bufs);
	static void ReadBuffer(GLenum mode);
	static void BindFramebuffer(GLenum target,  GLuint framebuffer);
	static void BlitFramebuffer(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, 
		GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
	static GLenum CheckFramebufferStatus(GLenum target);

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

	static void DrawArrays(GLenum mode, GLint first, GLsizei count);
	static void DrawElements (GLenum mode, GLsizei count, GLenum type, const void* indices);
	static void DrawElementsInstanced (GLenum mode, GLsizei count, GLenum type, const void* indices, GLsizei primcount);

	// Buffers
	static void BufferData (GLenum target, GLsizeiptr size, const GLvoid * data, GLenum usage);
	static void BufferSubData (GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid *data);
	static void GetBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, void * data);

	/*
	 * Vertex Attributes
	*/

	static void EnableVertexAttribArray (GLuint index);
	static void VertexAttribPointer (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid * pointer);
	static void VertexAttribIPointer (GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid * pointer);	
	static void VertexAttribDivisor (GLuint index, GLuint divisor);
	static void VertexAttrib1f(GLuint index,  GLfloat v0); 
	static void VertexAttrib2f(GLuint index,  GLfloat v0,  GLfloat v1); 
	static void VertexAttrib3f(GLuint index,  GLfloat v0,  GLfloat v1,  GLfloat v2);
	static void VertexAttrib4f(GLuint index,  GLfloat v0,  GLfloat v1,  GLfloat v2,  GLfloat v3); 
	static void VertexAttrib1fv(GLuint index,  const GLfloat *v); 
	static void VertexAttrib2fv(GLuint index,  const GLfloat *v); 
	static void VertexAttrib3fv(GLuint index,  const GLfloat *v); 
	static void VertexAttrib4fv(GLuint index,  const GLfloat *v);

	// Bind
	static void BindVertexArray (GLuint array);
	static void BindBuffer (GLenum target, GLuint buffer);
	static void BindBufferBase(GLenum target, GLuint index, GLuint buffer);
	static void UniformBlockBinding(GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding);
	static void ShaderStorageBlockBinding(GLuint program, GLuint storageBlockIndex, GLuint storageBlockBinding);

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
	static void StencilOpSeparate(GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass);
	static void StencilMask(GLuint mask);

	// Blend
	static void BlendFunc (GLenum sfactor, GLenum dfactor);
	static void BlendFunci (GLuint buf, GLenum sfactor, GLenum dfactor);
	static void BlendEquation (GLenum mode);

	// Generators
	static void GenVertexArrays (GLsizei n, GLuint * arrays);
	static void GenBuffers(GLsizei n,  GLuint * buffers);
	static void GenTextures(GLsizei n,  GLuint * textures);
	static void GenFramebuffers(GLsizei n,  GLuint * framebuffers);

	// Textures
	static void BindTexture(GLenum target, GLuint texture);
	static void ActiveTexture(GLenum texture);
	static void TextureView(GLuint texture, GLenum target, GLuint origtexture, GLenum internalformat,
		GLuint minlevel, GLuint numlevels, GLuint minlayer, GLuint numlayers);

	static void TexImage2D(GLenum target,  GLint level,  GLint internalformat,  GLsizei width,  
		GLsizei height,  GLint border,  GLenum format,  GLenum type,  const GLvoid * data); 
	static void TexImage3D(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, 
		GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid * data);

	static void TexStorage2D(GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);

	static void TexEnvi(GLenum target,  GLenum pname,  GLint param);
	static void TexEnvf(GLenum target,  GLenum pname,  GLfloat param);
	static void TexParameteri(GLenum target,  GLenum pname,  GLint param);
	static void TexParameterf(GLenum target,  GLenum pname,  GLfloat param);
	static void TexParameteriv(GLenum target, GLenum pname, const GLint * params);
	static void TexParameterfv(GLenum target, GLenum pname, const GLfloat * params);
	static void GenerateMipmap(GLenum target);
	static void GetTexImage(GLenum target, GLint level, GLenum format, GLenum type, GLvoid * pixels);
	static void GetTexLevelParameteriv(GLenum target, GLint level, GLenum pname, GLint * params);

	/*
	 * Pixels
	*/

	static void PixelStorei(GLenum pname, GLint param);

	/*
	 * Image Textures
	*/

	static void BindImageTexture (GLuint unit, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format);

	/*
	 * Shaders
	*/

	static GLuint CreateShader(GLenum shaderType);
	static void DeleteShader(GLuint shader);
	static void ShaderSource(GLuint shader, GLsizei count, const GLchar **string, const GLint *length);	
	static void CompileShader(GLuint shader);
	static void GetShaderInfoLog(GLuint  shader,  GLsizei  maxLength,  GLsizei * length,  GLchar * infoLog);
	static void GetShaderiv(GLuint shader, GLenum pname, GLint *params);

	static GLuint CreateProgram(void);
	static void DeleteProgram(GLuint program);
	static void UseProgram (GLuint program);
	static void LinkProgram(GLuint program);

	static void AttachShader(GLuint program, GLuint shader);
	static void DetachShader(GLuint program, GLuint shader);
	static GLint GetUniformLocation(GLuint program, const GLchar *name);
	static GLuint GetUniformBlockIndex(GLuint program, const GLchar *uniformBlockName);
	static GLuint GetProgramResourceIndex(GLuint program, GLenum programInterface, const char * name);

	static void DispatchCompute(GLuint num_groups_x,GLuint num_groups_y,GLuint num_groups_z);

	/*
	 * Uniforms
	*/

	static void Uniform1f(GLint location,  GLfloat v0);
	static void Uniform2f(GLint location,  GLfloat v0,  GLfloat v1);
	static void Uniform3f(GLint location,  GLfloat v0,  GLfloat v1,  GLfloat v2);
	static void Uniform4f(GLint location,  GLfloat v0,  GLfloat v1,  GLfloat v2,  GLfloat v3);
	static void Uniform1i(GLint location,  GLint v0); 
	static void Uniform2i(GLint location,  GLint v0,  GLint v1);
	static void Uniform3i(GLint location,  GLint v0,  GLint v1,  GLint v2); 
	static void Uniform4i(GLint location,  GLint v0,  GLint v1,  GLint v2,  GLint v3); 
	static void Uniform1ui(GLint location, GLuint v0);
	static void Uniform2ui(GLint location, GLuint v0, GLuint v1);
	static void Uniform3ui(GLint location, GLuint v0, GLuint v1, GLuint v2); 
	static void Uniform4ui(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
	static void Uniform1fv(GLint location, GLsizei count, const GLfloat *value);
	static void Uniform2fv(GLint location, GLsizei count, const GLfloat *value);
	static void Uniform3fv(GLint location, GLsizei count, const GLfloat *value);
	static void Uniform4fv(GLint location, GLsizei count, const GLfloat *value);
	static void Uniform1iv(GLint location, GLsizei count, const GLint *value);
	static void Uniform2iv(GLint location, GLsizei count, const GLint *value);
	static void Uniform3iv(GLint location, GLsizei count, const GLint *value);
	static void Uniform4iv(GLint location, GLsizei count, const GLint *value);
	static void Uniform1uiv(GLint location, GLsizei count, const GLuint *value);
	static void Uniform2uiv(GLint location, GLsizei count, const GLuint *value);
	static void Uniform3uiv(GLint location, GLsizei count, const GLuint *value);
	static void Uniform4uiv(GLint location, GLsizei count, const GLuint *value);
	static void UniformMatrix2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
	static void UniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
	static void UniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value); 
	static void UniformMatrix2x3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value); 
	static void UniformMatrix3x2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
	static void UniformMatrix2x4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value); 
	static void UniformMatrix4x2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value); 
	static void UniformMatrix3x4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
	static void UniformMatrix4x3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);

	/*
	 * Memory
	*/

	static void MemoryBarrier(GLbitfield barriers);

	/*
	 * Queries
	*/

	static void GenQueries(GLsizei n, GLuint * ids);
	static void DeleteQueries(GLsizei n, const GLuint * ids);
	static void QueryCounter(GLuint id, GLenum target);
	static void GetQueryObjectiv(GLuint id, GLenum pname, GLint * params);
	static void GetQueryObjectui64v(GLuint id, GLenum pname, GLuint64 * params);

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