//Fragment Shader source code
#version 330 core
out vec4 FragColor;

in vec3 fragPos;
in vec3 normalSurf;
uniform vec3 lightColor;
uniform vec3 lightPosition;
//uniform vec2 iResolution;

float getColor(float fx,float fy)
{
	int x;
	int y;
	if(fx*fy<0)
	{
		x = int(fx*2)+1;
		y = int(fy*2);
	}
	else
	{
		x = int(fx*2);
		y  = int(fy*2);
	}


	if(x%2==0)
		{
			if(y%2 != 0)
			{
				return 0;
			}
			else
			{
				return 1;
			}
		}
	else
		{
			if(y%2 != 0)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}


	}

	float N21(vec2 p)
	{
	    return fract(sin(p.x*100. + p.y*6547.)*5647.);
	}

	float smoothNoise(vec2 uv)
	{
	    vec2 lv = fract(uv*10.);
	    lv = lv*lv*(3.-2.*lv);
	    vec2 id = floor(uv*10.);
	    float bl = N21(id);
	    float br = N21(id+vec2(1,0));
	    float b = mix(bl,br,lv.x);

	    float tl = N21(id+vec2(0,1));
	    float tr = N21(id+vec2(1,1));
	    float t = mix(tl,tr,lv.x);

	    return mix(b,t,lv.y);
	}

	float smoothNoise2(vec2 uv)
	{
	    float c = smoothNoise(uv*.5);
	   c += smoothNoise(uv)*.5;
	   c += smoothNoise(uv*2.)*.25;
	   c += smoothNoise(uv*4.)*.125;
	   c += smoothNoise(uv*8.)*.0625;
	   return c/2.;
	}


void main()
{
	vec2 uv  = fragPos.xy;// /iResolution;
	// uv.x/=iResolution.x;
	// uv.y/=iResolution.y;
	uv.x/=25.0f;
	uv.y /=18.75f;



	vec3 N = normalize(normalSurf);
	vec3 L = normalize(lightPosition - fragPos);
	float diffuse = max(0,dot(N,L));

	vec3 color;

	//color = vec3(getColor(fragPos.x,fragPos.y));
	float c = smoothNoise2(uv);

	// if(c>.6)
  //    {
  //        c = 0.;
  //    }
  //    if(c>0.575)
  //    {
  //        c=1.;
  //    }
  //    else
  //    {
  //        c = 0.;
  //    }

	if(c>0.575f)
	{
		c=smoothstep(0.6f,0.595f,c);

	}
	else
	{
		c=smoothstep(0.57f,0.575f,c);

	}

	//c = smoothstep(0.5f,0.5f,c);
	color = vec3(c);

	if(c<0.9f)
	{
		color.z = 1;
		color.y = .5;
	}









	vec4 finalColor = vec4(color,1);

	//FragColor = vec4(color,1);
	FragColor = finalColor*vec4(diffuse * lightColor,1);
};
