#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <charconv>
//#include <iomanip>

int main(int argc, const char **argv) {
	if (argc < 2) {
		std::cout << "Not enough arguments" << std::endl;
		return 1;
	}

	std::chrono::seconds time(0);
	// Read time arguments, skip first because it's the name of this app
	for (int i = 1; i < argc; ++i) {
		const std::string arg(argv[i]);
		// TODO Check for env args to make cross platform
		if (arg.length() < 2) {
			std::cout << "Argument " << i << " has the wrong format: length < 2"
					<< std::endl;
			return 2;
		}

		const std::string strAmount = arg.substr(0, arg.length() - 1);

		int amount = 0;
		const std::from_chars_result res = std::from_chars(strAmount.data(),
				strAmount.data() + strAmount.size(), amount);
		if (res.ec != std::errc()) {
			std::cout << "Argument " << i
					<< " has the wrong format: Couldn't parse number "
					<< res.ptr << std::endl;
			return 3;
		}

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
			std::cout << "Argument " << i << " is the wrong time type: \""
					<< timeType << "\"" << std::endl;
			return 2;
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

		std::cout << "\r";
		// To string
		const auto hours = std::chrono::duration_cast<std::chrono::hours>(
				displayDuration);
		const int iHours = hours.count();
		if (iHours != 0) {
			displayDuration -= hours;
			std::cout << iHours << "h ";
		}

		const auto minutes = std::chrono::duration_cast<std::chrono::minutes>(
				displayDuration);
		const int iMinutes = minutes.count();
		if (iMinutes != 0) {
			displayDuration -= minutes;
			std::cout << iMinutes << "m ";
		}

		const auto seconds = std::chrono::duration_cast<std::chrono::seconds>(
				displayDuration);
		const int iSeconds = seconds.count();
		if (iSeconds != 0) {
			std::cout << iSeconds << "s ";
		}
		std::cout << std::flush;

		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	std::cout << "\r0s" << std::flush;

	// Signal the end (of the world)
	for (int i = 0; i < 10; i++) {
		std::cout << "\a" << std::flush;
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	std::cout << std::endl;

	return 0;
}
