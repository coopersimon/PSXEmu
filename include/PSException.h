#ifndef PS_EXCEPTION_H
#define PS_EXCEPTION_H

#include <stdexcept>
//#include <exception>

/*
 *	PSException.h
 *
 *	Exceptions for use in the CPU
 *
 */

class psException
{
public:
	//virtual void Throw() = 0;

	// get the code of the exception
	virtual const int execode() = 0;
};

class intException : public psException
{
public:

	const int execode()
	{
		return 0;
	}

};

class modException : public psException
{
public:
	const int execode()
	{
		return 1;
	}

};

class tlblException : public psException
{
public:
	const int execode()
	{
		return 2;
	}

};

class tlbsException : public psException
{
public:
	const int execode()
	{
		return 3;
	}

};

class adelException : public psException
{
public:
	const int execode()
	{
		return 4;
	}

};

class adesException : public psException
{
public:
	const int execode()
	{
		return 5;
	}

};

class ibeException : public psException
{
public:
	const int execode()
	{
		return 6;
	}

};

class dbeException : public psException
{
public:
	const int execode()
	{
		return 7;
	}

};

class sysException : public psException
{
public:
	const int execode()
	{
		return 8;
	}

};

class bpException : public psException
{
public:
	const int execode()
	{
		return 9;
	}

};

class riException : public psException
{
public:
	const int execode()
	{
		return 10;
	}

};

class cpuException : public psException
{
public:
	const int execode()
	{
		return 11;
	}

};

class ovfException : public psException
{
public:
	const int execode()
	{
		return 12;
	}

};

#endif
