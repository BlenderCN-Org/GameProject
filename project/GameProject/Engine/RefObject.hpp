#ifndef REF_OBJECT_HPP
#define REF_OBJECT_HPP

namespace Engine {

	class IRefObject {

	public:
		virtual ~IRefObject() {};

		virtual void refUpdate(float dt) = 0;

	};

	template<typename T>
	class RefObject : public IRefObject {

	public:

		RefObject(float ttl) : obj(nullptr), aliveTime(ttl) {}
		~RefObject() {
			if (obj) {
				delete obj;
				obj = nullptr;
			}
		}

		virtual void refUpdate(float dt) {
			if (ttl < 0) {
				delete obj;
				obj = nullptr;
				ttl = -1;
			} else {
				ttl -= dt;
			}
		}

		T* operator->() {
			ttl = aliveTime;
			if (obj == nullptr) {
				obj = new T();
			}
			return obj;
		}

	private:

		float aliveTime;
		float ttl;

		T* obj;
	};

}

#endif