#ifndef LOGOUTTASK_H
#define LOGOUTTASK_H


#include "abstracttask.h"


class LogOutTask : public AbstractTask
{

public:
	LogOutTask(TelegramClient* inputNewPtrTelegramClient);

	virtual TaskType getType() override;

	virtual bool execute() override;


protected:
	virtual void readResponse(td::ClientManager::Response& response) override;

	void setAuthorizationStateID(std::int32_t newAuthorizationStateID);

	void authorizationStateIDChanged();


private:
	TaskType type = TaskType::LogOut;

	std::int32_t authorizationStateID = 0;
};


#endif // LOGOUTTASK_H
