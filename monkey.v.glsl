attribute vec4 v_coord;
// attribute vec3 v_normal;
uniform mat4 mvp;
varying vec4 color;

void main(void) 
{
  gl_Position = mvp * v_coord;
  color = vec4(v_coord[1], v_coord[1], v_coord[0], 1.0);
}
