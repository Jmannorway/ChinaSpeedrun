#pragma once

#include <sstream>
#include <iostream>
#include <string>
#include <deque>

#include "Color.h"

#define STRING_BOOL(v) v ? "True" : "False"
#define STRING_MATRIX(m) '|' + std::to_string(m[0][0]) + ',' + std::to_string(m[0][1]) + ',' + std::to_string(m[0][2]) + ',' + std::to_string(m[0][3]) + "|\n|" + std::to_string(m[1][0]) + ',' + std::to_string(m[1][1]) + ',' + std::to_string(m[1][2]) + ',' + std::to_string(m[1][3]) + "|\n|" + std::to_string(m[2][0]) + ',' + std::to_string(m[2][1]) + ',' + std::to_string(m[2][2]) + ',' + std::to_string(m[2][3]) + "|\n|" + std::to_string(m[3][0]) + ',' + std::to_string(m[3][1]) + ',' + std::to_string(m[3][2]) + ',' + std::to_string(m[3][3]) + '|'
#define STRING_VEC2(v) '[' + std::to_string(v[0]) + ',' + std::to_string(v[1]) + ']'
#define STRING_VEC3(v) '[' + std::to_string(v[0]) + ',' + std::to_string(v[1]) + ',' + std::to_string(v[2]) + ']'
#define STRING_VEC4(v) '[' + std::to_string(v[0]) + ',' + std::to_string(v[1]) + ',' + std::to_string(v[2]) + ',' + std::to_string(v[3]) + ']'

namespace cs
{
	namespace iocolors
	{
		constexpr char WHITE[8]{ 0x1b, '[', '0', ';', '3', '9', 'm', 0 };
		constexpr char RED[8]{ 0x1b, '[', '1', ';', '3', '1', 'm', 0 };
		constexpr char GREEN[8]{ 0x1b, '[', '1', ';', '3', '2', 'm', 0 };
		constexpr char LIGHT_YELLOW[8]{ 0x1b, '[', '1', ';', '3', '3', 'm', 0 };
		constexpr char CYAN[8]{ 0x1b, '[', '1', ';', '3', '6', 'm', 0 };
	}

	class Debug
	{
	public:
		struct DebugMessage
		{
			Color color;
			std::string message;
		};

		constexpr static uint8_t messageCapacity{ 10 };

		// Log, here you may log anything.
		template<class ...Params>
		static void Log(Params&&...message);
		// Log importance, it is the same as Log, however; this is highlighted in a color.
		template<class ...Params>
		static void LogInfo(Params&&...message);
		// Log a success, this is called when something succeeds, usually when the compilation of scripts was successful.
		template<class ...Params>
		static void LogSuccess(Params&&...message);
		// Log a warning, this is not essential to your game. This is usually called when something didn't import correctly.
		template<class ...Params>
		static void LogWarning(Params&&...message);
		// Log an issue, giving the user the ability to know what is bugged and needs to be fixed.
		template<class ...Params>
		static void LogIssue(Params&&...message);
		// Calling this will prevent your game from running, calling it in-game will stop the game.
		template<class ...Params>
		static void LogError(Params&&...message);
		// Log Validation layers
		template<class ...Params>
		static void LogValidationLayer(Params&&...message);
		// Calling this closes the engine, potentially preventing damage.
		// It will also write a log file to the logs folder. (hehe not really...)
		template<class ...Params>
		static void LogFail(Params&&...message);

		static void ImGuiDrawMessages();

		template<typename T, class ...Targs>
		static std::string GetStringFromParameterPack(T& first, Targs&& ...rest);

	private:
		template<typename T>
		static std::string GetStringFromParameterPack(T& last);

		static void QueueMessage(Color color, std::string message);
		static std::deque<DebugMessage> messages;
	};
	
	template<class ...Params>
	inline void Debug::Log(Params&&...message)
	{
		std::cout << iocolors::WHITE << "[LOG]\t\t" << iocolors::WHITE << ": ";
		((std::cout << std::forward<Params>(message)), ...);
		std::cout << '\n';

		QueueMessage(Color::white, GetStringFromParameterPack(message...));
	}
	
	template<class ...Params>
	inline void Debug::LogInfo(Params&&...message)
	{
		std::cout << iocolors::CYAN << "[INFO]\t\t" << iocolors::WHITE << ": ";
		((std::cout << std::forward<Params>(message)), ...);
		std::cout << '\n';

		QueueMessage(Color::cyan, GetStringFromParameterPack(message...));
	}
	
	template<class ...Params>
	inline void Debug::LogSuccess(Params&&...message)
	{
		std::cout << iocolors::GREEN << "[SUCCESS]\t" << iocolors::WHITE << ": ";
		((std::cout << std::forward<Params>(message)), ...);
		std::cout << '\n';

		QueueMessage(Color::lime, GetStringFromParameterPack(message...));
	}
	
	template<class ...Params>
	inline void Debug::LogWarning(Params&&...message)
	{
		std::cout << iocolors::LIGHT_YELLOW << "[WARNING]\t" << iocolors::WHITE << ": ";
		((std::cout << std::forward<Params>(message)), ...);
		std::cout << '\n';
		
		QueueMessage(Color::yellow, GetStringFromParameterPack(message...));
	}
	
	template<class ...Params>
	inline void Debug::LogIssue(Params&&...message)
	{
		std::cout << iocolors::RED << "[ISSUE]\t\t" << iocolors::WHITE << ": ";
		((std::cout << std::forward<Params>(message)), ...);
		std::cout << '\n';

		QueueMessage(Color::orange, GetStringFromParameterPack(message...));
	}
	
	template<class ...Params>
	inline void Debug::LogError(Params&&...message)
	{
		std::cout << iocolors::RED << "[ERROR]\t\t" << iocolors::WHITE << ": ";
		((std::cout << std::forward<Params>(message)), ...);
		std::cout << '\n';

		QueueMessage(Color::red, GetStringFromParameterPack(message...));
	}

	template<class ...Params>
	inline void Debug::LogValidationLayer(Params && ...message)
	{
		std::cout << iocolors::RED << "[VALIDATION LAYERS] " << iocolors::WHITE << ": ";
		((std::cout << std::forward<Params>(message)), ...);
		std::cout << '\n';
	}
	
	template<class ...Params>
	inline void Debug::LogFail(Params&&...message)
	{
		std::cout << iocolors::RED << "[FAIL]\t\t" << iocolors::WHITE << ": ";
		((std::cout << std::forward<Params>(message)), ...);
		throw std::runtime_error("\n--- TERMINATED ---------------------------------------------\n");
	}
	
	template<typename T, class ...Targs>
	inline std::string Debug::GetStringFromParameterPack(T& first, Targs && ...rest)
	{
		std::ostringstream oss;
		oss << first;
		return oss.str() + GetStringFromParameterPack(rest...);
	}

	template<typename T>
	inline std::string Debug::GetStringFromParameterPack(T& last)
	{
		std::ostringstream oss;
		oss << last;
		return oss.str();
	}
}
