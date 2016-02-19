# version 330

layout(location = 0) out vec4 color;
layout(location = 4) out vec3 refractedRay;

in vec2 fUV;
in vec4 ftex;
//extra
in vec4 fEstimatedPoint;
in float fsize;

//uniform sampler2D renderedTexture;//grid points equivalent on water
//extra
uniform sampler2D sand;

void main(){
	color = vec4(1,0,0,0.3);//ftex;//texture( renderedTexture, fUV );

	//Find where on the sand this photon should land
	//color = texture( sand, fUV );
	//color = fEstimatedPoint;
	//refractedRay = fRefRay;


	//use the distance from the old point to the intersection point to find the new d
	//float d = length(fEstimatedPoint - intersectionPointOnWater);

	//vec3 p = intersectionPointOnWater + d * fRefRay;

	//project p into light-space and find its texture coordinates for depth if it would have been on the geometry
	
}

/*behöver den nya texturkoordinaten för p för att komma åt den nya positionen på sanden

Om jag ska lägga in p i en ny shader för att få dess texturkoordinat i interpoleringen till
fragmentshadern behöver jag veta vilken texturkoordinat vi är i nu. Det kan sparas i en textur
men då behövs koordinaterna för att få ut koordinaterna så det är omöjligt.

Då måste jag själv projicera punkten till light-space. Då behöver jag en matris som konverterar 
en punkt i world-space till light-space och kan lika gärna göra det i vertex shadern.
Ska jag räkna ut gridpunkternas texturkoordinater med samma formel? 
Fördel: Konsekvent 
Fact: Texturen börjar i hörnen av gridet
Nackdel: Måste räknas om? Inte om bara en rendering sker på gridet
	Måste skriva mer kod*/
