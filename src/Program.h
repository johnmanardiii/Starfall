
#pragma  once

#ifndef LAB471_PROGRAM_H_INCLUDED
#define LAB471_PROGRAM_H_INCLUDED

#include <map>
#include <string>

#include <glad/glad.h>


std::string readFileAsString(const std::string &fileName);

class Program
{

public:

	void setVerbose(const bool v) { verbose = v; }
	bool isVerbose() const { return verbose; }

	void setShaderNames(const std::string &v, const std::string &f);
	void setShaderNames(const std::string& v, const std::string& f, const std::string& g);
	void setShaderNames(std::string& v, std::string& f, std::string tc = "", std::string te = "");
	void setTransformFeedback(bool val) { usingTransformFeedback = val; }
	virtual bool Init();
	virtual void bind();
	virtual void unbind();

	void addAttribute(const std::string &name);
	void addUniform(const std::string &name);
	GLint getAttribute(const std::string &name) const;
	GLint getUniform(const std::string &name) const;
	GLuint pid = 0;
	
protected:

	std::string vShaderName;
	std::string fShaderName;
	std::string gShaderName = "";
	std::string teShaderName;
	std::string tcShaderName;
private:

	std::map<std::string, GLint> attributes;
	std::map<std::string, GLint> uniforms;
	bool verbose = true;
	bool usingTransformFeedback = false;
};

#endif // LAB471_PROGRAM_H_INCLUDED
