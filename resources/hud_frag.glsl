#version 330 core

in vec2 texCoord;
out vec4 color;
uniform sampler2D texBuf;

void main(){

	vec4 c = texture( texBuf, texCoord );
	color = c;
}
