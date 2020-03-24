#ifndef __RESOURCE_SHADER_H__
#define __RESOURCE_SHADER_H__

#include "Resource.h"
#include "Math.h"

BE_BEGIN_NAMESPACE
union BROKEN_API data
{
	data() {
		intU = 0;
		floatU = 0;
		vec2U = { 0,0 };
		vec3U = { 0,0,0 };
		vec4U = { 0,0,0,0 };
	}

	int intU;
	float floatU;
	float2 vec2U;
	float3 vec3U;
	float4 vec4U;
};

struct BROKEN_API Uniform 
{
	Uniform() {};

	std::string name;
	uint location = 0;
	uint type; // GLenum

	data value;
};

class BROKEN_API ResourceShader : public Resource 
{
public:
	ResourceShader(uint UID, const char* source_file);
	~ResourceShader();

	bool LoadInMemory() override;
	void FreeMemory() override;

	//// constructor reads and builds the shader
	//ResourceShader(const char* vertexPath, const char* fragmentPath, bool is_extern = true);
	//ResourceShader(const char* binary, uint size, uint format, const char* name, const char* vertexPath, const char* fragmentPath);

	// --- Getters ---
	void GetAllUniforms(std::vector<Uniform*>& uniforms);

	// --- Setters ---
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;

	// --- Utilities ---
	void use();
	void ReloadAndCompileShader();

public:
	// the program ID
	unsigned int ID = 0;
	bool binary = false;
	std::string ShaderCode;
	std::string vShaderCode;
	std::string fShaderCode;
private:
	unsigned int vertex, fragment = 0;

	bool CreateVertexShader(unsigned int& vertex, const char* vShaderCode);
	bool CreateFragmentShader(unsigned int& fragment, const char* fShaderCode);
	bool CreateShaderProgram(unsigned int vertex, unsigned int fragment);
	bool CreateShaderProgram();
	void DeleteShaderProgram();
	void FillUniform(Uniform* uniform, const char* name, const uint type) const;
private:
	void OnOverwrite() override;
	void OnDelete() override;
};
BE_END_NAMESPACE
#endif //__RESOURCE_SHADER_H__
