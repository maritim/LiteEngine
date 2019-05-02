#include "GL.h"

#include "Core/Console/Console.h"

#ifdef GL_DEPRECATED_PERMIT

/*
* Not intended to be used
*/

void GL::Begin(GLenum  mode)
{
	glBegin (mode);

	ErrorCheck ("glBegin");
}

void GL::End()
{
	glEnd();

	ErrorCheck ("glEnd");
}

#endif

/*
 * Viewport
*/

void GL::Viewport(GLint x,  GLint y,  GLsizei width,  GLsizei height)
{
	glViewport(x, y, width, height); 

	ErrorCheck ("glViewport");
}

/*
 * Scissor
*/

void GL::Scissor(GLint x, GLint y, GLsizei width, GLsizei height)
{
	glScissor(x, y, width, height);

	ErrorCheck ("glScissor");
}

/*
 * Frame Buffer
*/

void GL::Clear(GLbitfield  mask)
{
	glClear (mask);

	ErrorCheck ("glClear");
}

void GL::ClearColor(GLclampf red,  GLclampf green,  GLclampf blue,  GLclampf alpha)
{
	glClearColor(red, green, blue, alpha);

	ErrorCheck ("glClearColor");
} 

void GL::ColorMask(GLboolean red,  GLboolean green,  GLboolean blue,  GLboolean alpha)
{
	glColorMask (red, green, blue, alpha);

	ErrorCheck ("glColorMask");
}

void GL::FramebufferTexture (GLenum target, GLenum attachment, GLuint texture, GLint level)
{
	glFramebufferTexture (target, attachment, texture, level);

	ErrorCheck ("glFramebufferTexture");
}

void GL::FramebufferTexture2D(GLenum target,  GLenum attachment,  GLenum textarget,  GLuint texture,  GLint level)
{
	glFramebufferTexture2D (target, attachment, textarget, texture, level);

	ErrorCheck ("glFramebufferTexture2D");
}

void GL::DrawBuffer(GLenum buf)
{
	glDrawBuffer (buf);

	ErrorCheck ("glDrawBuffer");
}

void GL::DrawBuffers(GLsizei n, const GLenum *bufs)
{
	glDrawBuffers (n, bufs);

	ErrorCheck ("glDrawBuffers");
}

void GL::ReadBuffer(GLenum mode)
{
	glReadBuffer (mode);

	ErrorCheck ("glReadBuffer");
}

void GL::BindFramebuffer(GLenum target,  GLuint framebuffer)
{
	glBindFramebuffer (target, framebuffer);

	ErrorCheck ("glBindFramebuffer");
}

void GL::BlitFramebuffer(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, 
	GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter)
{
	glBlitFramebuffer (srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, mask, filter);

	ErrorCheck ("glBlitFramebuffer");
}

GLenum GL::CheckFramebufferStatus(GLenum target)
{
	GLenum check = glCheckFramebufferStatus (target);

	ErrorCheck ("glCheckFramebufferStatus");

	return check;
}

/*
 * Behaviour
*/

void GL::Hint(GLenum target,  GLenum mode)
{
	glHint (target, mode);

	ErrorCheck ("glHint");
}

/* 
 * Culling
*/

void GL::CullFace(GLenum mode)
{
	glCullFace (mode);

	ErrorCheck ("glCullFace");
}

/*
 * Draw Calls
*/

void GL::DrawArrays (GLenum mode, GLint first, GLsizei count)
{
	glDrawArrays (mode, first, count);

	ErrorCheck ("glDrawArrays");
}

void GL::DrawElements (GLenum mode, GLsizei count, GLenum type, const void* indices)
{
	glDrawElements (mode, count, type, indices);

	ErrorCheck ("glDrawElements");
}

void GL::DrawElementsInstanced (GLenum mode, GLsizei count, GLenum type, const void* indices, GLsizei primcount)
{
	glDrawElementsInstanced (mode, count, type, indices, primcount);

	ErrorCheck ("glDrawElementsInstanced");
}

/*
 * Buffers
*/

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

/*
 * Vertex Attributes
*/

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

void GL::VertexAttribIPointer (GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid * pointer)
{
	glVertexAttribIPointer (index, size, type, stride, pointer);

	ErrorCheck ("glVertexAttribIPointer");
}

