#include "logintask.h"


LogInTask::LogInTask(TelegramClient* inputNewPtrTelegramClient) : AbstractTask(inputNewPtrTelegramClient)
{

}

TaskType LogInTask::getType()
{
	return type;
}

bool LogInTask::execute()
{
	setAuthorizationStateID(ptrTelegramClient->authorizationState->get_id());

	return inProcess;
}

void LogInTask::readResponse(td::ClientManager::Response& response)
{
	if(response.object->get_id() == td::td_api::error::ID)
	{
		std::cout << "Получили ошибку на запрос с ID=" << response.request_id << std::endl;
		std::cout << to_string(response.object) << std::endl;

		authorizationStateID = 0;
	}
}

void LogInTask::setAuthorizationStateID(int32_t newAuthorizationStateID)
{
	if(authorizationStateID != newAuthorizationStateID)
	{
		authorizationStateID = newAuthorizationStateID;
		authorizationStateIDChanged();
	}
}

void LogInTask::authorizationStateIDChanged()
{
	switch(authorizationStateID)
	{
		case td::td_api::authorizationStateWaitTdlibParameters::ID:
		{
			td::td_api::object_ptr<td::td_api::Function> setTdlibParameters = td::td_api::make_object<td::td_api::setTdlibParameters>(false,
																																	  ptrTelegramClient->databaseDirectory,
																																	  ptrTelegramClient->filesDirectory,
																																	  ptrTelegramClient->databaseEncryptionKey,
																																	  ptrTelegramClient->useFileDatabase,
																																	  ptrTelegramClient->useChatInfoDatabase,
																																	  ptrTelegramClient->useMessageDatabase,
																																	  ptrTelegramClient->useSecretChats,
																																	  ptrTelegramClient->APIID,
																																	  ptrTelegramClient->APIHash,
																																	  ptrTelegramClient->systemLanguageCode,
																																	  ptrTelegramClient->deviceModel,
																																	  ptrTelegramClient->systemVersion,
																																	  ptrTelegramClient->applicationVersion);


			makeAndSendQuery(setTdlibParameters, true);
			break;
		}
		case td::td_api::authorizationStateWaitPhoneNumber::ID:
		{
			std::string phoneNumber;


			std::cout << "Введите номер телефона: " << std::flush;
			std::cin >> phoneNumber;

			td::td_api::object_ptr<td::td_api::Function> setAuthenticationPhoneNumber = td::td_api::make_object<td::td_api::setAuthenticationPhoneNumber>(phoneNumber, nullptr);

			makeAndSendQuery(setAuthenticationPhoneNumber, true);
			break;
		}
		case td::td_api::authorizationStateWaitCode::ID:
		{
			std::string code;


			std::cout << "Введите код аутентификации: " << std::flush;
			std::cin >> code;


			td::td_api::object_ptr<td::td_api::Function> checkAuthenticationCode = td::td_api::make_object<td::td_api::checkAuthenticationCode>(code);

			makeAndSendQuery(checkAuthenticationCode, true);
			break;
		}
		case td::td_api::authorizationStateWaitEmailAddress::ID:
		{
			std::string emailAddress;


			std::cout << "Введите электронную почту: " << std::flush;
			std::cin >> emailAddress;


			td::td_api::object_ptr<td::td_api::Function> setAuthenticationEmailAddress = td::td_api::make_object<td::td_api::setAuthenticationEmailAddress>(emailAddress);

			makeAndSendQuery(setAuthenticationEmailAddress, true);
			break;
		}
		case td::td_api::authorizationStateWaitEmailCode::ID:
		{
			std::string code;


			std::cout << "Введите код аутентификации из электронного письма: " << std::flush;
			std::cin >> code;


			td::td_api::object_ptr<td::td_api::Function> checkAuthenticationEmailCode = td::td_api::make_object<td::td_api::checkAuthenticationEmailCode>(td::td_api::make_object<td::td_api::emailAddressAuthenticationCode>(code));

			makeAndSendQuery(checkAuthenticationEmailCode, true);
			break;
		}
		case td::td_api::authorizationStateWaitRegistration::ID:
		{
			std::string firstName;
			std::string lastName;
			bool disableNotification = true;


			std::cout << "Введите своё имя: " << std::flush;
			std::cin >> firstName;
			std::cout << "Введите свою фамилию: " << std::flush;
			std::cin >> lastName;


			td::td_api::object_ptr<td::td_api::Function> registerUser = td::td_api::make_object<td::td_api::registerUser>(firstName, lastName, disableNotification);

			makeAndSendQuery(registerUser, true);
			break;
		}
		case td::td_api::authorizationStateWaitPassword::ID:
		{
			std::string password;


			std::cout << "Введите пароль: " << std::flush;
			std::getline(std::cin, password);


			td::td_api::object_ptr<td::td_api::Function> checkAuthenticationPassword = td::td_api::make_object<td::td_api::checkAuthenticationPassword>(password);

			makeAndSendQuery(checkAuthenticationPassword, true);
			break;
		}
		case td::td_api::authorizationStateWaitOtherDeviceConfirmation::ID:
		{
			td::tl_object_ptr<td::td_api::authorizationStateWaitOtherDeviceConfirmation> authorizationStateWaitOtherDeviceConfirmation = td::move_tl_object_as<td::td_api::authorizationStateWaitOtherDeviceConfirmation>(ptrTelegramClient->authorizationState);

			std::cout << "Откройте эту ссылку с другого устройства и подтвердите вход.\n" << authorizationStateWaitOtherDeviceConfirmation->link_ << std::endl;
			break;
		}
		case td::td_api::authorizationStateReady::ID:
		{
			std::cout << "Успешно авторизировались." << std::endl;
			inProcess = false;
			break;
		}
		default:
			std::cout << "Неизвестное состояние." << std::endl;
			break;
	}
}
