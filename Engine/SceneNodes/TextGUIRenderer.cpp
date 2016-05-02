#include "TextGUIRenderer.h"

#include "Wrappers/OpenGL/GL.h"

#include "Core/Math/glm/glm.hpp"

#include "Renderer/Pipeline.h"
#include "Systems/Screen/Screen.h"

#include "Managers/TextManager.h"
#include "Managers/ShaderManager.h"

#include "Core/Console/Console.h"

TextGUIRenderer::TextGUIRenderer () :
	Model3DRenderer ()
{
	CreateVBO (_bufferObject, GetBufferAttributes ());
}

TextGUIRenderer::TextGUIRenderer (Transform* transform) :
	Model3DRenderer (transform)
{
	CreateVBO (_bufferObject, GetBufferAttributes ());
}

TextGUIRenderer::~TextGUIRenderer ()
{
	delete _font;	
}

void TextGUIRenderer::Draw ()
{
	bool cull, depth;
	GL::IsEnabled (GL_CULL_FACE, &cull);
	GL::IsEnabled (GL_DEPTH_TEST, &depth);
	GL::Disable (GL_CULL_FACE);
	GL::Disable (GL_DEPTH_TEST);

	std::vector<PipelineAttribute> uniformAttributes = GetUniformAttributes (_font);

	if (_isDirty) {
		UpdateText ();
		FeedVBO (_bufferObject, _buffer, _iBuf);

		_isDirty = false;
	}

	std::string shaderName = TextManager::Instance ()->GetShaderName ();

	Pipeline::SetShader (ShaderManager::Instance ()->GetShader (shaderName));
	Pipeline::SendCustomAttributes (shaderName, uniformAttributes);
	
	GL::BindVertexArray (_bufferObject.VAO_INDEX);
	GL::DrawElements(GL_TRIANGLES, _bufferObject.INDEX_COUNT, GL_UNSIGNED_INT, 0);
	
	if (cull) {
		GL::Enable (GL_CULL_FACE);
	}

	if (depth) {
		GL::Enable (GL_DEPTH_TEST);
	}
}

void TextGUIRenderer::CreateVBO (BufferObject& bufferObject, const std::vector<BufferAttribute>& bufferAttributes)
{
	std::size_t charLimit = TextManager::Instance ()->GetCharLimits ();

	// Create VAO
	GL::GenVertexArrays (1, &bufferObject.VAO_INDEX);
	GL::BindVertexArray (bufferObject.VAO_INDEX);

	// Create VBO
	GL::GenBuffers(1, &bufferObject.VBO_INDEX);
	GL::BindBuffer(GL_ARRAY_BUFFER, bufferObject.VBO_INDEX);
	GL::BufferData(GL_ARRAY_BUFFER, bufferAttributes [0].stride * charLimit, NULL, GL_DYNAMIC_DRAW);

	// Create IBO
	glGenBuffers(1, &bufferObject.IBO_INDEX);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferObject.IBO_INDEX);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, bufferAttributes [0].stride * charLimit, NULL, GL_DYNAMIC_DRAW);

	for (BufferAttribute attr : bufferAttributes) {
		GL::EnableVertexAttribArray (attr.index);
		GL::VertexAttribPointer (attr.index, attr.size, attr.type, GL_FALSE, attr.stride, (void*) attr.pointer);
	}
}

void TextGUIRenderer::FeedVBO (BufferObject& bufferObject, const Buffer<float>& buffer, const Buffer<unsigned int>& iBuf)
{
	GL::BindVertexArray(bufferObject.VAO_INDEX);

	// VBO Update
	GL::BindBuffer(GL_ARRAY_BUFFER, bufferObject.VBO_INDEX);
	GL::BufferSubData(GL_ARRAY_BUFFER, 0, buffer.GetBytesCount (), buffer.GetPointer ());

	// IBO Update
	GL::BindBuffer (GL_ELEMENT_ARRAY_BUFFER, bufferObject.IBO_INDEX);
	GL::BufferSubData (GL_ELEMENT_ARRAY_BUFFER, 0, iBuf.GetBytesCount (), iBuf.GetPointer ());

	bufferObject.INDEX_COUNT = iBuf.GetSize ();
}

std::vector<BufferAttribute> TextGUIRenderer::GetBufferAttributes ()
{
	std::vector<BufferAttribute> attributes;

	for (int i=0;i<2;i++) {
		BufferAttribute attribute;

		attribute.index = i;
		attribute.size = 2;
		attribute.type = GL_FLOAT;
		attribute.stride = 4 * sizeof (float);
		attribute.pointer = i * 2 * sizeof (float);

		attributes.push_back (attribute);
	}

	return attributes;
}

