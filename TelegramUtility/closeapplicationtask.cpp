#include "closeapplicationtask.h"


CloseApplicationTask::CloseApplicationTask(TelegramClient* inputNewPtrTelegramClient) : AbstractTask(inputNewPtrTelegramClient)
{

}

TaskType CloseApplicationTask::getType()
{
	return type;
}

bool CloseApplicationTask::execute()
{
	return inProcess;
}
