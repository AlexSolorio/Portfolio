// Vertex shader

attribute vec3 vPosition;  // position of vertex (x, y, z)
attribute vec3 vColor;     // color of vertex (r, g, b)

varying vec4 fColor;       // output color to send to fragment shader
uniform float alphaValue;  // alpha value for vertex

void main() {
  gl_Position = vec4(vPosition, 1); // set vertex position (x, y, z, w)
  gl_PointSize = 10.0;              // set size of points drawn
  fColor = vec4(vColor, alphaValue);       // output color to fragment shader
}
