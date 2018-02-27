#ifndef SHADER_NODES_HPP
#define SHADER_NODES_HPP

enum class IdType {
	FLOAT,
	VEC2,
	VEC3,
	VEC4,

};

enum class MathType {
	MUL,
	DIV,
	ADD,
	SUB,
	MIN,
	MAX
};

enum class AttribInputTypes {
	POSITION,
	NORMAL,
	COLOR,
	B_INDEX,
	B_WEIGHT,
};

enum class InputTypes {
	INVALID,
	ATTRIB,
	CONST,
	UNIFORM,
};

class Identifier {
public:
	IdType type;
	unsigned int identifier;
};

class IShaderNode {
public:
	virtual ~IShaderNode() {};
};

class IdConverter {
public:
	Identifier convId;
	Identifier input;
};

class Math : public IShaderNode {

public:
	virtual ~Math() {};

	MathType mathMode;

	Identifier nodeId;

	Identifier input1;
	Identifier input2;

};

class Input : public IShaderNode {
public:
	virtual ~Input() {};

	virtual InputTypes getType() { return InputTypes::INVALID; }

	Identifier nodeId;
};

class AttribInput : public Input {

	virtual ~AttribInput() {};

	virtual InputTypes getType() { return InputTypes::ATTRIB; }

	AttribInputTypes inType;
};

class ConstInput : public Input {

	virtual ~ConstInput() {};

	virtual InputTypes getType() { return InputTypes::CONST; }
};

class UniformInput : public Input {

	virtual ~UniformInput() {};

	virtual InputTypes getType() { return InputTypes::UNIFORM; }
	int uniformId;

};

class OutPut : public IShaderNode {
public:
	virtual ~OutPut() {};

	int outPutTarget;

	Identifier input;
};


#endif