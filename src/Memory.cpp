#include "Memory.h"
#include <iostream>

// dealing with multiple bytes might need improvement. this needs to be quick.
// these also need to be carefully tested. casting etc

byte RAMImpl::readByte(unsigned address) {
	byte out;
	read(maskAddress(address), out);
	return out;
}

void RAMImpl::writeByte(unsigned address, byte in) {
	write(maskAddress(address), in);
}

halfword RAMImpl::readHalfword(unsigned address) {
	if (address % 2)
		throw new adelException;
	byte bytes[2];
	unsigned masked_addr = maskAddress(address);
	readMultiple(address, bytes, 2);
	return bytes[0] | (bytes[1] << 8);
}

void RAMImpl::writeHalfword(unsigned address, halfword in) {
	if (address % 2)
		throw new adesException;
	byte bytes[2];
	unsigned masked_addr = maskAddress(address);
	bytes[0] = in & 0xFF;
	bytes[1] = (in >> 8) & 0xFF;
	writeMultiple(address, bytes, 2);
}

word RAMImpl::readWord(unsigned address) {
	if (address % 4)
		throw new adelException;
	byte bytes[4];
	unsigned masked_addr = maskAddress(address);
	readMultiple(address, bytes, 4);
	return bytes[0] | (bytes[1] << 8) | (bytes[2] << 16) | (bytes[3] << 24);
}

void RAMImpl::writeWord(unsigned address, word in) {
	if (address % 4)
		throw new adesException;
	byte bytes[4];
	unsigned masked_addr = maskAddress(address);
	bytes[0] = in & 0xFF;
	bytes[1] = (in >> 8) & 0xFF;
	bytes[2] = (in >> 16) & 0xFF;
	bytes[3] = (in >> 24) & 0xFF;
	writeMultiple(address, bytes, 4);
}


/* ram test
int main()
{
	using namespace std;
	RAMImpl ram(10); // 1024 bits

	while (true)
	{
		int loadstore, size, address, data = 0;
		byte outb = 0;
		halfword outh = 0;
		word outw = 0;

		cout << endl << "0: read or 1: write" << endl;
		cin >> loadstore;
		cout << "1: byte, 2: halfword, 4: word" << endl;
		cin >> size;
		cout << "address" << endl;
		cin >> address;
		cout << "data" << endl;
		cin >> data;

		if (loadstore == 0)
		{
			if (size == 1)
			{
				ram.readByte(address, outb);
				cout << int(outb);
			}
			else if (size == 2)
			{
				ram.readHalfword(address, outh);
				cout << int(outh);
			}
			else
			{
				ram.readWord(address, outw);
				cout << int(outw);
			}
		}
		else
		{
			if (size == 1)
			{
				data = data & 0xFF;
				ram.writeByte(address, byte(data));
			}

			else if (size == 2)
			{
				data = data & 0xFFFF;
				ram.writeHalfword(address, halfword(data));
			}

			else
				ram.writeWord(address, word(data));

		}
	}

	return 0;
}*/
