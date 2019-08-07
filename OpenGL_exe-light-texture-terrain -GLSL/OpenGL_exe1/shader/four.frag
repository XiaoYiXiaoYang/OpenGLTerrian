
varying vec3 norm;
varying vec3 eyeCoords;
varying vec3 s;
varying vec3 v;
varying vec3 r;
uniform sampler2D baseMap;
void main()
{
	vec4  fvBaseColor      = texture2D(baseMap, gl_TexCoord[0].st);

    float sDotN = max(0.0, dot(s, norm));

	vec3 ambient = vec3(gl_FrontMaterial.ambient * gl_LightSource[0].ambient);
 	vec3 diffuse = vec3(vec3(gl_FrontMaterial.diffuse) * vec3(gl_LightSource[0].diffuse) * sDotN );
    vec3 specular = vec3(0.0);
    if(sDotN > 0.0)
    {
        specular = vec3 (gl_LightSource[0].specular * gl_FrontMaterial.specular *
                pow(max(0.0,dot(r, v)), gl_FrontMaterial.shininess));
    }

    vec3 LightIntensity=(ambient+diffuse +specular)*vec3(fvBaseColor);
    gl_FragColor = vec4(LightIntensity,1.0);
}
