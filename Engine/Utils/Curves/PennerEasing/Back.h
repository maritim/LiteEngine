#ifndef _PENNER_BACK
#define _PENNER_BACK

namespace PennerEasing
{

class Back {
	
	public:
		
		static float easeIn(float t,float b , float c, float d);
		static float easeOut(float t,float b , float c, float d);
		static float easeInOut(float t,float b , float c, float d);
		
};

}

#endif