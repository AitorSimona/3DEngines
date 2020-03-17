#ifndef __RESOURCE_FONT_H__
#define __RESOURCE_FONT_H__

#include "Resource.h"
#include <map>
#include "Math.h"

BE_BEGIN_NAMESPACE

class BROKEN_API ResourceFont : public Resource {

public:
	struct Character {
		GLuint	TextureID;  // ID handle of the glyph texture
		float2	Size;       // Size of glyph
		float2	Bearing;    // Offset from baseline to left/top of glyph
		GLuint	Advance;    // Offset to advance to next glyph
	};

	ResourceFont(uint UID, const char* source_file);
	~ResourceFont();

	bool LoadInMemory() override;
	void FreeMemory() override;

	std::map<GLchar, Character> characters;
	float size = 0.02f;

	GLuint VAO = 0;
	GLuint VBO = 0;
	GLuint texture = 0;

	

private:
	void OnOverwrite() override;
	void OnDelete() override;
};
BE_END_NAMESPACE
#endif //__RESOURCE_FONT_H__