void GL::VertexAttribDivisor(GLuint index, GLuint divisor)
{
	glVertexAttribDivisor (index, divisor);

	ErrorCheck ("glVertexAttribDivisor");
}

void GL::VertexAttrib1f(GLuint index,  GLfloat v0)
{
	glVertexAttrib1f(index, v0);

	ErrorCheck ("glVertexAttrib1f");
}

void GL::VertexAttrib2f(GLuint index,  GLfloat v0,  GLfloat v1)
{
	glVertexAttrib2f (index, v0, v1);

	ErrorCheck ("glVertexAttrib2f");
}

void GL::VertexAttrib3f(GLuint index,  GLfloat v0,  GLfloat v1,  GLfloat v2)
{
	glVertexAttrib3f (index, v0, v1, v2);

	ErrorCheck ("glVertexAttrib3f");
}

void GL::VertexAttrib4f(GLuint index,  GLfloat v0,  GLfloat v1,  GLfloat v2,  GLfloat v3)
{
	glVertexAttrib4f (index, v0, v1, v2, v3);

	ErrorCheck ("glVertexAttrib4f");
}

void GL::VertexAttrib1fv(GLuint index,  const GLfloat *v)
{
	glVertexAttrib1fv (index, v);

	ErrorCheck ("glVertexAttrib1fv");
}

void GL::VertexAttrib2fv(GLuint index,  const GLfloat *v)
{
	glVertexAttrib2fv (index, v);

	ErrorCheck ("glVertexAttrib2fv");
}

void GL::VertexAttrib3fv(GLuint index,  const GLfloat *v)
{
	glVertexAttrib3fv (index, v);

	ErrorCheck ("glVertexAttrib3fv");
}

void GL::VertexAttrib4fv(GLuint index,  const GLfloat *v)
{
	glVertexAttrib4fv (index, v);

	ErrorCheck ("glVertexAttrib4fv");
}

/*
 * Bindings
*/

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

/*
 * Depth Buffer
*/

void GL::DepthMask (GLboolean flag)
{
	glDepthMask (flag);

	ErrorCheck ("glDepthMask");
}

void GL::DepthRange(GLclampd nearVal, GLclampd farVal)
{
	glDepthRange (nearVal, farVal);

	ErrorCheck ("glDepthRange");
}

void GL::ClearDepth(GLclampd  depth)
{
	glClearDepth (depth);

	ErrorCheck ("glClearDepth");
}

void GL::DepthFunc(GLenum func)
{
	glDepthFunc(func);

	ErrorCheck ("glDepthFunc");
}

/*
 * Stencil Buffer
*/

void GL::StencilFunc(GLenum func, GLint ref, GLuint mask)
{
	glStencilFunc (func, ref, mask);

	ErrorCheck ("glStencilFunc");
}

void GL::StencilOp(GLenum sfail, GLenum dpfail, GLenum dppass)
{
	glStencilOp (sfail, dpfail, dppass);

	ErrorCheck ("glStencilOp");
}

void GL::StencilOpSeparate(GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass)
{
	glStencilOpSeparate(face, sfail, dpfail, dppass);

	ErrorCheck ("glStencilOpSeparate");
}

void GL::StencilMask(GLuint mask)
{
	glStencilMask (mask);

	ErrorCheck ("glStencilMask");
}

/*
 * Blending
*/

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

void GL::BlendEquation (GLenum mode)
{
	glBlendEquation (mode);

	ErrorCheck ("glBlendEquation");
}

/*
 * Generators
*/

void GL::GenVertexArrays (GLsizei n, GLuint * arrays)
{
	glGenVertexArrays (n, arrays);

	ErrorCheck ("glGenVertexArrays");
}

void GL::GenBuffers(GLsizei n,  GLuint * buffers)
{
	glGenBuffers (n, buffers);

	ErrorCheck ("glGenBuffers");
}

void GL::GenTextures(GLsizei n,  GLuint * textures)
{
	glGenTextures (n, textures);

	ErrorCheck ("glGenTextures");
}

void GL::GenFramebuffers(GLsizei n,  GLuint * framebuffers)
{
	glGenFramebuffers (n, framebuffers);

	ErrorCheck ("glGenFramebuffers");
}

