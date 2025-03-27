#ifndef FINDCHATIDTASK_H
#define FINDCHATIDTASK_H

#include "abstracttask.h"

class FindChatIDTask : public AbstractTask
{

public:
	FindChatIDTask(std::string inputChatName, TelegramClient* inputNewPtrTelegramClient);

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


	std::string chatName = "";


	void setCurrentState(State newCurrentState);

	TaskType type = TaskType::FindChatID;


	State currentState = StartTask;

};


#endif // FINDCHATIDTASK_H
