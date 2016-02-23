#ifndef _PENNER_CIRC
#define _PENNER_CIRC

#include <cmath>

namespace PennerEasing
{

class Circ {
	
	public:
		
		static float easeIn(float t,float b , float c, float d);
		static float easeOut(float t,float b , float c, float d);
		static float easeInOut(float t,float b , float c, float d);
};

}

#endif