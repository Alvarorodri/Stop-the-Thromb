#version 330

uniform vec4 color;
uniform sampler2D tex;

in vec2 texCoordFrag;
out vec4 outColor;

void main()
{
	// Discard fragment if texture sample has alpha < 0.5
	// otherwise compose the texture sample with the fragment's interpolated color
	vec4 texColor = texture(tex, texCoordFrag);
	if(texColor.y >= 1.0f && texColor.x <= 0.0f && texColor.z <= 0.0f || texColor.a < 0.5f) discard;
	outColor = color * texColor;
}

