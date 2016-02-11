# version 330

layout(location = 0) out vec4 color;

in vec2 fUV;
in vec4 ftex;

//uniform sampler2D renderedTexture;//grid points equivalent on water

void main(){
	color = vec4(1,0,0,0);//ftex;//ture( renderedTexture, fUV );

	//color = vec3(1,1,1);
	//float i = texture( shadowMap, vec3(ShadowCoord.xy, (ShadowCoord.z)/ShadowCoord.w) );
	//color = vec3(i,i,i);


	//if(texture( shadowmap, UV.xy).z < UV.z){
		//color = vec3(0,0,0);
	//}
	//color = texture( renderedTexture, UV + 0.005*vec2( sin(time+1024.0*UV.x),cos(time+768.0*UV.y)) ).xyz ;
	//color = vec4(texture( depthTex, UV + 0.005*vec2( sin(time+1024.0*UV.x),cos(time+768.0*UV.y)) ).x, 1.0) ;
	//byt till värdet i ljusmappen
}