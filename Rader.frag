precision mediump float;
uniform vec2 resolution;
uniform float time;
uniform vec2 mouse;

#define PI     3.1415926535897932384626433832795
#define TWO_PI 2.*3.1415926535897932384626433832795

float plot(vec2 st, float pct, float w){
 return  smoothstep( pct-w/2., pct, st.y) -
         smoothstep( pct, pct+w/2., st.y);
}


/* Circle using Distance Field */
vec3 circle(in vec2 _st, in vec2 _o1, in float _r) {
 vec2  d = _st - _o1;
 float w = _r;
 return vec3(1. - smoothstep( _r- w , _r+ w, dot(d,d)*(5.)) ) ;
}

vec2 mesh(in vec2 _uv, in float k, in float asp) {
   vec2 st = _uv;
   st.y = fract(_uv.y*k) ;
   st.x = fract(_uv.x*k - step(1., mod(_uv.y*k , 2.))*0.5 )  ;
   st -=0.5 ;
   return st;
}

/*
 背景部のグリッド。小数部が一定値以下なら線を引く。
 _p : 返還後の座標。 n 倍になっている事を期待
 _w : グリッド線の幅。uv座標系での太さを設定する。
*/
float grid(in vec2 _p , in int n, in float asp, in float _w) {
   vec2 p_frac = fract(_p*float(n));
   float w = _w * float(n);
   float col = step( p_frac.x , w ) + step(p_frac.y, w)  ;
   return col/col;
}

//乱数
float rand_1(float k) {
   return fract(cos(k*42829.6431137957832384 + 5254.74236 )*100.)  ;
}

//乱数
float rand_2(float k) {
   return fract(k*6431129.82937457832384 + 54.7365242 )  ;
}

//乱数;時間変化する
float rand_t(float _k) {
   float t = rand_1( floor(time*10.)*0.0005)*1.5;
   float k = _k + 0.5;
   float u = (k*3651.594949538594162674 + 2353.32305960796 + t )/1.41421356 ;
   return fract(sin(u)*100.);
}

//乱数;時間変化する
float rand_t_o(in float _k, in float _o) {
   float t = rand_1( floor(time*10.)*0.0005)*1.5;
   float k = _k + 0.5 + _o;
   float u = (k*3651.594949538594162674 + 2353.32305960796 + t )/1.41421356 ;
   return fract(sin(u)*100.);
}

//ノイズ
float noise_t( float _x , int _m) {
 // remap
 float x = _x -0.5;
 // calc
 float n = float(_m);

 float i = fract(x*n) ;
 float f = floor(x*n) ;
 float nz = mix( rand_t(f) , rand_t(f + 1.), smoothstep(0., 1., i ) );
 //float nz = rand_t(f) ;
 return nz;
}

float noise_t_o( float _x , in float _o, int _m) {
 // remap
 float x = _x -0.5 - _o;
 // calc
 float n = float(_m);

 float i = fract(x*n) ;
 float f = floor(x*n) ;
 float nz = mix( rand_t(f) , rand_t(f + 1.), smoothstep(0., 1., i ) );
 return nz;
}

// 背景を緑に塗る関数。
vec3 bgcolor(in vec2 uv, in float asp, in int n) {
 float w = 0.0025;
 float f_grid = grid(uv, n/4, asp, w);
 vec3 col = vec3(
   f_grid*1.00 ,
   f_grid*1.00 ,
   f_grid*0.00
   )*0.33;

 col = vec3 (
   max(col.r, 0.000),
   max(col.g, 0.166),
   max(col.b, 0.000)
   ) ;
 return col;
}


void main () {
 vec2 uv = gl_FragCoord.xy / resolution.xy ;
 uv -= .5;
 float asp = resolution.x/resolution.y;
 uv.x *= asp;


 int n = 24;
 float sz = 0.35;

 //vec3 col =  vec3(0.);
 vec3 col =  bgcolor(uv, asp, n) ;

 vec3 x_o = vec3 (
             0.175 * sin(time*1.29),
             0.175 * sin(time*0.62),
             0.175 * sin(time*0.84)
             )/float(n);

 float r = noise_t_o(uv.x, x_o.r, n)*sz;
 float g = noise_t_o(uv.x, x_o.g, n)*sz;
 float b = noise_t_o(uv.x, x_o.b, n)*sz;

 //col = vec3(plot(vec2(uv.x, uv.y),y,0.01));
 col += vec3(
         plot(vec2(uv.x, uv.y+sz/2.),r, 0.0100*sin(time)+0.05 ) ,
         plot(vec2(uv.x, uv.y+sz/2.),g, 0.0115*sin(time)+0.05 ) ,
         plot(vec2(uv.x, uv.y+sz/2.),b, 0.0125*sin(time)+0.05 )
         );


 gl_FragColor = vec4(col, 1.) ;
}