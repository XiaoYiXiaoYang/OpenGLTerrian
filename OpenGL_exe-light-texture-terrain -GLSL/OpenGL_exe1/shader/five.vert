
uniform vec3 eyePosition;

varying vec3 norm;
varying vec3 objectPosition;
varying vec3 v;
varying vec3 l;

//光线方向和视线方向似乎算错了，
void main()
{
     gl_TexCoord[0] = gl_MultiTexCoord0;
    
     norm = gl_NormalMatrix * gl_Normal;			//法线向量
    
     objectPosition = vec3(gl_ModelViewMatrix * gl_Vertex);   //求取顶点变换到相机空间的位置,在世界空间中的顶点位置

     v=eyePosition-objectPosition;								//计算视线方向向量
     l=vec3(gl_LightSource[0].position) - objectPosition;		//沿着法线轴的反射向量

    gl_Position = ftransform();
}