std::vector<PipelineAttribute> TextGUIRenderer::GetUniformAttributes (Font* font)
{
	// Create attribute
	std::vector<PipelineAttribute> attributes;

	PipelineAttribute fontTexture;

	fontTexture.type = PipelineAttribute::AttrType::ATTR_TEXTURE_2D;

	fontTexture.name = "FontTexture";

	fontTexture.value.x = font->GetTexture (0)->GetGPUIndex ();

	attributes.push_back (fontTexture);

	return attributes;
}

void TextGUIRenderer::UpdateText ()
{
	this->Clear ();

	_buffer = CharBufferData (_text, _font, glm::vec2 (_transform->GetScale ().x, _transform->GetScale ().y));
	_iBuf = CharIndexData (_text.size ());
}

Buffer<float> TextGUIRenderer::CharBufferData (const std::string& text, Font* font, const glm::vec2& scale)
{
	Buffer<float> buffer;

	glm::vec2 screenPos = glm::vec2 (_screenPos.x, _screenPos.y);

	for (std::size_t i=0;i<text.size ();i++) {
		FontChar* ch = font->GetChar (text [i]);

		buffer.Add (CharData (ch, screenPos, scale));

		float xAdvance = 1.0f * ch->GetXAdvance () / Screen::Instance ()->GetWidth () * scale.x;
		screenPos.x += xAdvance;
	}

	return buffer;
}

Buffer<unsigned int> TextGUIRenderer::CharIndexData (std::size_t textSize)
{
	Buffer<unsigned int> iBuf;

	for (std::size_t i=0;i<textSize;i++) {
		std::size_t upLeft = i * 4, upRight = i * 4 + 1,
			downLeft = i * 4 + 2, downRight = i * 4 + 3;
		iBuf.Add (upLeft); iBuf.Add (downLeft); iBuf.Add (upRight);
		iBuf.Add (upRight); iBuf.Add (downLeft); iBuf.Add (downRight);
	}

	return iBuf;
}

Buffer<float> TextGUIRenderer::CharData (FontChar* ch, const glm::vec2& screenPos, const glm::vec2& fontSize)
{
	Buffer<float> bufferData;

	Texture* texture = _font->GetTexture (0);
	glm::vec2 texSize (texture->GetSize ().x, texture->GetSize ().y);
	glm::vec2 screenSize (Screen::Instance ()->GetSize ());
	glm::vec2 scale = (glm::vec2) ch->GetSize () / screenSize * fontSize;
	glm::vec2 offset = glm::vec2(ch->GetOffset ().x, -ch->GetOffset ().y) / screenSize * fontSize;

	glm::vec2 upLeft = screenPos + offset;
	glm::vec2 upRight = screenPos + glm::vec2 (scale.x, 0) + offset;
	glm::vec2 downLeft = screenPos + glm::vec2 (0, -scale.y) + offset;
	glm::vec2 downRight = screenPos + glm::vec2 (scale.x, -scale.y) + offset;

	glm::vec2 tUpLeft = ((glm::vec2) ch->GetPosition ()) / texSize;
	glm::vec2 tUpRight = tUpLeft + (glm::vec2 (ch->GetSize ().x, 0)) / texSize;
	glm::vec2 tDownLeft = tUpLeft + (glm::vec2 (0, ch->GetSize ().y)) / texSize;
	glm::vec2 tDownRight = tDownLeft + (glm::vec2 (ch->GetSize ().x, 0)) / texSize;

	bufferData.Add (upLeft.x); bufferData.Add (upLeft.y);
	bufferData.Add (tUpLeft.x); bufferData.Add (tUpLeft.y);
	bufferData.Add (upRight.x); bufferData.Add (upRight.y);
	bufferData.Add (tUpRight.x); bufferData.Add (tUpRight.y);
	bufferData.Add (downLeft.x); bufferData.Add (downLeft.y);
	bufferData.Add (tDownLeft.x); bufferData.Add (tDownLeft.y);
	bufferData.Add (downRight.x); bufferData.Add (downRight.y);
	bufferData.Add (tDownRight.x); bufferData.Add (tDownRight.y);

	return bufferData;
}

void TextGUIRenderer::UpdateText (const std::string& text, Font* font,
	Vector3 screenPos, float lineLength)
{
	_text = text;
	_font = font;
	_screenPos = screenPos;
	_screenPos = Vector3 (screenPos.x, 1.0 - screenPos.y);
	_screenPos = _screenPos * 2 - Vector3::One;
	_lineLength = lineLength;
	_isDirty = true;
}

void TextGUIRenderer::Clear ()
{
	_buffer.Clear ();
	_iBuf.Clear ();
}