#ifndef I_CANIMATION_HPP
#define I_CANIMATION_HPP

namespace Engine {
	namespace Interfaces {

		class ICAnimation {
		public:

			virtual void updateAnimation(float dt) = 0;
			virtual void* getCurrentAnimation(unsigned int &numJoints) = 0;

		};
	}
}


#endif