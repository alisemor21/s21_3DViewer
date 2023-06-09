attribute vec4 tex;
attribute vec4 position;
uniform mat4 matrix;
varying vec4 color;

void main(void)
{
    gl_Position = matrix * position;
    color = tex;
}
