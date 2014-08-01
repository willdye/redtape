// This is just a formatting experiment, not necessarily code for public use.
const int   NUM_BUCKETS       = 32;
const int   ITER_PER_BUCKET   = 1024;
const float HISTO_SCALE       = 8.;
const float NUM_BUCKETS_F     = float(NUM_BUCKETS);
const float ITER_PER_BUCKET_F = float(ITER_PER_BUCKET);

float nrand( vec2 n ) { //note: uniformly distributed, normalized rand, [0;1[
  return fract( sin( dot( n.xy, vec2( 12.9898, 78.233 ))) * 43758.5453 ); }
float remap( float a, float b, float v ) {//remap v to [0;1] in interval [a;b]
  return clamp( (v-a) / (b-a), .0, 1. ); }
float trunc( float a, float l ) { //note: quantizes in l levels
  return floor(a*l)/l; }
float n1rand( vec2 n ) {
  float nrnd0 = nrand( n + .70 * fract( iGlobalTime ) );
  return nrnd0; }
float n2rand( vec2 n ) {
  float nrnd0 = nrand( n + .70 * fract( iGlobalTime ) );
  float nrnd1 = nrand( n + .11 * fract( iGlobalTime + .573953 ) );
  return ( nrnd0 + nrnd1 ) / 2.; }
float n3rand( vec2 n ) {
  float nrnd0 = nrand( n + .70 * fract( iGlobalTime ) );
  float nrnd1 = nrand( n + .11 * fract( iGlobalTime + .573953 ) );
  float nrnd2 = nrand( n + .13 * fract( iGlobalTime + .158604) );
  return ( nrnd0 + nrnd1 + nrnd2 ) / 3.; }
float n4rand( vec2 n ) {
  float nrnd0 = nrand( n + .70 * fract( iGlobalTime ) );
  float nrnd1 = nrand( n + .11 * fract( iGlobalTime + .573953 ) );
  float nrnd2 = nrand( n + .13 * fract( iGlobalTime + .158604) );
  float nrnd3 = nrand( n + .17 * fract( iGlobalTime + .958469) );
  return ( nrnd0 + nrnd1 + nrnd2 + nrnd3 ) / 4.0; }
float histo( int iter, vec2 uv, vec2 interval ) {
  float t = remap( interval.x, interval.y, uv.x );
  float bucketval = .0;
  vec2  bucket = vec2( trunc( t, NUM_BUCKETS_F ),
		       trunc( t, NUM_BUCKETS_F ) + 1./NUM_BUCKETS_F );
  for( int i=0; i < ITER_PER_BUCKET; ++i ) {
    float r, seed = float(i)/ITER_PER_BUCKET_F;
    if     ( iter < 2 ) r = n1rand( vec2( uv.x, .5 ) + seed );
    else if( iter < 3 ) r = n2rand( vec2( uv.x, .5 ) + seed );
    else if( iter < 4 ) r = n3rand( vec2( uv.x, .5 ) + seed );
    else                r = n4rand( vec2( uv.x, .5 ) + seed );
    bucketval += step( bucket.x, r ) * step( r, bucket.y ); }
  bucketval /= ITER_PER_BUCKET_F;
  bucketval *= HISTO_SCALE;
  return 0.85 - 0.7 * step( bucketval, uv.y / 0.25 ); }
void main(void) {
  float o;
  vec2 uv = gl_FragCoord.xy / iResolution.xy;
  if( ( abs( uv.x - .25 ) < .002 ) ||
      ( abs( uv.x - .50 ) < .002 ) ||
      ( abs( uv.x - .75 ) < .002 ) ||
      ( abs( uv.y - .25 ) < .002 ) ) {
    o = 0.; }
  else if(   uv.y < .25 ) {
    if(      uv.x < 0.25 ) { o = histo( 1, uv, vec2( 0.00, 0.25 ) ); }
    else if( uv.x < 0.50 ) { o = histo( 2, uv, vec2( 0.25, 0.50 ) ); }
    else if( uv.x < 0.75 ) { o = histo( 3, uv, vec2( 0.50, 0.75 ) ); }
    else                   { o = histo( 4, uv, vec2( 0.75, 1.00 ) ); } }
  else {
    if(      uv.x < 0.25 ) { o = n1rand( uv ); }
    else if( uv.x < 0.50 ) { o = n2rand( uv ); }
    else if( uv.x < 0.75 ) { o = n3rand( uv ); }
    else                   { o = n4rand( uv ); } }
  //display lines
  gl_FragColor = vec4( vec3(o), 1. ); }
