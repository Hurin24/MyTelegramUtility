#include "logouttask.h"

LogOutTask::LogOutTask(TelegramClient* inputNewPtrTelegramClient) : AbstractTask(inputNewPtrTelegramClient)
{

}

TaskType LogOutTask::getType()
{
	return type;
}

bool LogOutTask::execute()
{
	setAuthorizationStateID(ptrTelegramClient->authorizationState->get_id());

	return inProcess;
}

void LogOutTask::readResponse(td::ClientManager::Response& response)
{
	if(response.object->get_id() == td::td_api::error::ID)
	{
		std::cout << to_string(response.object) << std::endl;

		authorizationStateID = 0;
	}
}

void LogOutTask::setAuthorizationStateID(int32_t newAuthorizationStateID)
{
	if(authorizationStateID != newAuthorizationStateID)
	{
		authorizationStateID = newAuthorizationStateID;
		authorizationStateIDChanged();
	}
}

void LogOutTask::authorizationStateIDChanged()
{
	switch(authorizationStateID)
	{
		case td::td_api::authorizationStateReady::ID:
		{
			td::td_api::object_ptr<td::td_api::Function> checkAuthenticationPassword = td::td_api::make_object<td::td_api::logOut>();

			makeAndSendQuery(checkAuthenticationPassword, true);
			std::cout << "Начинаем выход из аккаутна..." << std::endl;
			break;
		}
		case td::td_api::authorizationStateLoggingOut ::ID:
		{
			std::cout << "В процессе выхода из аккаутна..." << std::endl;
			break;
		}
		case td::td_api::authorizationStateClosed::ID:
		{
			std::cout << "Успешно вышли из аккаутна..." << std::endl;
			inProcess = false;
			break;
		}
		default:
			std::cout << "Неизвестное состояние." << std::endl;
			break;
	}
}
