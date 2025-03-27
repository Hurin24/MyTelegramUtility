#include "abstracttask.h"


AbstractTask::AbstractTask(TelegramClient* inputNewPtrTelegramClient)
{
	ptrTelegramClient = inputNewPtrTelegramClient;

	inProcess = true;
}

AbstractTask::~AbstractTask()
{
	std::unordered_map<std::uint64_t, std::unordered_map<std::uint64_t, AbstractTask*>::iterator>::iterator hashIterator = queryIDHash.begin();

	while(hashIterator != queryIDHash.end())
	{
		ptrTelegramClient->queryIDHash.erase(hashIterator->second);
	}
}

bool AbstractTask::getInProcess()
{
	return inProcess;
}

void AbstractTask::parseAndReadResponse(td::ClientManager::Response& response)
{
	std::unordered_map<std::uint64_t, std::unordered_map<std::uint64_t, AbstractTask*>::iterator>::iterator hashIterator = queryIDHash.find(response.request_id);

	if(hashIterator != queryIDHash.end())
	{
		queryIDHash.erase(hashIterator);
	}

	readResponse(response);
}

void AbstractTask::makeAndSendQuery(td::td_api::object_ptr<td::td_api::Function>& inputFunction, bool waitResponse)
{
	if(waitResponse)
	{
		std::unordered_map<std::uint64_t, AbstractTask*>::iterator newIterator = ptrTelegramClient->sendQuery(inputFunction, this);
		queryIDHash.insert({newIterator->first, newIterator});
	}
	else
	{
		ptrTelegramClient->sendQuery(inputFunction, nullptr);
	}
}
