#ifndef TELEGRAMCLIENT_H
#define TELEGRAMCLIENT_H


#include <iostream>
#include <Windows.h>

#include <unordered_map>
#include <map>

#include <td/telegram/Client.h>
#include <td/telegram/td_api.h>
#include <td/telegram/td_api.hpp>


#include "personaldata.h"

enum TaskType;


class AbstractTask;
class CloseApplication0Task;
class LogInTask;
class LogOutTask;
class FindChatIDTask;
class FindUserIDTask;
class FindUserIDInChatTask;
class DeleteMessageInChatTask;
class DeleteMessageInChatFromUseTask;

class TelegramClient
{
	friend class AbstractTask;
	friend class CloseApplicationTask;
	friend class LogInTask;
	friend class LogOutTask;
	friend class FindChatIDTask;
	friend class FindUserIDTask;
	friend class FindUserIDInChatTask;
	friend class DeleteMessageInChatTask;
	friend class DeleteMessageInChatFromUseTask;


public:
	TelegramClient();


	void loop();


private:
	enum State
	{
		Initialization = 0,
		Unauthorized,
		InTheAuthorizationProcess,
		Authorized,
		ExecutingTask,
		WaitingTask,
		ClosingApplication
	};

	void trimFrontSpaces(std::string& inputSting);
	int findFirstNotSpaceChar(std::string& inputSting, int startPos = 0);
	int findFirstSpaceChar(std::string& inputSting, int startPos = 0);

	std::string cp1251_to_utf8(const char* str);
	int parseCommand(std::string& inputCommand);

	void init();

	std::unordered_map<std::uint64_t, AbstractTask*>::iterator sendQuery(td::td_api::object_ptr<td::td_api::Function>& inputFunction, AbstractTask* ptrTask = nullptr);

	void recvResponse(td::ClientManager::Response& response);

	void readResponse(td::td_api::object_ptr<td::td_api::Object>& response);

	void setAuthorizationState(td::td_api::object_ptr<td::td_api::Object>& response);

	void authorizationStateChanged();

	void setCurrentState(State newCurrenState);

	void currentStateChanged();

	td::td_api::int53 messageTDLibID_To_messageLinkID(td::td_api::int53 message_id);

	td::td_api::int53 messageLinkID_To_messageTDLibID(td::td_api::int53 message_id);

	std::uint64_t nextQueryID();

	int tempInt = 0;

	td::td_api::int53 chatID = -1002036657839;
	td::td_api::int53 userID = 0;
	td::td_api::int53 startMessageID = 0;
	td::td_api::int53 endMessageID = 0;

	td::td_api::int53 currentMessageID = 0;
	td::td_api::int53 countDeleted = 0;
	bool needDelete = false;


	std::vector<td::td_api::int53> vectorMessageID;
	std::unordered_map<std::uint64_t, AbstractTask*> queryIDHash;

//	std::string databaseDirectory = "databaseTelegramUtility";
	std::string databaseDirectory = "database";
	std::string filesDirectory = "";
	std::string databaseEncryptionKey = "";
	bool useFileDatabase = false;
	bool useChatInfoDatabase = false;
	bool useMessageDatabase = false;
	bool useSecretChats = true;
	td::td_api::int32 APIID = API_ID;
	std::string APIHash = API_HASH;
	std::string systemLanguageCode = "en";
	std::string deviceModel = "Desktop";
	std::string systemVersion = "";
	std::string applicationVersion = "1.0";


	std::unique_ptr<td::ClientManager> clientManager;
	std::int32_t clientID{0};

	td::td_api::object_ptr<td::td_api::AuthorizationState> authorizationState;

	std::int32_t authorizationStateID = 0;

	std::uint64_t currentQueryID = 0;

	State currentState = Initialization;
	std::list<AbstractTask*> taskList;


	std::map<std::string, TaskType> commandMap;
};

#endif // TELEGRAMCLIENT_H
