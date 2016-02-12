# version 330

layout(location = 0) out vec4 color;
layout(location = 4) out vec3 refractedRay;

in vec2 fUV;
in vec4 ftex;
//extra
in vec4 fEstimatedPoint;
in vec3 fRefRay;

//uniform sampler2D renderedTexture;//grid points equivalent on water
//extra
uniform sampler2D sand;

void main(){
	color = vec4(1,0,0,0);//ftex;//texture( renderedTexture, fUV );

	//Find where on the sand this photon should land
	//color = texture( sand, fUV );
	//color = fEstimatedPoint;
	//refractedRay = fRefRay;
}