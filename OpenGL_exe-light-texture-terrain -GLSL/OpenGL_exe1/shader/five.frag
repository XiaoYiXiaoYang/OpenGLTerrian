
varying vec3 norm;
varying vec3 objectPosition;
varying vec3 v;
varying vec3 l;

uniform sampler2D baseMap;
void main()
{
	vec4  fvBaseColor = texture2D(baseMap, gl_TexCoord[0].st);

    vec3 Normal=normalize(norm);
    vec3 LightDir = normalize(l);
    float NDotL = max(dot(Normal,LightDir),0.0);
    vec3 ViewDir =normalize(v);
    vec3 reflectDir = reflect(-LightDir,Normal);

    vec3 ambient = vec3(gl_FrontMaterial.ambient * gl_LightSource[0].ambient);                         //Ka*La   
 	vec3 diffuse = vec3(vec3(gl_FrontMaterial.diffuse) * vec3(gl_LightSource[0].diffuse) * NDotL );   //Kd *Ld *max（（n.l），0）  

    vec3 specular = vec3(0.0);                                                                        //Ks *Ls *max（cos $,0） @次幂
    if(NDotL > 0.0)
    {
        specular = vec3 (gl_LightSource[0].specular * gl_FrontMaterial.specular *
                pow(max(dot(ViewDir,reflectDir),0.0), gl_FrontMaterial.shininess));
    }

    vec3 LightIntensity=(ambient+diffuse +specular);
    gl_FragColor = vec4(LightIntensity,1.0)*fvBaseColor;
}
