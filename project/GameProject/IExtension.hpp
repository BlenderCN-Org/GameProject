#ifndef IEXTENSION_HPP
#define IEXTENSION_HPP

class IExtension {
public:
	virtual void execute(int argv, void** argc) = 0;
};

#endif