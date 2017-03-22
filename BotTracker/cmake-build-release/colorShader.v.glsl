
attribute vec4 vertex;
uniform vec2 offset;
void main(void) 
{
    vec4 pos=vertex;
    pos.xy+=offset;
	gl_Position = pos;
}
