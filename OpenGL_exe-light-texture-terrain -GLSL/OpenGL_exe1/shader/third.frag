
varying vec3 LightIntensity;

uniform sampler2D baseMap;
void main()
{
	vec4  fvBaseColor      = texture2D(baseMap, gl_TexCoord[0].st);

    gl_FragColor = vec4(LightIntensity,1.0)*fvBaseColor;

}
