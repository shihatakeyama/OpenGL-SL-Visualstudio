/*
 * Original shader from: https://www.shadertoy.com/view/7sB3Dy
 */

#ifdef GL_ES
precision mediump float;
#endif

// glslsandbox uniforms
uniform float time;
uniform vec2 resolution;

// shadertoy emulation
#define iTime time
#define iResolution vec3(resolution,1.)

// --------[ Original ShaderToy begins here ]---------- //
#define R(p,a,r)mix(a*dot(p,a),p,cos(r))+sin(r)*cross(p,a)
#define H(h)(cos((h)*6.3+vec3(0,23,21))*.5+.5)
void mainImage(out vec4 O, vec2 C)
{
    O=vec4(0);
    vec3 p,r=iResolution,
    d=normalize(vec3((C-.5*r.xy)/r.y,1));  
    float s,e,g=0.;
//O=vec4(C,r.y,1.0);
    for(float i=1.;i<70.;++i)
    {
        p=g*d-vec3(0,0,1.5);
        p=R(p.zxy,normalize(vec3(1,0,10)),iTime*.3);
        s=2.;
        for(int i=0;i<6;++i)
            p=abs(p-vec3(1,2.8,1.5+sin(iTime*.5)*.2))-vec3(1,3.+sin(iTime*.7)*.3,2.1),
            p*=(fract(iTime*.1)>.5)?-1.:1.,
            s*=e=7./clamp(dot(p,p),1.2,7.),
            p*=e;
        g+=e=min(abs(p.z),length(p.xy)-.05)/s+1e-3;
        O.xyz+=mix(vec3(1),H(log(s)*.3),.6)*.03*exp(-.2*i*i*e);
    }
}
// --------[ Original ShaderToy ends here ]---------- //

void main(void)
{
#if 1
    mainImage(gl_FragColor, gl_FragCoord.xy);
    gl_FragColor.a = 1.;
#else
//	gl_FragColor = vec4(mod(time,1.0),mod(float(gl_FragCoord.x)/100.0,1.0),mod(float(gl_FragCoord.y)/100.0,1.0),1.0);
	gl_FragColor	= vec4(gl_FragCoord.xy/200.0,1.0,1.0);
#endif
}