#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <charconv>

#define FMT_HEADER_ONLY
#include "fmt/format.h"


int main(int argc, const char **argv) {

	if (argc < 2) {
		fmt::print("Not enough arguments\n");
		return 1;
	}

	std::chrono::seconds time(0);
	// Read time arguments, skip first because it's the name of this app
	for (int i = 1; i < argc; ++i) {
		const std::string arg(argv[i]);
		// TODO Check for env args to make cross platform
		if (arg.length() < 2) {
			fmt::print("Argument {} has the wrong format: length < 2\n", i);
			return 2;
		}

		// Remove time type indicator character
		const std::string strAmount = arg.substr(0, arg.length() - 1);

		int amount = 0;
		const std::from_chars_result res = std::from_chars(strAmount.data(),
				strAmount.data() + strAmount.size(), amount);
		if (res.ec != std::errc()) {
			// std::from_chars failed
			fmt::print("Argument {0} has the wrong format: Couldn't parse number {1}\n", i, strAmount);
			return 3;
		}
		if (amount < 0) {
			fmt::print("Argument {0} is smaller than 0: {1}\n", i, amount);
			return 4;
		}

		// Decide by time type indicator
		const char timeType = *arg.rbegin();
		switch (timeType) {
		case 'h':
			time += std::chrono::hours(amount);
			break;
		case 'm':
			time += std::chrono::minutes(amount);
			break;
		case 's':
			time += std::chrono::seconds(amount);
			break;
		default:
			fmt::print("Argument {0} is of an unknown time type \'{1}\'. Expected \'h\', \'m\' or \'s\'\n", i, timeType);
			return 5;
		}
	}

	// Count down
	const auto start = std::chrono::system_clock::now();
	const auto end = start + time;

	for (auto current = start; current < end;
			current = std::chrono::system_clock::now()) {
		//auto t_current = std::chrono::system_clock::to_time_t(current);
		//std::cout << std::put_time(std::localtime(&t_current), "%F %T") << std::endl;
		// Current time to display
		auto displayDuration = end - current;

		fmt::print("\r");
		// To string
		const auto hours = std::chrono::duration_cast<std::chrono::hours>(
				displayDuration);
		const int iHours = hours.count();
		if (iHours != 0) {
			displayDuration -= hours;
			fmt::print("{}h ", iHours);
		}

		const auto minutes = std::chrono::duration_cast<std::chrono::minutes>(
				displayDuration);
		const int iMinutes = minutes.count();
		if (iMinutes != 0) {
			displayDuration -= minutes;
			fmt::print("{}m ", iMinutes);
		}

		const auto seconds = std::chrono::duration_cast<std::chrono::seconds>(
				displayDuration);
		const int iSeconds = seconds.count();
		if (iSeconds != 0) {
			fmt::print("{}s ", iSeconds);
		}
		fmt::print("           ");
		std::cout << std::flush;

		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	fmt::print("\r0s               ");

	// Signal the end (of the world)
	for (int i = 0; i < 10; i++) {
		fmt::print("\a");
		std::cout << std::flush;
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	fmt::print("\n");

	return 0;
}
