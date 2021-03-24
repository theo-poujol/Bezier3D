attribute highp vec4 posAttr;
attribute lowp  vec4 colAttr;
attribute highp vec3 norAttr;
varying lowp  vec4 col;
varying highp vec3 nor;
uniform highp mat4 matrix;
uniform highp mat3 norMatrix;

void main() {
   col = colAttr;
   gl_Position = matrix * posAttr;
   nor = norMatrix * norAttr;
}
