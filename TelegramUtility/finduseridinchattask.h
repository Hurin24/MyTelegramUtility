#ifndef FINDUSERIDINCHAT_H
#define FINDUSERIDINCHAT_H


#include "abstracttask.h"

class FindUserIDInChatTask : public AbstractTask
{

public:
	FindUserIDInChatTask(std::string inputUserName, td::td_api::int53 inputChatID, TelegramClient* inputNewPtrTelegramClient);

	virtual TaskType getType() override;

	virtual bool execute() override;


protected:
	virtual void readResponse(td::ClientManager::Response& response) override;


private:
	enum State
	{
		StartTask = 0,
		WaitInput,
		SendingQuery,
		WaitResponse,
		ReceivedResponse,
		EndTask
	};


	std::string userName = "";
	td::td_api::int53 chatID = 0;

	void setCurrentState(State newCurrentState);

	TaskType type = TaskType::FindUserID;


	State currentState = StartTask;

};


#endif // FINDUSERIDINCHAT_H
