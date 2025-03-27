#ifndef FINDUSERIDTASK_H
#define FINDUSERIDTASK_H


#include "abstracttask.h"

class FindUserIDTask : public AbstractTask
{

public:
	FindUserIDTask(std::string inputUserName, TelegramClient* inputNewPtrTelegramClient);

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


	void setCurrentState(State newCurrentState);

	TaskType type = TaskType::FindUserID;


	State currentState = StartTask;

};


#endif // FINDUSERIDTASK_H
