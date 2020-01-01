precision mediump float;
uniform sampler2D texture;
varying vec2 texcoordOut;

void main()
{
    gl_FragColor = texture2D(texture, texcoordOut);
}
