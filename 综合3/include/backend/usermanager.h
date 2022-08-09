#ifndef B_USERMANAGER_H
#define B_USERMANAGER_H
#include "../misc/duser.h"
#include "../misc/cuser.h"
#include "../misc/manager.h"
#include "../common/usermanager.h"
#include <map>
#include <string>
#include <vector>
using namespace std;
namespace Backend
{
	class UserManager : Common::UserManager
	{
	public:
		UserManager(string userFile);
		~UserManager();
		bool reg(User* user);
		bool del(User* user);
		bool login(string username, string password);
		bool logout();
		bool addFund(int amount);
		bool changePassword(string password);
		User* getUserByName(string username) const;
		bool setCurrentUser(string username);
		map<string, User*> listUser() const;
	private:
		string userFile;
		map <string, User*> userList;
		User* _getUserByName(string username) const;
		void save();
		void load();
	};
};
#endif
