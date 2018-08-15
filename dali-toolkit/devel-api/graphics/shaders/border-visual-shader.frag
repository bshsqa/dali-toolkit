#version 430

layout( set = 0, binding = 1, std140 ) uniform FragData
{
  lowp vec4 uColor;
  lowp vec4 borderColor;
  lowp vec3 mixColor;
};

layout( location = 0 ) out vec4 fragColor;

void main()
{
  fragColor = vec4(mixColor, 1.0)*borderColor*uColor;
}
