#ifndef LOGINMANAGER_H
#define LOGINMANAGER_H

#include <string>

class Scene;
class TextInputField;
class Button;

struct LoginData{
	std::string name;
	std::string password;
	std::string ipAddress;
	bool isRegistration;
};

class LoginManager
{
	public:
		LoginManager(Scene & scene);
		virtual ~LoginManager();


		void startLogin();
		void update();
		bool isReady()const{return isReady_;}
		void setInfo(const std::string & str);

		const LoginData & getLoginData()const {return loginData_;}


	protected:
	private:
		LoginData loginData_;
		enum LoginState{
			Idle,
			Name,
			Password,
			ServerAddress,
			RegisterUser
		};
		LoginState state_;
		Scene & scene_;

		void setState(LoginState state);

		TextInputField * activeInput;
		Button * loginButton;
		Button * registerButton;
		bool isReady_;

};

#endif // LOGINMANAGER_H
