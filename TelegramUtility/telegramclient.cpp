#include "telegramclient.h"


#include "abstracttask.h"
#include "closeapplicationtask.h"
#include "logintask.h"
#include "logouttask.h"
#include "findchatidtask.h"
#include "finduseridtask.h"
#include "finduseridinchattask.h"
#include <regex>


TelegramClient::TelegramClient()
{
	commandMap.insert({"quit", TaskType::CloseApplication});
	commandMap.insert({"log_out", TaskType::LogOut});
	commandMap.insert({"find_chat_ID", TaskType::FindChatID});
	commandMap.insert({"find_user_ID", TaskType::FindUserID});
	commandMap.insert({"find_user_ID_in_chat", TaskType::FindUserIDInChat});
	commandMap.insert({"delete_message_in_chat", TaskType::DeleteMessageInChat});
	commandMap.insert({"delete_message_in_chat_from_user", TaskType::DeleteMessageInChatFromUser});

	init();
}

void TelegramClient::loop()
{
	while(true)
	{
		auto response = clientManager->receive(1);

		if(response.object)
		{
			recvResponse(response);
		}

		if(currentState != Initialization)
		{
			if(!taskList.empty())
			{
				taskList.front()->execute();

				if(!taskList.front()->getInProcess())
				{
					taskList.pop_front();
				}
			}
			else
			{
				std::cout << "Введите задачу:\n"
							 "quit - закрыть программу\n"
							 "log_out - разлогиниться\n"
							 "\"find_chat_ID\" <имя чата> - получить ID чата по имени\n"
							 "\"find_user_ID\" <имя пользователя> - получить ID пользователя по имени\n"
							 "\"find_user_ID in chat\" <имя пользователя> <ID чата> - получить ID пользователя по имени в указанном, с помощью ID, чате\n"
							 "\"delete_message_in_chat\" <ID самого позднего сообщения> <id самого раннего сообщения> - удалить сообщения с самого позднего ID по самое раннее ID\n"
							 "\"delete_message_in_chat_from_user\" <ID самого позднего сообщения> <ID самого раннего сообщения> <ID пользователя> - удалить сообщения с самого позднего ID по самое раннее ID от указанного, с помощью ID, пользователя\n";


				std::string newTask;
				std::getline(std::cin, newTask);
				std::cout << std::endl;




				auto findIterator = commandMap.lower_bound(newTask);

				--findIterator;


				if(findIterator != commandMap.end())
				{
					std::cout << "Найдено\n";
					std::cout << findIterator->second;
				}
				else
				{
					std::cout << "Не найдено\n";
				}


//				int newTask = 0;

//				switch(newTask)
//				{
//					case TaskType::CloseApplication:
//					{
//						break;
//					}
//					case TaskType::LogOut:
//					{
//						break;
//					}
//					case TaskType::FindChatID:
//					{
//						break;
//					}
//					case TaskType::FindUserID:
//					{
//						break;
//					}
//					case TaskType::FindUserIDInChat:
//					{
//						break;
//					}
//					case TaskType::DeleteMessageInChat:
//					{
//						break;
//					}
//					case TaskType::DeleteMessageInChatFromUser:
//					{
//						break;
//					}
//					default:
//						break;
//				}


				std::string findString = "log out";

				int pos = newTask.find(findString);

				if(pos != -1)
				{
					AbstractTask* ptrAbstractTask = new LogOutTask(this);

					taskList.push_back(ptrAbstractTask);
					std::cout << "taskList.push_back(LogOutTask)" << std::endl;
					continue;
				}


				findString = "find chat ID";

				pos = -1;
				pos = newTask.find(findString);

				if(pos != -1)
				{
					std::string newString = newTask.substr(pos + findString.size());

					//Удаляем пробелы спереди
					trimFrontSpaces(newString);

					//Переводим из 1251 в utf8
					newString = cp1251_to_utf8(newString.data());

					AbstractTask* ptrAbstractTask = new FindChatIDTask(newString, this);

					taskList.push_back(ptrAbstractTask);
					std::cout << "taskList.push_back(FindChatID)" << std::endl;
					continue;
				}


				findString = "find user ID in chat";

				pos = -1;
				pos = newTask.find(findString);

				if(pos != -1)
				{
					std::string newString = newTask.substr(pos + findString.size());

					//Удаляем пробелы спереди
					trimFrontSpaces(newString);

					//Переводим из 1251 в utf8
					newString = cp1251_to_utf8(newString.data());

					int firstSpace = newString.find_first_of(' ');



					std::string userName = newString.substr(0, firstSpace);
					trimFrontSpaces(userName);

					std::string chatID = newString.substr(firstSpace);


					td::td_api::int53 intChatID = 0;

					try
					{
						intChatID = std::stoll(chatID);
					}
					catch(std::invalid_argument const& ex)
					{
						std::cout << "Некорректный ID." << std::endl;
						continue;
					}
					catch(std::out_of_range const& ex)
					{
						std::cout << "Слишком длинный ID." << std::endl;
						continue;
					}

					AbstractTask* ptrAbstractTask = new FindUserIDInChatTask(userName, intChatID, this);

					taskList.push_back(ptrAbstractTask);
					std::cout << "taskList.push_back(FindUserIDInChatTask)" << std::endl;
					continue;
				}


				findString = "find user ID";

				pos = -1;
				pos = newTask.find(findString);

				if(pos != -1)
				{
					std::string newString = newTask.substr(pos + findString.size());

					//Удаляем пробелы спереди
					trimFrontSpaces(newString);

					//Переводим из 1251 в utf8
					newString = cp1251_to_utf8(newString.data());

					AbstractTask* ptrAbstractTask = new FindUserIDTask(newString, this);

					taskList.push_back(ptrAbstractTask);
					std::cout << "taskList.push_back(FindUserIDTask)" << std::endl;
					continue;
				}
			}
		}
	}
}

