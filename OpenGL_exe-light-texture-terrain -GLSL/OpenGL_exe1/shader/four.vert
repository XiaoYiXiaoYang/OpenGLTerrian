
varying vec3 norm;
varying vec3 eyeCoords;
varying vec3 s;
varying vec3 v;
varying vec3 r;

void main()
{
    gl_TexCoord[0] = gl_MultiTexCoord0;
    
     norm = (gl_NormalMatrix * gl_Normal);
     eyeCoords = vec3(gl_ModelViewMatrix * gl_Vertex);

     s = normalize(vec3(gl_LightSource[0].position) - eyeCoords);
     v = normalize(-eyeCoords.xyz);
     r = reflect(-s, norm);

    gl_Position = ftransform();
}
