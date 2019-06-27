#version 330 core
in vec2 TexCoord;

uniform sampler1D tex;
uniform int iter;
uniform vec2 c;
uniform vec2 pan;
uniform float zoom;
uniform float ratio;
uniform float rel_size;

float abs2(vec2 z)
{
    return sqrt(z.x*z.x + z.y*z.y);
}

vec3 rgb2hsv(vec3 c)
{
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}

vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main() {
    vec2 z;
    vec2 point = TexCoord / pow(zoom, exp(1));
    point.x /= (rel_size);

    z.x = (point.x + pan.x);
    z.y = (point.y + pan.y);

    int i;
    for(i=0; i<iter; i++) {
        float x = (z.x * z.x - z.y * z.y) + c.x;
        float y = (z.y * z.x + z.x * z.y) + c.y;

        if((x * x + y * y) > 4.0) break;
        z.x = x;
        z.y = y;
    }

    //magic logarithm color so we can index the palette with iterations before loop exit
    float palIndex = ( i - log(z.x*z.x + z.y*z.y) / log(4)) / i;
    //get palette with index
    vec3 col = texture(tex, (i >= iter) ? 0 : palIndex).xyz;
    //col = hsv2rgb(vec3(rgb2hsv(col).x, 0.5, 1));
    //get a fancy hsv color with the index
    //vec3 colHSV = hsv2rgb(vec3(palIndex, 1, 0.9));

/*     float cross = 0.01f/zoom;
    if ((abs(point.x) < cross) || (abs(point.y) < cross))
    {
        col = vec3(1);
    }
    if ((abs(TexCoord.x) < cross) || (abs(TexCoord.y) < cross))
    {
        col = vec3(1, 0, 0);
    } */
    vec3 colHSV = rgb2hsv(col);
    col = hsv2rgb(vec3(colHSV.xy, pow(colHSV.z, 4)));

    //output the product of both colors to maximize coolness
    gl_FragColor = vec4(col, 1);

}
			