void TelegramClient::trimFrontSpaces(std::string& inputSting)
{
	int countDelete = 0;

	for(int i = 0; i < inputSting.size(); i++)
	{
		if(inputSting[i] != ' ')
		{
			countDelete = i;
			break;
		}
	}

	if(countDelete > 0)
	{
		inputSting = inputSting.erase(0, countDelete);
	}
}

int TelegramClient::findFirstNotSpaceChar(std::string& inputSting, int startPos)
{
	if(startPos < 0 && startPos >= inputSting.size())
	{
		return -1;
	}


	for(int i = startPos; i < inputSting.size(); i++)
	{
		if(inputSting[i] != ' ')
		{
			countDeleted = i;
			break;
		}
	}
}

int TelegramClient::findFirstSpaceChar(std::string& inputSting, int startPos)
{

}

std::string TelegramClient::cp1251_to_utf8(const char* str)
{
	int sizeTransform1 = MultiByteToWideChar(1251, 0, str, -1, 0, 0);

	if(!sizeTransform1)
	{
		return "";
	}


	std::wstring stringWideChar;
	stringWideChar.resize(sizeTransform1);


	bool resultTransform1 = MultiByteToWideChar(1251, 0, str, -1, stringWideChar.data(), sizeTransform1);

	if(!resultTransform1)
	{
		return "";
	}



	int sizeTransform2 = WideCharToMultiByte(CP_UTF8, 0, stringWideChar.data(), -1, 0, 0, 0, 0);

	if(!sizeTransform2)
	{
		return "";
	}


	std::string stringMultiByte;
	stringMultiByte.resize(sizeTransform2);


	bool resultTransform2 = WideCharToMultiByte(CP_UTF8, 0, stringWideChar.data(), -1, stringMultiByte.data(), sizeTransform2, 0, 0);

	if(!resultTransform2)
	{
		return "";
	}



	return stringMultiByte;
}

int TelegramClient::parseCommand(std::string& inputCommand)
{
//	quit
//	log out
//	find chat ID
//	find user ID
//	find user ID in chat
//	delete message in chat
//	delete message in chat from user
//	if(!inputCommand.empty())
//	{
//		switch(inputCommand[0])
//		{
//		case 'd':
//		{
//			break;
//		}
//		case 'f':
//		{
//			break;
//		}
//		case 'l':
//		{
//			break;
//		}
//		case 'l':
//		{
//			break;
//		}
//		}
//	}

	for(int i = 0; i < inputCommand.size(); i++)
	{

	}
}

