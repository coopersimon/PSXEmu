#ifndef PS_EXCEPTION_H
#define PS_EXCEPTION_H

#include <stdexcept>

/*
 *	PSException.h
 *
 *	Exceptions for use in the CPU
 *
 */

class psException
{
	// get the code of the exception
	virtual const unsigned execode() = 0;

};

class intException : public psException
{

	const unsigned execode()
	{
		return 0;
	}

};

class modException : public psException
{

	const unsigned execode()
	{
		return 1;
	}

};

class tlblException : public psException
{

	const unsigned execode()
	{
		return 2;
	}

};

class tlbsException : public psException
{

	const unsigned execode()
	{
		return 3;
	}

};

class adelException : public psException
{

	const unsigned execode()
	{
		return 4;
	}

};

class adesException : public psException
{

	const unsigned execode()
	{
		return 5;
	}

};

class ibeException : public psException
{

	const unsigned execode()
	{
		return 6;
	}

};

class dbeException : public psException
{

	const unsigned execode()
	{
		return 7;
	}

};

class sysException : public psException
{

	const unsigned execode()
	{
		return 8;
	}

};

class bpException : public psException
{

	const unsigned execode()
	{
		return 9;
	}

};

class riException : public psException
{

	const unsigned execode()
	{
		return 10;
	}

};

class cpuException : public psException
{

	const unsigned execode()
	{
		return 11;
	}

};

class ovfException : public psException
{

	const unsigned execode()
	{
		return 12;
	}

};

#endif
