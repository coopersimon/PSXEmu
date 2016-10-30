#include <Memory.h>
#include <PSException.h>
#include <iostream>

// TODO: dealing with multiple bytes might need improvement. this needs to be quick.
// these also need to be carefully tested. casting etc

/*** Memory Bus ***/

int memBus::find(unsigned address)
{
      // search list for correct memory device
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

// TODO: should these be subtracting address_start?
word memBus::readWord(unsigned address)
{
      // address is divided by 4 to get word granularity
	return bus_list[find(address)].memory_device->readWord(address / 4);
}

void memBus::writeWord(unsigned address, word in)
{
      // address is divided by 4 to get word granularity
	bus_list[find(address)].memory_device->writeWord(address / 4, in);
}

/*** RAM ***/

void RAMImpl::writeWord(unsigned address, word in)
{
	write(maskAddress(address), in);
}

word RAMImpl::readWord(unsigned address)
{
	return read(maskAddress(address));
}