void TelegramClient::init()
{
	clientManager.reset();

	td::ClientManager::execute(td::td_api::make_object<td::td_api::setLogVerbosityLevel>(0));

	clientManager = std::make_unique<td::ClientManager>();

	clientID = clientManager->create_client_id();


	td::td_api::object_ptr<td::td_api::Function> getOption = td::td_api::make_object<td::td_api::getOption>("version");

	sendQuery(getOption);
}

std::unordered_map<std::uint64_t, AbstractTask*>::iterator TelegramClient::sendQuery(td::td_api::object_ptr<td::td_api::Function>& inputFunction, AbstractTask* ptrTask)
{
	auto newQueryID = nextQueryID();

	clientManager->send(clientID, newQueryID, std::move(inputFunction));

	if(ptrTask)
	{
		return queryIDHash.insert({newQueryID, ptrTask}).first;
	}
	else
	{
		return queryIDHash.end();
	}
}

void TelegramClient::recvResponse(td::ClientManager::Response& response)
{
	if(!response.object)
	{
		return;
	}


	if(response.request_id == 0)
	{
		readResponse(response.object);

		return;
	}


	std::unordered_map<std::uint64_t, AbstractTask*>::iterator findIterator = queryIDHash.find(response.request_id);

	if(findIterator != queryIDHash.end())
	{
		findIterator->second->parseAndReadResponse(response);
		queryIDHash.erase(findIterator);
	}
	else
	{
		std::cout << "Получили ответ на запрос с неизвестным ID=" << response.request_id << std::endl;
		std::cout << to_string(response.object) << std::endl;
	}
}

void TelegramClient::readResponse(td::td_api::object_ptr<td::td_api::Object>& response)
{
	switch(response->get_id())
	{
		case td::td_api::updateAuthorizationState::ID:
		{
			setAuthorizationState(response);
			break;
		}
		default:
			break;
	}
}

void TelegramClient::setAuthorizationState(td::td_api::object_ptr<td::td_api::Object>& response)
{
	td::tl_object_ptr<td::td_api::updateAuthorizationState> updateAuthorizationState = td::move_tl_object_as<td::td_api::updateAuthorizationState>(response);

	if(authorizationStateID != updateAuthorizationState->authorization_state_->get_id())
	{
		authorizationState = std::move(updateAuthorizationState->authorization_state_);
		authorizationStateID = authorizationState->get_id();

		authorizationStateChanged();
	}
}

void TelegramClient::authorizationStateChanged()
{
	switch(authorizationStateID)
	{
		case td::td_api::authorizationStateReady::ID:
		{
			setCurrentState(Authorized);
			break;
		}
		case td::td_api::authorizationStateWaitTdlibParameters::ID:
		{
			setCurrentState(Unauthorized);
			break;
		}
		case td::td_api::authorizationStateClosed::ID:
		{
			setCurrentState(Unauthorized);
			init();
			break;
		}
		default:
			break;
	}
}

void TelegramClient::setCurrentState(State newCurrenState)
{
	if(currentState != newCurrenState)
	{
		currentState = newCurrenState;
		currentStateChanged();
	}
}

void TelegramClient::currentStateChanged()
{
	switch(currentState)
	{
		case Unauthorized:
		{
			AbstractTask* ptrAbstractTask = new LogInTask(this);

			taskList.push_back(ptrAbstractTask);
			std::cout << "taskList.push_back(LogIn)" << std::endl;
			break;
		}
		case InTheAuthorizationProcess:
		{
			break;
		}
		case Authorized:
		{
			break;
		}
		case ExecutingTask:
		{
			break;
		}
		case WaitingTask:
		{
			break;
		}
		case ClosingApplication:
		{
			break;
		}
		default:
			break;
	}
}

td::td_api::int53 TelegramClient::messageTDLibID_To_messageLinkID(td::td_api::int53 message_id)
{
	return message_id / 1048576;
}

td::td_api::int53 TelegramClient::messageLinkID_To_messageTDLibID(td::td_api::int53 message_id)
{
	return message_id * 1048576;
}

std::uint64_t TelegramClient::nextQueryID()
{
	return ++currentQueryID;
}
