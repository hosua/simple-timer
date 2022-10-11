#include <bits/stdc++.h>
#include <SFML/Audio.hpp>

using namespace std;

#define SOUND_FILE "alarm.wav"
void usage();
bool isint(string str);

class Timer {
private:
	u_int32_t dur_hr = 0;
	u_int32_t dur_min = 0;
	u_int32_t dur_s = 0;

public:
	Timer(int hr, int min, int sec){
		dur_hr = hr;
		dur_min = min;
		dur_s = sec;
		if (dur_s >= 60){
			dur_min +=  sec / 60;
			dur_s = sec % 60;
		}

		if (dur_min >= 60){
			dur_hr += dur_min / 60;
			dur_min = dur_min % 60;
		}
	}

	void printTime(){
		cout << "Time remaining: ";
		cout << setw(2) << setfill('0') << dur_hr << ":" << 
				setw(2) << setfill('0') << dur_min << ":" <<
				setw(2) << setfill('0') << dur_s;
	}

	void countdown(){
		sf::SoundBuffer sound_buf;
		if (!sound_buf.loadFromFile(SOUND_FILE)){
			cerr << "ERROR: Could not load sound file" << endl;
			exit(EXIT_FAILURE);
		}

		while (dur_s || dur_min || dur_hr){
			printTime();
			if (dur_s == 0){
				if (dur_min == 0){
					if (dur_hr){
						dur_hr--;
						dur_min = 59;
					}
				} else {
					dur_min--;
				}
				dur_s = 60;
			}
			this_thread::sleep_for(chrono::seconds(1));
			cout << flush;
			cout << "\33[2K"; // https://stackoverflow.com/questions/1508490/erase-the-current-printed-console-line
			cout << '\r';
			dur_s--;
		}
		printTime();
		this_thread::sleep_for(chrono::seconds(1));
		cout << flush;
		cout << "\33[2K"; // https://stackoverflow.com/questions/1508490/erase-the-current-printed-console-line
		cout << '\r';
		// dur_s--;
		printTime();
		cout << endl;
		cout << "Times up!\n";
		sf::Sound alarm_sound;
		alarm_sound.setBuffer(sound_buf);
		alarm_sound.play();
		this_thread::sleep_for(chrono::seconds(3));
	}
};

void usage(){
	cout << "-h xx\tSet hours to xx\n"
			"-m xx\tSet minutes to xx\n"
			"-s xx\tSet seconds to ss\n";
}

bool isint(string str){
	for (char ch : str)
		if (!isdigit(ch))
			return false;
	return true;
}

int main(int argc, char** argv){
	int opt;
	int h = 0, m = 0, s = 0;
	if (argc == 1){
		usage();
		exit(EXIT_FAILURE);
	}
	while ((opt = getopt(argc, argv, ":h:m:s:")) != -1){
		switch(opt){
			case 'h':
				if (!isint(optarg)){
					fprintf(stderr, "Option %c has a non-integer value\n", opt);
					exit(EXIT_FAILURE);		
				}
				h = stoi(optarg);
				break;
			case 'm':
				if (!isint(optarg)){
					fprintf(stderr, "Option %c has a non-integer value\n", opt);
					exit(EXIT_FAILURE);		
				}
				m = stoi(optarg);
				break;
			case 's':
				if (!isint(optarg)){
					fprintf(stderr, "Option %c has a non-integer value\n", opt);
					exit(EXIT_FAILURE);		
				}
				s = stoi(optarg);
				break;
			case ':':
				fprintf(stderr, "Option %c missing a value\n", optopt);
				exit(EXIT_FAILURE);
				break;
			case '?':
				cerr << "Unknown option: " << optopt << endl;
				usage();
				exit(EXIT_FAILURE);
				break;
		}
	}

	Timer t(h, m, s);
	t.countdown();
}
