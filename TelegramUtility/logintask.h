#ifndef LOGINTASK_H
#define LOGINTASK_H


#include "abstracttask.h"


class LogInTask : public AbstractTask
{

public:
	LogInTask(TelegramClient* inputNewPtrTelegramClient);

	virtual TaskType getType() override;

	virtual bool execute() override;


protected:
	virtual void readResponse(td::ClientManager::Response& response) override;

	void setAuthorizationStateID(std::int32_t newAuthorizationStateID);

	void authorizationStateIDChanged();


private:
	TaskType type = TaskType::LogIn;

	std::int32_t authorizationStateID = 0;
};

#endif // LOGINTASK_H
