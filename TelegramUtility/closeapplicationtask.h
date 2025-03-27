#ifndef CLOSEAPPLICATIONTASK_H
#define CLOSEAPPLICATIONTASK_H


#include "abstracttask.h"


class CloseApplicationTask : public AbstractTask
{

public:
	CloseApplicationTask(TelegramClient* inputNewPtrTelegramClient);

	virtual TaskType getType() override;

	virtual bool execute() override;

protected:
	virtual void readResponse(td::ClientManager::Response& response)
	{

	}


private:
	TaskType type = TaskType::CloseApplication;

};

#endif // CLOSEAPPLICATIONTASK_H