/*
 * Uniforms
*/

void GL::Uniform1f(GLint location,  GLfloat v0)
{
	glUniform1f(location, v0);

	ErrorCheck ("glUniform1f");
}

void GL::Uniform2f(GLint location,  GLfloat v0,  GLfloat v1)
{
	glUniform2f(location, v0, v1);

	ErrorCheck ("glUniform2f");
}

void GL::Uniform3f(GLint location,  GLfloat v0,  GLfloat v1,  GLfloat v2)
{
	glUniform3f(location, v0, v1, v2);

	ErrorCheck ("glUniform3f");
}

void GL::Uniform4f(GLint location,  GLfloat v0,  GLfloat v1,  GLfloat v2,  GLfloat v3)
{
	glUniform4f (location, v0, v1, v2, v3);

	ErrorCheck ("glUniform4f");
}

void GL::Uniform1i(GLint location,  GLint v0)
{
	glUniform1i (location, v0); 

	ErrorCheck ("glUniform1i");
}

void GL::Uniform2i(GLint location,  GLint v0,  GLint v1)
{
	glUniform2i(location, v0, v1);

	ErrorCheck ("glUniform2i");
}

void GL::Uniform3i(GLint location,  GLint v0,  GLint v1,  GLint v2)
{
	glUniform3i(location, v0, v1, v2); 

	ErrorCheck ("glUniform3i");
}

void GL::Uniform4i(GLint location,  GLint v0,  GLint v1,  GLint v2,  GLint v3)
{
	glUniform4i(location, v0, v1, v2, v3);

	ErrorCheck ("glUniform4i");
}

void GL::Uniform1ui(GLint location, GLuint v0)
{
	glUniform1ui(location, v0);

	ErrorCheck ("glUniform1ui");
}

void GL::Uniform2ui(GLint location, GLuint v0, GLuint v1)
{
	glUniform2ui(location, v0, v1);

	ErrorCheck ("glUniform2ui");
}

void GL::Uniform3ui(GLint location, GLuint v0, GLuint v1, GLuint v2)
{
	glUniform3ui(location, v0, v1, v2);

	ErrorCheck ("glUniform3ui");
}

void GL::Uniform4ui(GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3)
{
	glUniform4ui(location, v0, v1, v2, v3);

	ErrorCheck ("glUniform4ui");
}

void GL::Uniform1fv(GLint location, GLsizei count, const GLfloat *value)
{
	glUniform1fv(location, count, value);

	ErrorCheck ("glUniform1fv");
}

void GL::Uniform2fv(GLint location, GLsizei count, const GLfloat *value)
{
	glUniform2fv(location, count, value);

	ErrorCheck ("glUniform2fv");
}

void GL::Uniform3fv(GLint location, GLsizei count, const GLfloat *value)
{
	glUniform3fv(location, count, value);

	ErrorCheck ("glUniform3fv");
}

void GL::Uniform4fv(GLint location, GLsizei count, const GLfloat *value)
{
	glUniform4fv (location, count, value);

	ErrorCheck ("glUniform4fv");
}

void GL::Uniform1iv(GLint location, GLsizei count, const GLint *value)
{
	glUniform1iv (location, count, value);

	ErrorCheck ("glUniform1iv");
}

void GL::Uniform2iv(GLint location, GLsizei count, const GLint *value)
{
	glUniform2iv (location, count, value);

	ErrorCheck ("glUniform2iv");
}

void GL::Uniform3iv(GLint location, GLsizei count, const GLint *value)
{
	glUniform3iv (location, count, value);

	ErrorCheck ("glUniform3iv");
}

void GL::Uniform4iv(GLint location, GLsizei count, const GLint *value)
{
	glUniform4iv (location, count, value);

	ErrorCheck ("glUniform4iv");
}

void GL::Uniform1uiv(GLint location, GLsizei count, const GLuint *value)
{
	glUniform1uiv (location, count, value);

	ErrorCheck ("glUniform1uiv");
}

void GL::Uniform2uiv(GLint location, GLsizei count, const GLuint *value)
{
	glUniform2uiv (location, count, value);

	ErrorCheck ("glUniform2uiv");
}

