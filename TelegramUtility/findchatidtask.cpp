#include "findchatidtask.h"


#include <Windows.h>
#include <locale>
#include <codecvt>

FindChatIDTask::FindChatIDTask(std::string inputChatName, TelegramClient* inputNewPtrTelegramClient) : AbstractTask(inputNewPtrTelegramClient)
{
	chatName = inputChatName;
}

TaskType FindChatIDTask::getType()
{
	return type;
}

bool FindChatIDTask::execute()
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
			std::cout << "Ищем: " << chatName << std::endl;

			td::td_api::object_ptr<td::td_api::Function> searchChatsOnServer = td::td_api::make_object<td::td_api::searchChatsOnServer>(chatName, 10);

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

void FindChatIDTask::readResponse(td::ClientManager::Response& response)
{
	if(response.object->get_id() == td::td_api::error::ID)
	{
		std::cout << "Получили ошибку на запрос с ID=" << response.request_id << std::endl;
		std::cout << to_string(response.object) << std::endl;
	}
	else
	{
//		std::cout << to_string(response.object) << std::endl;
		td::tl_object_ptr<td::td_api::chats> chats = td::move_tl_object_as<td::td_api::chats>(response.object);

		std::cout << "Найдено чатов: " << chats->chat_ids_.size() << std::endl;

		for(int i = 0; i < chats->chat_ids_.size(); i++)
		{
			std::cout << i + 1 << ". " << chats->chat_ids_[i] << std::endl;
		}
	}

	setCurrentState(ReceivedResponse);
}

void FindChatIDTask::setCurrentState(State newCurrentState)
{
	currentState = newCurrentState;
}
