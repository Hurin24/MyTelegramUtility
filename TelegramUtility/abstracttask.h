#ifndef ABSTRACTTASK_H
#define ABSTRACTTASK_H


#include <unordered_map>


#include "telegramclient.h"


enum TaskType
{
	CloseApplication = 0,
	LogIn,
	LogOut,
	FindChatID,
	FindUserID,
	FindUserIDInChat,
	DeleteMessageInChat,
	DeleteMessageInChatFromUser
};

class AbstractTask
{
	friend class TelegramClient;


public:
	AbstractTask(TelegramClient* inputNewPtrTelegramClient);

	virtual ~AbstractTask();

	virtual TaskType getType() = 0;

	bool getInProcess();

	virtual void parseAndReadResponse(td::ClientManager::Response& response);

	virtual bool execute() = 0;


protected:
	TelegramClient* ptrTelegramClient = nullptr;

	bool inProcess = false;

	std::unordered_map<std::uint64_t, std::unordered_map<std::uint64_t, AbstractTask*>::iterator> queryIDHash;

	virtual void makeAndSendQuery(td::td_api::object_ptr<td::td_api::Function>& inputFunction, bool waitResponse = true);

	virtual void readResponse(td::ClientManager::Response& response) = 0;
};

#endif // ABSTRACTTASK_H
