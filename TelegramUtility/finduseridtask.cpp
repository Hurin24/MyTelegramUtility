#include "finduseridtask.h"


FindUserIDTask::FindUserIDTask(std::string inputUserName, TelegramClient* inputNewPtrTelegramClient) : AbstractTask(inputNewPtrTelegramClient)
{
	userName = inputUserName;
}

TaskType FindUserIDTask::getType()
{
	return type;
}

bool FindUserIDTask::execute()
{
	switch(currentState)
	{
		case StartTask:
		{
			setCurrentState(SendingQuery);
			break;
		}
		case SendingQuery:
		{
			std::cout << "Ищем: " << userName << std::endl;

			td::td_api::object_ptr<td::td_api::Function> searchChatsOnServer = td::td_api::make_object<td::td_api::searchContacts>(userName, 10);

			makeAndSendQuery(searchChatsOnServer, true);

			setCurrentState(WaitResponse);
			break;
		}
		case WaitResponse:
		{
			break;
		}
		case ReceivedResponse:
		{
			setCurrentState(EndTask);
			break;
		}
		case EndTask:
		{
			inProcess = false;
			break;
		}
		default:
			break;
	}

	return inProcess;
}

void FindUserIDTask::readResponse(td::ClientManager::Response& response)
{
	if(response.object->get_id() == td::td_api::error::ID)
	{
		std::cout << "Получили ошибку на запрос с ID=" << response.request_id << std::endl;
		std::cout << to_string(response.object) << std::endl;
	}
	else
	{
		td::tl_object_ptr<td::td_api::users> users = td::move_tl_object_as<td::td_api::users>(response.object);

		std::cout << "Найдено пользователей: " << users->user_ids_.size() << std::endl;

		for(int i = 0; i < users->user_ids_.size(); i++)
		{
			std::cout << i + 1 << ". " << users->user_ids_[i] << std::endl;
		}
	}

	setCurrentState(ReceivedResponse);
}

void FindUserIDTask::setCurrentState(State newCurrentState)
{
	currentState = newCurrentState;
}
