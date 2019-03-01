#version 430

// Deferred Buffer: Must contain the fragment position in the world coordinates system
layout( binding = 0) uniform sampler2DArray deferredBuffer;

// Texture coordinate to read in the deferred buffer
in vec3 texCoord;

// fragment final color
layout (location = 0) out vec4 color;

// a TOP tree node
struct node {
	vec4 plane;
	uint link[4]; /* 0: positive child, 1: intersection child, 2: negative child (not used), 3: wedge angle */
};

// TOP tree buffer.
layout (std430, binding=13) restrict buffer TOPTree	{
	node nodes[];
};

// Buffer to read the root index
layout (std430, binding=29) restrict buffer TOPTreeRoot	{
	uint root;
};


/* return 0 if p is in shadow, 1 otherwise
	p is the fragment in the world coordinates system with the light as origin
	normal is the surface normal at p
*/
float TOPTREE_query( in vec3 p, in vec3 normal ){
	// 32 uint stack to push intersection node. 32 is the value used in the EG2015 paper for all tests on hard shadows
    uint stack[32];
	uint stacksize = 0;
	const float sqdist = dot(p,p); // squared distance from p to the origin (the light)

	stack[ stacksize++ ] = root; // push the root

	// if we are back facing the light, querying the TOP tree is not necessary
	if ( dot( normal, -p) < 0.0 ) return 0.0;

 	// find the location of p...
	while(stacksize>0){
		// pop
		const uint current = stack[ --stacksize ];
		const node n = nodes[ current ];

		// compute the signed distance from p to the current plane
		const float offset = dot(n.plane, vec4(p,1));

		// if an intersection child exists and if current is a capping plane or if it is a shadow plane and p is inside its wedge
		if ( n.link[1]>0 && (current%4==3 || offset*offset / sqdist < uintBitsToFloat(n.link[3])) )
		 		stack[ stacksize++ ] = n.link[1];

		// if p is in the positive halfspace of the plane
		if ( offset>0.0 ){
			// the location of p continues in the positive child if it exists
			if ( n.link[0]>0 ) stack[ stacksize++ ] = n.link[0];
		}
		// otherwise p is in the negative halfspace
		else {
			// if the negative child is a leaf, p is inside a SV and thus it is not visible from the light
			if ( current%4==3 ) return 0.0;
			// otherwise the localisation of p continues in the negative child
			else stack[ stacksize++ ] = current+1;
		}
	}
	return 1.0;
}

// must return the fragment position in the world space coordinates system
vec4 getFragmentPosition(){
	// coordinates should not be interpolated !
	return textureLod(deferredBuffer, /* TO COMPLETE */ );
}

// must return the fragment normal in the world coordinates system
vec3 getFragmentNormal(){
	return normalize( textureLod( deferredBuffer, /* TO COMPLETE */ ).xyz);
}

// return true if a fragment exists, otherwise false (no projected geometry)
bool fragmentExist( in vec4 frag ){
	// TO COMPLETE
}

// must return the light position in the world space coordinates system
vec3 getLight(){
	// TO COMPLETE
}

void main()
{
	vec4 pos = getFragmentPosition();
	if( fragmentExist(pos)==true )
	{
		vec3 normal = getFragmentNormal();
		vec3 light = getLight();
		float visibility = TOPTREE_query(pos.xyz-light, normal);

		/* You may want compute the final color using visiblity (0/1), light, normal and pos ! Or whatever you want.
		color = TO COMPLETE
		*/
	}
	else
		color=vec4(1);
}