void GL::Uniform3uiv(GLint location, GLsizei count, const GLuint *value)
{
	glUniform3uiv (location, count, value);

	ErrorCheck ("glUniform3uiv");
}

void GL::Uniform4uiv(GLint location, GLsizei count, const GLuint *value)
{
	glUniform4uiv (location, count, value);

	ErrorCheck ("glUniform4uiv");
}

void GL::UniformMatrix2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
	glUniformMatrix2fv(location, count, transpose, value);

	ErrorCheck ("glUniformMatrix2fv");
}

void GL::UniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
	glUniformMatrix3fv(location, count, transpose, value);

	ErrorCheck ("glUniformMatrix3fv");
}

void GL::UniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
	glUniformMatrix4fv(location, count, transpose, value); 

	ErrorCheck ("glUniformMatrix4fv");
}

void GL::UniformMatrix2x3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
	glUniformMatrix2x3fv(location, count, transpose, value); 

	ErrorCheck ("glUniformMatrix2x3fv");
}

void GL::UniformMatrix3x2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
	glUniformMatrix3x2fv(location, count, transpose, value);

	ErrorCheck ("glUniformMatrix3x2fv");
}

void GL::UniformMatrix2x4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
	glUniformMatrix2x4fv(location, count, transpose, value); 

	ErrorCheck ("glUniformMatrix2x4fv");
}

void GL::UniformMatrix4x2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
	glUniformMatrix4x2fv(location, count, transpose, value);

	ErrorCheck ("glUniformMatrix4x2fv");
} 

void GL::UniformMatrix3x4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
	glUniformMatrix3x4fv(location, count, transpose, value);

	ErrorCheck ("glUniformMatrix3x4fv");
}

void GL::UniformMatrix4x3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value)
{
	glUniformMatrix4x3fv(location, count, transpose, value);

	ErrorCheck ("glUniformMatrix4x3fv");
}

/*
 * Memory
*/

void GL::MemoryBarrier (GLbitfield barriers)
{
	glMemoryBarrier (barriers);

	ErrorCheck ("glMemoryBarrier");
}

/*
 * Capabilities
*/

void GL::Enable (GLenum cap)
{
	glEnable (cap);

	ErrorCheck ("glEnable");
}

void GL::Disable (GLenum cap)
{
	glDisable (cap);

	ErrorCheck ("glDisable");
}

void GL::IsEnabled(GLenum cap, bool *val)
{
	*val = glIsEnabled (cap);

	ErrorCheck ("glIsEnabled");
}

/*
 * Textures
*/

void GL::BindTexture(GLenum target, GLuint texture)
{
	glBindTexture (target, texture);

	ErrorCheck ("glBindTexture");
}

void GL::ActiveTexture(GLenum texture)
{
	glActiveTexture (texture);

	ErrorCheck ("glActiveTexture");
}


void GL::TexImage2D(GLenum target,  GLint level,  GLint internalformat,  
	GLsizei width,  GLsizei height,  GLint border,  GLenum format,  
	GLenum type,  const GLvoid * data)
{
	glTexImage2D (target, level, internalformat, width, height, border,
		format, type, data);

	ErrorCheck ("glTexImage2D");
}

void GL::TexImage3D(GLenum target, GLint level, GLint internalFormat, 
	GLsizei width, GLsizei height, GLsizei depth, GLint border, 
	GLenum format, GLenum type, const GLvoid * data)
{
	glTexImage3D(target, level, internalFormat, width, height, depth, 
		border, format, type, data);

	ErrorCheck ("glTexImage3D");
}

void GL::TexEnvi(GLenum target,  GLenum pname,  GLint param)
{
	glTexEnvi (target, pname, param);

	ErrorCheck ("glTexEnvi");
}

void GL::TexEnvf(GLenum target,  GLenum pname,  GLfloat param)
{
	glTexEnvf (target, pname, param);

	ErrorCheck ("glTexEnvf");
}

void GL::TexParameteri(GLenum target,  GLenum pname,  GLint param)
{
	glTexParameteri(target, pname, param); 

	ErrorCheck ("glTexParameteri");
}

