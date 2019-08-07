
varying vec3 LightIntensity;

void main()
{
	gl_TexCoord[0] = gl_MultiTexCoord0;

    vec3 norm = normalize(gl_NormalMatrix * gl_Normal);
    vec3 eyeCoords = vec3(gl_ModelViewMatrix * gl_Vertex);

    vec3 s = normalize(vec3(gl_LightSource[0].position) - eyeCoords);
    vec3 v = normalize(-eyeCoords.xyz);
    vec3 r = reflect(-s, norm);
    float sDotN = max(0.0, dot(s, norm));
    
    vec3 ambient = vec3(gl_FrontMaterial.ambient * gl_LightSource[0].ambient);
 	vec3 diffuse = vec3(vec3(gl_FrontMaterial.diffuse) * vec3(gl_LightSource[0].diffuse) * sDotN);

    vec3 specular = vec3(0.0);
    if(sDotN > 0.0)
    {
        specular = vec3 (gl_LightSource[0].specular * gl_FrontMaterial.specular *
                pow(max(0.0,dot(r, v)), gl_FrontMaterial.shininess));
    }

    LightIntensity = ambient + diffuse + specular;
    gl_Position = ftransform();
}
