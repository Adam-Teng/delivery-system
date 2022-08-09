#ifndef C_USERMANAGER_H
#define C_USERMANAGER_H
#include "../misc/duser.h"
#include "../misc/cuser.h"
#include "../misc/manager.h"
#include "../common/usermanager.h"
#include "httpclient.h"
#include <map>
#include <string>
#include <vector>
using namespace std;
namespace Client {
	class UserManager : Common::UserManager
	{
	public:
		UserManager(xHttpClient* Client);
		~UserManager();
		bool reg(User* user);
		bool del(User* user);
		bool changePassword(string Password);
		map<string, User*> listUser() const;
		bool login(string username, string password);
		bool logout();
		bool addFund(int amount);
		User* getUserByName(string username) const;
		void load();
		void loadCurrentUser();
	private:
		xHttpClient* client;
		map <string, User*> userList;
		User* _getUserByName(string username) const;
	};
}

#endif // !C_USERMANAGER_H
