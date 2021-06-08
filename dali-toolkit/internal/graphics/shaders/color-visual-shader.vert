#ifndef IS_REQUIRED_ROUNDED_CORNER
#define IS_REQUIRED_ROUNDED_CORNER 0
#endif
#ifndef IS_REQUIRED_BORDERLINE
#define IS_REQUIRED_BORDERLINE 0
#endif
#ifndef IS_REQUIRED_BLUR
#define IS_REQUIRED_BLUR 0
#endif

INPUT mediump vec2 aPosition;
#if IS_REQUIRED_ROUNDED_CORNER || IS_REQUIRED_BORDERLINE || IS_REQUIRED_BLUR
OUTPUT mediump vec2 vPosition;
OUTPUT mediump vec2 vRectSize;
OUTPUT mediump vec2 vOptRectSize;
#if IS_REQUIRED_ROUNDED_CORNER
OUTPUT mediump vec4 vCornerRadius;
#endif
#endif

uniform highp mat4 uMvpMatrix;
uniform highp vec3 uSize;

//Visual size and offset
uniform mediump vec2 offset;
uniform highp vec2 size;
uniform mediump vec4 offsetSizeMode;
uniform mediump vec2 origin;
uniform mediump vec2 anchorPoint;
#if !IS_REQUIRED_BLUR && IS_REQUIRED_BORDERLINE
uniform mediump float borderlineWidth;
uniform mediump float borderlineOffset;
#endif
#if IS_REQUIRED_BLUR
uniform mediump float blurRadius;
#endif
#if IS_REQUIRED_ROUNDED_CORNER
uniform mediump vec4 cornerRadius;
uniform mediump float cornerRadiusPolicy;
#endif
uniform mediump vec2 extraSize;

vec4 ComputeVertexPosition()
{
  vec2 visualSize = mix(uSize.xy*size, size, offsetSizeMode.zw ) + extraSize;
  vec2 visualOffset = mix(offset, offset/uSize.xy, offsetSizeMode.xy);

#if IS_REQUIRED_ROUNDED_CORNER || IS_REQUIRED_BORDERLINE || IS_REQUIRED_BLUR
  vRectSize = visualSize * 0.5;
  vOptRectSize = vRectSize;
#endif

#if IS_REQUIRED_ROUNDED_CORNER
#if !IS_REQUIRED_BLUR && IS_REQUIRED_BORDERLINE
  mediump float minSize = min(visualSize.x, visualSize.y) + (1.0 + clamp(borderlineOffset, -1.0, 1.0)) * borderlineWidth;
#else
  mediump float minSize = min(visualSize.x, visualSize.y);
#endif
  vCornerRadius = mix(cornerRadius * minSize, cornerRadius, cornerRadiusPolicy);
  vCornerRadius = min(vCornerRadius, minSize * 0.5);
  // Optimize fragment shader. 0.2929 ~= 1.0 - sqrt(0.5)
  mediump float maxRadius = max(max(vCornerRadius.x, vCornerRadius.y), max(vCornerRadius.z, vCornerRadius.w));
  vOptRectSize -= 0.2929 * maxRadius + 1.0;
#endif

#if IS_REQUIRED_BLUR
  vPosition = aPosition * (visualSize + 2.0 * blurRadius);
  vOptRectSize -= blurRadius + 1.0;
#elif IS_REQUIRED_BORDERLINE
  vPosition = aPosition * (visualSize + (1.0 + clamp(borderlineOffset, -1.0, 1.0))* borderlineWidth);
  vOptRectSize -= (1.0 - clamp(borderlineOffset, -1.0, 1.0)) * 0.5 * borderlineWidth + 1.0;
#elif IS_REQUIRED_ROUNDED_CORNER
  vPosition = aPosition * visualSize;
#else
  mediump vec2 vPosition = aPosition * visualSize;
#endif
  return vec4(vPosition + anchorPoint * visualSize + (visualOffset + origin) * uSize.xy, 0.0, 1.0);
}

void main()
{
  gl_Position = uMvpMatrix * ComputeVertexPosition();
}
