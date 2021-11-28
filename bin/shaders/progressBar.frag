//Fragment Shader source code
#version 330 core
out vec4 FragColor;


//uniform vec2 u_resolution;

uniform vec3 currentColor;




void main(){

    //vec2 pos3 = gl_FragCoord.xy/u_resolution;
    vec2 position = gl_FragCoord.xy;

    vec3 color =currentColor;

    
    
   
	//gl_FragColor = vec4(color,1);
	//gl_FragColor = vec4(1);
	FragColor = vec4(color,1);
}