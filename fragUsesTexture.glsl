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

/*beh�ver den nya texturkoordinaten f�r p f�r att komma �t den nya positionen p� sanden

Om jag ska l�gga in p i en ny shader f�r att f� dess texturkoordinat i interpoleringen till
fragmentshadern beh�ver jag veta vilken texturkoordinat vi �r i nu. Det kan sparas i en textur
men d� beh�vs koordinaterna f�r att f� ut koordinaterna s� det �r om�jligt.

D� m�ste jag sj�lv projicera punkten till light-space. D� beh�ver jag en matris som konverterar 
en punkt i world-space till light-space och kan lika g�rna g�ra det i vertex shadern.
Ska jag r�kna ut gridpunkternas texturkoordinater med samma formel? 
F�rdel: Konsekvent 
Fact: Texturen b�rjar i h�rnen av gridet
Nackdel: M�ste r�knas om? Inte om bara en rendering sker p� gridet
	M�ste skriva mer kod*/
