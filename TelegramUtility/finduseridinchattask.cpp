#include "finduseridinchattask.h"


FindUserIDInChatTask::FindUserIDInChatTask(std::string inputUserName, td::td_api::int53 inputChatID, TelegramClient* inputNewPtrTelegramClient) : AbstractTask(inputNewPtrTelegramClient)
{
	userName = inputUserName;
	chatID = inputChatID;
}

TaskType FindUserIDInChatTask::getType()
{
	return type;
}

bool FindUserIDInChatTask::execute()
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
			std::cout << "Ищем: " << userName << ", в чате с ID=" << chatID << std::endl;

			td::td_api::object_ptr<td::td_api::Function> searchChatMembers = td::td_api::make_object<td::td_api::searchChatMembers>(chatID, userName, 10, nullptr);

			makeAndSendQuery(searchChatMembers, true);

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

void FindUserIDInChatTask::readResponse(td::ClientManager::Response& response)
{
	if(response.object->get_id() == td::td_api::error::ID)
	{
		std::cout << "Получили ошибку на запрос с ID=" << response.request_id << std::endl;
		std::cout << to_string(response.object) << std::endl;
	}
	else
	{
		td::tl_object_ptr<td::td_api::chatMembers> chatMembers = td::move_tl_object_as<td::td_api::chatMembers>(response.object);

		std::cout << "Найдено пользователей: " << chatMembers->members_.size() << std::endl;

		for(int i = 0; i < chatMembers->members_.size(); i++)
		{
			switch(chatMembers->members_[i]->member_id_->get_id())
			{
				case td::td_api::messageSenderUser::ID:
				{
					td::tl_object_ptr<td::td_api::messageSenderUser> messageSenderUser = td::move_tl_object_as<td::td_api::messageSenderUser>(chatMembers->members_[i]->member_id_);
					std::cout << i + 1 << ". " << messageSenderUser->user_id_ << std::endl;
					break;
				}
				case td::td_api::messageSenderChat::ID:
				{
					td::tl_object_ptr<td::td_api::messageSenderChat> messageSenderChat = td::move_tl_object_as<td::td_api::messageSenderChat>(chatMembers->members_[i]->member_id_);
					std::cout << i + 1 << ". " << messageSenderChat->chat_id_ << std::endl;
					break;
				}
				default:
					break;
			}
		}
	}

	setCurrentState(ReceivedResponse);
}

void FindUserIDInChatTask::setCurrentState(State newCurrentState)
{
	currentState = newCurrentState;
}
