#ifndef GVAR_HPP
#define GVAR_HPP

struct GVar {
	enum class VarType {
		BYTE,
		UBYTE,
		SHORT,
		USHORT,
		INT,
		UINT,
		FLOAT,
	};

	VarType typ;

	union {
		char byte;
		unsigned char ubyte;

		short srt;
		unsigned short usrt;

		int i;
		unsigned int ui;

		float flt;
	};
};

#endif