#ifndef PS_EXCEPTION_H
#define PS_EXCEPTION_H

#include <stdexcept>

/*
 *	PSException.h
 *
 *	Exceptions for use in the CPU
 *
 */

struct psException
{
	// get the code of the exception
	const int execode()
	{
		return -1;
	}

};

struct intException : public psException
{

	const int execode()
	{
		return 0;
	}

};

struct modException : public psException
{

	const int execode()
	{
		return 1;
	}

};

struct tlblException : public psException
{

	const int execode()
	{
		return 2;
	}

};

struct tlbsException : public psException
{

	const int execode()
	{
		return 3;
	}

};

struct adelException : public psException
{

	const int execode()
	{
		return 4;
	}

};

struct adesException : public psException
{

	const int execode()
	{
		return 5;
	}

};

struct ibeException : public psException
{

	const int execode()
	{
		return 6;
	}

};

struct dbeException : public psException
{

	const int execode()
	{
		return 7;
	}

};

struct sysException : public psException
{

	const int execode()
	{
		return 8;
	}

};

struct bpException : public psException
{

	const int execode()
	{
		return 9;
	}

};

struct riException : public psException
{

	const int execode()
	{
		return 10;
	}

};

struct cpuException : public psException
{

	const int execode()
	{
		return 11;
	}

};

struct ovfException : public psException
{

	const int execode()
	{
		return 12;
	}

};

#endif
