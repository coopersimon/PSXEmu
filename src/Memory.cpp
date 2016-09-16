#include <Memory.h>
#include <PSException.h>
#include <iostream>

// TODO: dealing with multiple bytes might need improvement. this needs to be quick.
// these also need to be carefully tested. casting etc

int memBus::find(unsigned address)
{
	//std::cout << "address: " << address << std::endl;
	for (unsigned i = 0; i < bus_list.size(); i++)
	{
		if ((address >= bus_list[i].address_start) && (address <= bus_list[i].address_end))
		{
			return i;
		}
	}		
	//TODO: pick proper exception
	throw adesException();
}

byte memBus::readByte(unsigned address)
{
	return bus_list[find(address)].memory_device->readByte(address);
}

void memBus::writeByte(unsigned address, byte in)
{
	bus_list[find(address)].memory_device->writeByte(address, in);
}

halfword memBus::readHalfwordLittle(unsigned address)
{
	return bus_list[find(address)].memory_device->readHalfwordLittle(address);
}

void memBus::writeHalfwordLittle(unsigned address, halfword in)
{
	bus_list[find(address)].memory_device->writeHalfwordLittle(address, in);
}

halfword memBus::readHalfwordBig(unsigned address)
{
	return bus_list[find(address)].memory_device->readHalfwordBig(address);
}

void memBus::writeHalfwordBig(unsigned address, halfword in)
{
	bus_list[find(address)].memory_device->writeHalfwordBig(address, in);
}

word memBus::readWordLittle(unsigned address)
{
	return bus_list[find(address)].memory_device->readWordLittle(address);
}

void memBus::writeWordLittle(unsigned address, word in)
{
	bus_list[find(address)].memory_device->writeWordLittle(address, in);
}

word memBus::readWordBig(unsigned address)
{
	return bus_list[find(address)].memory_device->readWordBig(address);
}

void memBus::writeWordBig(unsigned address, word in)
{
	bus_list[find(address)].memory_device->writeWordBig(address, in);
}

void RAMImpl::writeByte(unsigned address, byte in)
{
	write(maskAddress(address), in);
}

byte RAMImpl::readByte(unsigned address)
{
	byte out;
	read(maskAddress(address), out);
	return out;
}

halfword RAMImpl::readHalfwordLittle(unsigned address)
{
	byte bytes[2];
	readMultiple(maskAddress(address), bytes, 2);
	return bytes[0] | (bytes[1] << 8);
}

void RAMImpl::writeHalfwordLittle(unsigned address, halfword in)
{
	byte bytes[2];
	bytes[0] = in & 0xFF;
	bytes[1] = (in >> 8) & 0xFF;
	writeMultiple(maskAddress(address), bytes, 2);
}

halfword RAMImpl::readHalfwordBig(unsigned address)
{
	byte bytes[2];
	readMultiple(maskAddress(address), bytes, 2);
	return bytes[1] | (bytes[0] << 8);
}

void RAMImpl::writeHalfwordBig(unsigned address, halfword in)
{
	byte bytes[2];
	bytes[1] = in & 0xFF;
	bytes[0] = (in >> 8) & 0xFF;
	writeMultiple(maskAddress(address), bytes, 2);
}

word RAMImpl::readWordLittle(unsigned address)
{
	byte bytes[4];
	readMultiple(maskAddress(address), bytes, 4);
	return bytes[0] | (bytes[1] << 8) | (bytes[2] << 16) | (bytes[3] << 24);
}

void RAMImpl::writeWordLittle(unsigned address, word in)
{
	byte bytes[4];
	bytes[0] = in & 0xFF;
	bytes[1] = (in >> 8) & 0xFF;
	bytes[2] = (in >> 16) & 0xFF;
	bytes[3] = (in >> 24) & 0xFF;
	writeMultiple(maskAddress(address), bytes, 4);
}

word RAMImpl::readWordBig(unsigned address)
{
	byte bytes[4];
	readMultiple(maskAddress(address), bytes, 4);
	return bytes[3] | (bytes[2] << 8) | (bytes[1] << 16) | (bytes[0] << 24);
}

void RAMImpl::writeWordBig(unsigned address, word in)
{
	byte bytes[4];
	bytes[3] = in & 0xFF;
	bytes[2] = (in >> 8) & 0xFF;
	bytes[1] = (in >> 16) & 0xFF;
	bytes[0] = (in >> 24) & 0xFF;
	writeMultiple(maskAddress(address), bytes, 4);
}
