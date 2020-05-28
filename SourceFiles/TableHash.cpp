#include "../HeaderFiles/TableHash.h"


TableHash::RecordHash::RecordHash()
{
	busy = false;
}

Record* TableHash::FindRecord(std::string name)
{
	int step = RelativePrime(length);
	int id = Hash(name, length);
	for (int i = 0; i < length; i++)
	{
		if ((records[id].busy) && (records[id].GetName() == name))
		{
			return (records + id);
		}
		id = (id + step) % length;
	}

	return nullptr;
}

int TableHash::Hash(std::string name, int length)
{
	if (name == "")
		throw "Void name";
	int id = (name.front() * name.back()) % length;
	return id;
}

int TableHash::RelativePrime(int n)
{
	int i = 1;
	for (i = 2; i < n; i++)
	{
		if (n % i != 0)
			break;
	}
	if (i == n)
		return 1;
	else
		return i;
}

TableHash::TableHash(int startLength)
{
	if (startLength > 0)
		length = startLength;
	else
		length = 1;
	records = new RecordHash[length];
}

TableHash::~TableHash()
{
	delete[] records;
}

Table::iterator TableHash::begin()
{
	int i = 0;
	if (DataCount == 0)
		return end();
	while (!records[i].busy)
	{
		i++;
	}
	Table::iterator tmp(records + i);
	return tmp;
}

Table::iterator TableHash::end()
{
	Table::iterator tmp(records + length);
	return tmp;
}

void TableHash::Insert(std::string name, std::pair<Var*, bool>& polynomial)
{
	RecordHash* foundRecord = (RecordHash*)FindRecord(name);

	if (foundRecord != nullptr)
	{
		throw "Item already exists";
	}

	
	

	if (DataCount == length)
	{

		int prevLength = length;
		RecordHash* prevRecords = records;
		length *= 2;
		records = new RecordHash[length];

		DataCount = 0;
		for (int j = 0; j < prevLength; j++)
		{
			RecordHash curRec = prevRecords[j];
			Insert(curRec.GetName(), curRec.polynomial);
		}
		int k;

		delete[] prevRecords;
		
		
	}

	int step = RelativePrime(length);
	int id = Hash(name, length);
	for (int j = 0; records[id].busy; j++)
	{
		id = (id + step) % length;
	}


	RecordHash tmp(name, polynomial, true, records + length);
	records[id] = tmp;
	DataCount++;
}

void TableHash::Remove(std::string name)
{
	RecordHash* foundRecord = (RecordHash*)FindRecord(name);
	if (foundRecord == nullptr)
	{
		throw "Item doesn't exist";
	}

	int i;
	DataCount--;
	int stId = foundRecord - records;
	records[stId].busy = false;
	int step = RelativePrime(length);
	int remHash = Hash(name, length);
	int prevId = stId;
	for (i = (stId + step) % length; (i != remHash) && (records[i].busy) && (remHash == Hash(records[i].GetName(), length)); i = (i + step) % length)
	{
		records[prevId] = records[i];
		records[i].busy = false;
		prevId = i;
	}

}

void TableHash::Clear()
{
	delete[] records;
	DataCount = 0;
	length = 1;
	records = new RecordHash[length];

}

Record* TableHash::RecordHash::GetNext()
{
	RecordHash* next = this + 1;

	while ((next != last) && !(next->busy))
	{
		next++;
	}
	return next;
}
