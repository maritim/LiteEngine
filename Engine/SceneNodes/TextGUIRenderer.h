#ifndef TEXTGUIRENDERER_H
#define TEXTGUIRENDERER_H

#define CHAR_LIMIT 2000

#include "Model3DRenderer.h"

#include <string>

#include "Core/Math/Vector3.h"

#include "Fonts/Font.h"

#include "Renderer/Buffer.h"
#include "Renderer/BufferAttribute.h"
#include "Renderer/PipelineAttribute.h"

class TextGUIRenderer : public Model3DRenderer
{
protected:
	bool _isDirty;
	
	std::string _text;
	Font* _font;
	Vector3 _screenPos;
	float _lineLength;

	Buffer<float> _buffer;
	Buffer<unsigned int> _iBuf;
	BufferObject _bufferObject;

public:
	TextGUIRenderer ();
	TextGUIRenderer (Transform* transform);
	virtual ~TextGUIRenderer ();

	virtual void Draw ();

	virtual void UpdateText (const std::string& text, Font* font,
		Vector3 screenPos, float lineLength);
private:
	void CreateVBO (BufferObject&, const std::vector<BufferAttribute>&);
	void FeedVBO (BufferObject&, const Buffer<float>&, const Buffer<unsigned int>&);

	std::vector<BufferAttribute> GetBufferAttributes ();
	std::vector<PipelineAttribute> GetUniformAttributes (Font* font);

	Buffer<float> CharBufferData (const std::string&, Font*, const glm::vec2&);
	Buffer<unsigned int> CharIndexData (std::size_t);

	Buffer<float> CharData (FontChar* ch, const glm::vec2& screenPos, const glm::vec2& fontSize);

	void UpdateText ();
	void Clear ();
};

#endif