void GL::TexParameterf(GLenum target,  GLenum pname,  GLfloat param)
{
	glTexParameterf (target, pname, param);

	ErrorCheck ("glTexParameterf");
}

void GL::TexParameteriv(GLenum target, GLenum pname, const GLint * params)
{
	glTexParameteriv(target, pname, params);

	ErrorCheck("glTexParameteriv");
}

void GL::TexParameterfv(GLenum target, GLenum pname, const GLfloat * params)
{
	glTexParameterfv(target, pname, params);

	ErrorCheck ("glTexParameterfv");
}

void GL::GenerateMipmap (GLenum target)
{
	glGenerateMipmap(target);

	ErrorCheck ("glGenerateMipmap");
}

/*
 * Pixels
*/

void GL::PixelStorei(GLenum pname, GLint param)
{
	glPixelStorei(pname, param);

	ErrorCheck ("glPixelStorei");
}

/*
 * Image Textures
*/

void GL::BindImageTexture (GLuint unit, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format)
{
	glBindImageTexture (unit, texture, level, layered, layer, access, format);

	ErrorCheck ("glBindImageTexture");
}

/*
 * Shaders
*/

GLuint GL::CreateShader (GLenum shaderType)
{
	GLuint shader = glCreateShader(shaderType);

	ErrorCheck ("glCreateShader");

	return shader;
}

void GL::DeleteShader(GLuint shader)
{
	glDeleteShader (shader);

	ErrorCheck ("glDeleteShader");
}

void GL::ShaderSource (GLuint shader, GLsizei count, const GLchar **string, const GLint *length)
{
	glShaderSource(shader, count, string, length);	

	ErrorCheck ("glShaderSource");
}

void GL::CompileShader(GLuint shader)
{
	glCompileShader (shader);

	ErrorCheck ("glCompileShader");
}

void GL::GetShaderInfoLog(GLuint  shader,  GLsizei  maxLength,  GLsizei * length,  GLchar * infoLog)
{
	glGetShaderInfoLog(shader, maxLength, length, infoLog);

	ErrorCheck ("glGetShaderInfoLog");
}

GLuint GL::CreateProgram(void)
{
	GLuint program = glCreateProgram ();

	ErrorCheck ("glCreateProgram");

	return program;
}

void GL::DeleteProgram(GLuint program)
{
	glDeleteProgram (program);

	ErrorCheck ("glDeleteProgram");
}

void GL::UseProgram (GLuint program)
{
	glUseProgram (program);

	ErrorCheck ("glUseProgram");
}

void GL::LinkProgram(GLuint program)
{
	glLinkProgram (program);

	ErrorCheck ("glLinkProgram");
}

void GL::AttachShader(GLuint program, GLuint shader)
{
	glAttachShader(program, shader);

	ErrorCheck ("glAttachShader");
}

void GL::DetachShader(GLuint program, GLuint shader)
{
	glDetachShader (program, shader);

	ErrorCheck ("glDetachShader");
}


GLint GL::GetUniformLocation(GLuint program, const GLchar *name)
{
	GLint uniformLocation = glGetUniformLocation(program, name);

	ErrorCheck ("glGetUniformLocation");

	return uniformLocation;
}

void GL::DispatchCompute(GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z)
{
	glDispatchCompute(num_groups_x, num_groups_y, num_groups_z);

	ErrorCheck ("glDispatchCompute");
}

/*
 * Getters
*/

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

/*
 * Cleaning
*/

void GL::DeleteVertexArrays(GLsizei n, const GLuint *arrays)
{
	glDeleteVertexArrays(n, arrays);

	ErrorCheck ("glDeleteVertexArrays");
}

void GL::DeleteBuffers (GLsizei n, const GLuint* arrays)
{
	glDeleteBuffers (n, arrays);

	ErrorCheck ("glDeleteBuffers");
}

void GL::DeleteFramebuffers(GLsizei n, const GLuint * framebuffers)
{
	glDeleteFramebuffers (n, framebuffers);

	ErrorCheck ("glDeleteFramebuffers");
}

void GL::DeleteTextures(GLsizei n, const GLuint * textures)
{
	glDeleteTextures (n, textures);

	ErrorCheck ("glDeleteTextures");
}

void GL::Check ()
{
	ErrorCheck ("Custom Query");
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